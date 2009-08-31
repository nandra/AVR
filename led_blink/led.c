#include <avr/io.h>

//#define F_CPU 10000000UL

#include <util/delay.h>

/*
 * Assumptions:
 * - LED connected to PORTB
 * - F_CPU is defined to be your cpu speed (preprocessor define)
 */



int main (void)
{
/* set PORTB for output*/
DDRC = 0xFF;

while (1) {
/* set PORTB high */
PORTC = 0xFF;//_BV(5);
//PORTC |= _BV(6);
_delay_ms(500);

/* set PORTB low */
PORTC = 0x00;// ~_BV(5);
//PORTC &= ~_BV(6);
_delay_ms(500);
}
return 0;
}
