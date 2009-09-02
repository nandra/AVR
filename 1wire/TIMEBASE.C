/************************************************************************/
/*                                                                      */
/*			Precise 1 Second Timebase			*/
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#include "main.h"


#define DEBOUNCE	256L		// debounce clock (256Hz = 4msec)


uchar prescaler;
uchar volatile second;			// count seconds


SIGNAL (SIG_OUTPUT_COMPARE1A)
{
  uchar tcnt1h = TCNT1H;

  OCR1A += XTAL / DEBOUNCE;		// new compare value

  if( ++prescaler == (uchar)DEBOUNCE ){
    prescaler = 0;
    second++;				// exact one second over
#if XTAL % DEBOUNCE			// handle remainder
    OCR1A += XTAL % DEBOUNCE; 		// compare once per second
#endif
  }
  TCNT1H = tcnt1h;			// restore for delay() !
}


void init_timer( void )
{
  TCCR1B = 1<<CS10;			// divide by 1
  OCR1A = 0;
  TCNT1 = -1;
  second = 0;
  prescaler = 0;

  TIMSK = 1<<OCIE1A;
}
