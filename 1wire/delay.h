#ifndef _delay_h_
#define _delay_h_


#define DELAY_US(x)	((uint)( (x) * 1e-6 * XTAL ))
#define DELAY(x)	delay(x)

void delay( int d );


#endif
