#include "main.h"


//				Attention !!!
// take care, that during delay no interrupt access a timer register
// or restore TCNT1H


void delay( int d )			// d = 0 ... 32000
{
  d += TCNT1;				// not atomic !

  while( (TCNT1 - d) & 0x8000 );	// not atomic !
}
