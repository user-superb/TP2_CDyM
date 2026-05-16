#include "led.h"

void init_ports_leds(void) {
	// 1. Configuro los puertos para los LEDs.
	/* Los configuro como salida */
	DDRB |= (1 << DDB5);
	DDRC |= (1 << DDC4) | (1 << DDC5);
	
	/* Por defecto deben estar apagados */
	PRENDER_LED(PORTB, PORTB5);
	APAGAR_LED(PORTC, PORTC4); APAGAR_LED(PORTC, PORTC5);
}

void actualizarLEDS(uint8_t status_leds) {
	
	// Evaluo el Bit 0 (Led 1 - Magnetrón)
	if (status_leds & 0x01) {
		PRENDER_LED(PORTB, PORTB5);
		} else {
		APAGAR_LED(PORTB, PORTB5);
	}
	
	// Evaluo el Bit 1 (Led 2 - Luz interior)
	if (status_leds & 0x02) {
		PRENDER_LED(PORTC, PORTC4);
		} else {
		APAGAR_LED(PORTC, PORTC4);
	}
	
	// Evaluo el Bit 2 (Led 3 - Alarma)
	if (status_leds & 0x04) {
		PRENDER_LED(PORTC, PORTC5);
		} else {
		APAGAR_LED(PORTC, PORTC5);
	}
}