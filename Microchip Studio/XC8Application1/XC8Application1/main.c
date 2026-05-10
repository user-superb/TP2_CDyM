#define F_CPU 16000000UL // Asegurate de que coincida con la frecuencia de tu simulación
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

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
	init_ports();
	while (1);
}