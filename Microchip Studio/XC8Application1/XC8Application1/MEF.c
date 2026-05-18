/*
 * MEF.c
 */ 
#define true 1
#define false 0
#include "MEF.h"

//Enumerativo de estados.
typedef enum {INICIAL,INGRESO,COCINANDO,PARADO,FIN,PUERTA_ABIERTA}ESTADO;
ESTADO est_actual;
//¡ENTRADAS MEF!
static int clear,keypad_flag,start,stop,fin,inicioRapido,puerta;
//¡SALIDAS MEF!
static int number;
static uint8_t LEDS;
static uint8_t ticks;
//var buffer
static char keypad_out[7];
//var contador
static int contador;
static int ciclo_fin;
//var bool
static int isNumber;
void iniciarMEF()
{
	est_actual=INICIAL;
	contador = 0;
	number = 0;
	ticks = 0;
	puerta = false;
	LEDS = 0;
}
void actualizarMEF(uint8_t kf, uint8_t pkey)
{
	// LEER ENTRADAS
	if (kf != 0) {
		keypad_flag = true;
		switch (pkey) {
			case('A'): start = true; stop = false; fin = false; inicioRapido = false; puerta = puerta; clear = false; isNumber = false; break;
			case('B'): start = false; stop = true; fin = false; inicioRapido = false; puerta = puerta; clear = true; isNumber = false; break;
			case('C'): start = false; stop = false; fin = false; inicioRapido = true; puerta = puerta; clear = false; isNumber = false; break;
			case('D'): start = false; stop = true; fin = false; inicioRapido = false; puerta = !(puerta); clear = false; isNumber = false; break;
			case('*'): break;
			case('#'): break;
			default: // Número
			isNumber = true;
			//el microondas solo actualiza el número en los estados de ingreso o inicio
			if (est_actual == PUERTA_ABIERTA)
				return;
			if (est_actual == COCINANDO)
				return;
			if (est_actual == PARADO)
				return;
			if (pkey >= '0' && pkey <= '9') {
				number = number * 10 + (pkey - '0');
			}
			start = false; stop = false; fin = false; inicioRapido = false; clear = false;
			break;
		}
	}

	// ACTUALIZAR ESTADOS (Limpiado de snprintf y returns)
	if(puerta) //esto debe hacerse independientemente del estado actual de la máquina.
	{
		est_actual = PUERTA_ABIERTA;
		return;
	}
	switch (est_actual) {
		case PUERTA_ABIERTA:
			if(puerta ==  false)
			{
				est_actual = PARADO; //si la puerta está cerrada, se vuelve al estado parado
			}
			break;
		case INICIAL:
		ciclo_fin = 0;
		if(start && (number != 0)) {
			est_actual = COCINANDO; //si el número ingresado es distinto de 0 y se presionó la tecla A (start), el microondas cocina
			} else if(inicioRapido == true) {
				number = 30;
				est_actual = COCINANDO;//si se presionó el botón C (inicio rápido) se comienza a cocinar con +30 seg.
			} 
			else if(isNumber == true) {
			est_actual = INGRESO; //si se detecta que se ingresó un número en el estado inicial, se pasa al estado de ingreso, ya que num != 0.
			contador++;
		}
		break;
		
		case INGRESO:
		if (keypad_flag == true) {
			if (start == true && puerta == false) {
				est_actual = COCINANDO; //se cocina si se presiona start y la puerta está cerrada (doble verificación)
				} else if (clear == true) {
				number = 0;
				contador = 0;
				est_actual = INICIAL; //si se presiona clear, vuelve al estado inicial.
				} else if (isNumber == true) {
				if (!((number == 0) && (contador == 0))) contador++; //nos aseguramos que number esté dentro de la longitud del display del microondas
				if (contador >= 5) { // tiene que estar en 5
					contador = 0;
					number = 0;
				}
				
				}
				else if(inicioRapido == true) { //sumamos los +30 del inicio rápido
				int min = number / 100;
				int seg = number % 100;
				seg += 30;
				if(seg > 59)
				{
					min++;
					seg = seg - 60;
				}
				number = (min * 100) + seg;
				est_actual = COCINANDO;
			}
		}
		break;
		
		case COCINANDO:
			if (stop) {
				est_actual = PARADO; //se presionó el botón de pausa
				break;
			}
			if (fin) {
				est_actual = FIN; //el contador llegó al fin.
				break;
			}
			//aca actualiza por si el numero es invalido. Preferiria que se hiciera en el estado INGRESO, pero no me salio	
			int min = number / 100;
			int seg = number % 100;
			//lógica para el contador de segundos y minutos
		
			if(min > 59) min = 59;
			if(seg > 59) seg = 59;
			if(inicioRapido)
			{
				seg += 30;
				if(seg > 59)
				{
					if(!(min == 59))
					{
						min++;
						seg = seg - 60;
					}
					else
						seg = 59;
				}
			}
			number = (min * 100) + seg;
			
			ticks++;
			if(ticks >= 100) //Como se ingresa a esta parte del codigo cada 10ms, puedo multiplicar eso por 100 y aproximar 1s
			{
				ticks = 0; 
			
				min = number / 100;
				seg = number % 100;
			
				if (seg > 0) {
					seg--; 
					} else { 
					if (min > 0) {
						min--;   
						seg = 59; 
					}
				}
				number = (min * 100) + seg;
			
				if (number == 0) {
					est_actual = FIN;
				}
			}
			break;
		
		case PARADO:
		if (start && (puerta == false)) {
			est_actual = COCINANDO;
		}
		if (isNumber) 
		{
			number=0;
			contador=0;
			est_actual = INICIAL;
		}
			
		if (clear) {
			contador = 0;
			number = 0; // Agrego esto por seguridad al limpiar
			est_actual = INICIAL;
		}
		if(inicioRapido)
		{
			min = number / 100;
			seg = number % 100;
			seg += 30;
			if(seg > 59)
			{
				min++;
				seg = seg - 60;
			}
			number = (min * 100) + seg;
		}
		
		
		break;
		
		case FIN:			
			if (keypad_flag || ciclo_fin == 5) {
				est_actual = INICIAL;
			}
			
			if (ticks >= 100){
				ciclo_fin++;
				ticks = 0;
			}
			
			ticks++;
		break;
	}

	// bajo flags para que no se crea que el boton es eterno
	keypad_flag = false;
	inicioRapido = false;
	isNumber = false;
	start = false;
	stop = false;
	clear = false;
}

char* actualizarSalida(uint8_t* status_leds)
{
	switch (est_actual) {
		case INICIAL:
		snprintf(keypad_out, sizeof(keypad_out), "00:00 ");
		LEDS &= 0x00; // -- Apago todos los LEDs
		break;
		
		case INGRESO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d ", number / 100, number % 100);

		break;
		
		case COCINANDO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d ", number / 100, number % 100);
		LEDS |= 0x03; // -- Prendo todos los LEDs
		break;
		
		case PARADO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d ", number / 100, number % 100);
		LEDS &= ~0x01; // -- Apago el MAGNETRON (LED 1)
		break;

		case FIN: // -- Alterno el LED 3
		if (ticks < 50) {
			snprintf(keypad_out, sizeof(keypad_out), "FIN   ");
			LEDS = 0x04;
		}
		else {
			snprintf(keypad_out, sizeof(keypad_out), "      ");
			LEDS = 0x00;
		}
		break;
		case PUERTA_ABIERTA:
			snprintf(keypad_out, sizeof(keypad_out), "PUERTA");
			LEDS = 0x02;
		break; 
	}
	
	// Actualizo el estado de los LEDs
	*status_leds = LEDS;
	
	return keypad_out;
}
