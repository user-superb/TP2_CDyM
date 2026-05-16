#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <avr/io.h>

#define PRENDER_LED(puerto, pin)  ( (puerto) |= (1 << (pin)) )
#define APAGAR_LED(puerto, pin)   ( (puerto) &= ~(1 << (pin)) )
#define TOGGLE_LED(puerto, pin)   ( (puerto) ^= (1 << (pin)) )

void init_ports_leds(void);
void actualizarLEDS(uint8_t);


#endif /* INCFILE1_H_ */