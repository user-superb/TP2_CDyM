/*
 * IncFile1.h
 *
 * Created: 12/5/2026 15:18:31
 *  Author: exaul
 */ 

#include <avr/io.h>
#ifndef MEF_H_
#define MEF_H_
#endif /* MEF_H_ */
void iniciarMEF();
//char * actualizarMEF(uint8_t kf, uint8_t pkey);
void actualizarMEF(uint8_t kf, uint8_t pkey, uint8_t* status_leds);
char * actualizarSalida();