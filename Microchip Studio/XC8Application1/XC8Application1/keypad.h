/*
 * IncFile1.h
 *
 * Created: 12/5/2026 13:38:30
 *  Author: exaul
 */ 

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL
#ifndef keypad_H_
#define keypad_H_
#endif /* keypad_H_ */
char leerTeclado();
uint8_t KEYPAD_Scan (uint8_t *pkey);