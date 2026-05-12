/*
 * MEF.c
 */ 
#define true 1
#define false 0
#include "MEF.h"
//Enumerativo de estados.
typedef enum {INICIAL,INGRESO,COCINANDO,PARADO,FIN}ESTADO;
ESTADO est_actual;
//¡ENTRADAS MEF!
static int clear,keypad_flag,start,stop,fin,inicioRapido,puerta;
//¡SALIDAS MEF!
static char * keypad_;
static int number;
static int LED1,LED2,LED3;
//var buffer
static char keypad_out[6];
//var contador
static int contador;
//var bool
static int isNumber;
void iniciarMEF()
{
	
	est_actual=INICIAL;
	contador = 0;
	number = 0;
}
char * actualizarMEF(uint8_t kf, uint8_t pkey) //recibo si el usuario tocó el teclado y el char que se ingresó.
{

	//leer entradas
	if (kf != 0)
	{
		keypad_flag = true;
		switch (pkey)
		{
			//actualizo los flags de las entradas.
			case('A'): //START.
				start = true;
				stop = false;
				fin = false;
				inicioRapido = false;
				puerta = false;
				clear = false;
				break;
			case('B'): //CLEAR.
				start = false;
				stop = true;
				fin = false;
				inicioRapido = false;
				puerta = false;
				clear = true;
				break;
			case('C'): //INICIO RÁPIDO.
				start = false;
				stop = false;
				fin = false;
				inicioRapido = true;
				puerta = false;
				clear = false;
				break;		
			case('D'): //PUERTA.
				start = false;
				stop = true;
				fin = false;
				inicioRapido = false;
				puerta = false;
				clear = false;	
				break;
			default: //entra acá si se presionó un número
				isNumber = true;
				if (pkey >= '0' && pkey <= '9')
				{
					number = number * 10 + (pkey - '0');
				}
				start = false;
				stop = false;
				fin = false;
				inicioRapido = false;
				puerta = false;
				clear = false;				
			break;
				
		}
	}
	//fin lectura de entradas.
	
	//actualizar estados
	/*
	* ¡ADVERTENCIA!
	*
	*  la parte de actualizar la salida (modificar keypad_out) debería hacerse por fuera de la actualización de datos.
	*  Por el momento está implementado todo junto, está cochino
	*/
	switch (est_actual)
	{
		case(INICIAL):
			if(inicioRapido == true)
			{
				est_actual = COCINANDO;
			}
			else
			{			
				if(isNumber == true)
				{
					est_actual = INGRESO;
					contador++;
					snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d", //este cacho de código debería ser una función
					number / 100,
					number % 100);
					return keypad_out;
					//hasta acá (se repite varias veces).
				}
				
			}


		break;
		case (INGRESO):
			if (keypad_flag == true)
			{

				if (start == true)
				{
					est_actual = COCINANDO;	
				}
				if (clear == true)
				{
					number = 0;
					contador = 0;
					est_actual = INICIAL;
					snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d",
					number / 100,
					number % 100);
					return keypad_out;
				}
				
				contador++;
				if (contador < 4)
				{
					snprintf(keypad_out, sizeof(keypad_out), "%02d:%02d",
					number / 100,
					number % 100);
				}
				else
				{
					contador = 0;
					number = 0;
				}
				return keypad_out;		
			}
			break;
			case (COCINANDO):
			
				if (stop)
				{
					est_actual = PARADO;
				}
				if (fin)
				{
					est_actual = FIN;
				}
			break;
			case (PARADO):
				if (start)
				{
					est_actual = COCINANDO;
				}
				if (clear)
				{
					est_actual = INICIAL;
				}			
			break;
			case (FIN):
			break;
	}
	//actualizo la salida
	/*
	* Acá se debería armar la trama que se envía al LCD por medio de keypad_out.
	* se lo dejo a Christian.
	*/
	
}

