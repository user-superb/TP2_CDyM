#ifndef INCFILE1_H_
#define INCFILE1_H_

#define PRENDER_LED(puerto, pin)  ( (puerto) |= (1 << (pin)) )
#define APAGAR_LED(puerto, pin)   ( (puerto) &= ~(1 << (pin)) )
#define TOGGLE_LED(puerto, pin)   ( (puerto) ^= (1 << (pin)) )


#endif /* INCFILE1_H_ */