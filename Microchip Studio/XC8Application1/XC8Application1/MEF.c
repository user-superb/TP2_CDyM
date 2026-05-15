/*
 * MEF.c
 */ 
#define true 1
#define false 0
#include "MEF.h"

#include "led.h"

//Enumerativo de estados.
typedef enum {INICIAL,INGRESO,COCINANDO,PARADO,FIN}ESTADO;
ESTADO est_actual;
//¡ENTRADAS MEF!
static int clear,keypad_flag,start,stop,fin,inicioRapido,puerta;
//¡SALIDAS MEF!
static int number;
static int LED1,LED2,LED3;
static uint8_t ticks;
//var buffer
static char keypad_out[6];
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
}
void actualizarMEF(uint8_t kf, uint8_t pkey)
{
	// LEER ENTRADAS
	if (kf != 0) {
		keypad_flag = true;
		switch (pkey) {
			case('A'): start = true; stop = false; fin = false; inicioRapido = false; puerta = false; clear = false; isNumber = false; break;
			case('B'): start = false; stop = true; fin = false; inicioRapido = false; puerta = false; clear = true; isNumber = false; break;
			case('C'): start = false; stop = false; fin = false; inicioRapido = true; puerta = false; clear = false; isNumber = false; break;
			case('D'): start = false; stop = true; fin = false; inicioRapido = false; puerta = false; clear = false; isNumber = false; break;
			case('*'): break;
			case('#'): break;
			default: // Número
			isNumber = true;
			if (est_actual == COCINANDO)
				return;
			if (pkey >= '0' && pkey <= '9') {
				number = number * 10 + (pkey - '0');
			}
			start = false; stop = false; fin = false; inicioRapido = false; puerta = false; clear = false;
			break;
		}
	}

	// ACTUALIZAR ESTADOS (Limpiado de snprintf y returns)
	switch (est_actual) {
		case INICIAL:
		ciclo_fin = 0;
		if(start) {
			est_actual = COCINANDO;
			} else if(inicioRapido == true) {
				number = 30;
				est_actual = COCINANDO;
				PRENDER_LED(PORTB, PORTB5);
				PRENDER_LED(PORTC, PORTC4);
			} 
			else if(isNumber == true) {
			est_actual = INGRESO;
			contador++;
		}
		break;
		
		case INGRESO:
		if (keypad_flag == true) {
			if (start == true) {
				est_actual = COCINANDO;
				PRENDER_LED(PORTB, PORTB5);
				PRENDER_LED(PORTC, PORTC4);
				} else if (clear == true) {
				number = 0;
				contador = 0;
				est_actual = INICIAL;
				} else if (isNumber == true) {
				if (!((number == 0) && (contador == 0))) contador++;
				if (contador >= 5) { // tiene que estar en 5
					contador = 0;
					number = 0;
				}
				
				}
				else if(inicioRapido == true) {
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
				PRENDER_LED(PORTB, PORTB5);
				PRENDER_LED(PORTC, PORTC4);
			}
		}
		break;
		
		case COCINANDO:
			if (stop) {
				est_actual = PARADO;
				APAGAR_LED(PORTB, PORTB5);
				break;
			}
			if (fin) {
				est_actual = FIN;
				break;
			}
			//aca actualiza por si el numero es invalido. Preferiria que se hiciera en el estado INGRESO, pero no me salio	
			int min = number / 100;
			int seg = number % 100;
		
		
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
		if (start) {
			est_actual = COCINANDO;
			PRENDER_LED(PORTB, PORTB5);
		}
		if (isNumber) est_actual = INGRESO;
		if (clear) {
			number = 0; // Agrego esto por seguridad al limpiar
			est_actual = INICIAL;
			APAGAR_LED(PORTB, PORTB5);
			APAGAR_LED(PORTC, PORTC4);
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
			APAGAR_LED(PORTB, PORTB5);
			APAGAR_LED(PORTC, PORTC4);
			if (keypad_flag || ciclo_fin == 5) {est_actual = INICIAL;}
			if (ticks == 100){
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

char* actualizarSalida()
{
	switch (est_actual) {
		case INICIAL:
		snprintf(keypad_out, sizeof(keypad_out), "00:00");
		APAGAR_LED(PORTC, PORTC5); // Me aseguro que LED3 esté apagado en el estado inicial
		break;
		
		case INGRESO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d", number / 100, number % 100);

		break;
		
		case COCINANDO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d", number / 100, number % 100);
		
		case PARADO:
		snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d", number / 100, number % 100);
		break;

		case FIN:
		if (ticks < 50) {
			snprintf(keypad_out, sizeof(keypad_out), "FIN  ");
			PRENDER_LED(PORTC, PORTC5);
		}
		else {
			snprintf(keypad_out, sizeof(keypad_out), "     ");
			APAGAR_LED(PORTC, PORTC5);
		}
		break;
	}
	return keypad_out;
}
