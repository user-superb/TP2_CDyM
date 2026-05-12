#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "MEF.h"
volatile uint8_t flag10ms = 0;

void init_timer()
{
	TCCR0A |= (1 << WGM01);
	OCR0A = (F_CPU / 1024) * 0.01;
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= (1 << CS02) | (1 << CS00);
}
ISR(TIMER0_COMPA_vect) {
	flag10ms = 1;
}
void init_ports(void) {
	// 1. Configuro los puertos para los LEDs.
	/* Los configuro como salida */
	DDRB |= (1 << DDB5);
	DDRC |= (1 << DDC4) | (1 << DDC5);
	
	/* Por defecto deben estar apagados */
	PORTB &= ~(1 << PORTB5);
	PORTC &= ~((1 << PORTC4) | (1 << PORTC5));
	
	
	// 2. Configuro los puertos para el DISPLAY LED
	/* Los configuro como salida */
	DDRC |= (1 << DDC1) | (1 << DDC2);
	DDRB |= (1 << DDB1) | (1 << DDB2);
	
	DDRD |= (1 << DDD0) | (1 << DDD1); // E y RS
	
	
	// 3. Configuro los puertos para la matríz de botones
	/* Configuro los puertos PB0, PB3, PB4 y PD7 como SALIDA */
	DDRB |= (1 << DDB0) | (1 << DDB3) | (1 << DDB4);
	DDRD |= (1 << DDD7);
	
	/* Inicializo las filas en HIGH (para escanear poniendo en LOW) */
	PORTB |= (1 << PORTB4) | (1 << PORTB3) | (1 << PORTB0);
	PORTD |= (1 << PORTD7);
	
	
	/* Configuro los puertos PD2, PD3, PD4, PD5 como ENTRADA */
	DDRD &= ~((1 << DDD2) | (1 << DDD3) | (1 << DDD4) | (1 << DDD5));
	
	/*Habilitar resistencias Pull-Up internas para las columnas*/
	PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5);
	
}
int main(void) {
	sei();
	LCDinit();
	iniciarMEF(); //inicializar la máquina de estados. Estado Actual = INICIO.
	init_ports();
	init_timer();	
	LCDGotoXY(1,1);
	char pkey;
	uint8_t flag_key;
	LCDclr();
	while (1)
	{
		if(flag10ms == 1)
		{
			flag10ms = 0;
			
			// 1. LEER ENTRADAS
			flag_key = KEYPAD_Scan(&pkey);
			
			// 2. ACTUALIZAR LÓGICA (La MEF se encarga de ignorar la tecla si flag_key es 0)
			actualizarMEF(flag_key, pkey);
			
			// 3. ACTUALIZAR SALIDAS (Siempre, en cada tick)
			LCDGotoXY(1,1);
			LCDstring(actualizarSalida(), 5);
		}
	}
}