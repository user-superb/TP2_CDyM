/*
 * keyPad.c
 *
 * Created: 12/5/2026 13:34:48
 */
#include "keypad.h"

char leerTeclado()
{
	char mapa_teclas[4][4] = {
		{'1', '2', '3', 'A'},
		{'4', '5', '6', 'B'},
		{'7', '8', '9', 'C'},
		{'*', '0', '#', 'D'}
	};
	PORTB &= ~(1 << PORTB4);
	_delay_us(10);

	if (!(PIND & (1 << PIND3))) { PORTB |= (1 << PORTB4); return mapa_teclas[0][0]; }
	if (!(PIND & (1 << PIND5))) { PORTB |= (1 << PORTB4); return mapa_teclas[0][1]; }
	if (!(PIND & (1 << PIND4))) { PORTB |= (1 << PORTB4); return mapa_teclas[0][2]; }
	if (!(PIND & (1 << PIND2))) { PORTB |= (1 << PORTB4); return mapa_teclas[0][3]; }
	
	PORTB |= (1 << PORTB4);
	
	return 0xFF;
} 
/********************************************************
FUNCION PARA ESCANEAR UN TECLADO MATRICIAL Y DEVOLVER LA
TECLA PRESIONADA UNA SOLA VEZ. TIENE DOBLE VERIFICACION Y
MEMORIZA LA ULTIMA TECLA PRESIONADA
DEVUELVE:
0 -> NO HAYNUEVA TECLA PRESIONADA
1 -> HAY NUEVA TECLA PRESIONADA Y ES *pkey
********************************************************/
uint8_t KEYPAD_Scan (uint8_t *pkey)
{
	static uint8_t Old_key, Last_valid_key=0xFF; // no hay tecla presionada;
	uint8_t Key;
	Key = leerTeclado(); //devuelve el char.
	if(Key==0xFF){
		Old_key=0xFF; // no hay tecla presionada
		Last_valid_key=0xFF;
		return 0;
	}
	if(Key==Old_key) { //2da verificación
		if(Key!=Last_valid_key){ //evita múltiple detección
			*pkey=Key;
			Last_valid_key = Key;
			return 1;
		}
	}
	Old_key=Key; //1era verificación
	return 0;
}
