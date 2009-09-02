#include "main.h"


void uinit( void )
{
  UBRRL = bauddivider;			//set baud rate
  UBRRH = bauddivider >> 8;
  UCSRA = 0;				//no U2X, MPCM
  UCSRC = 1<<URSEL^1<<UCSZ1^1<<UCSZ0;	//8 Bit
  UCSRB = 1<<RXEN^1<<TXEN;		//enable RX, TX
}


void uputchar( char c )
{
  while( (UCSRA & 1<<UDRE) == 0 );
  UDR = c;
}


void uputs( char *s )
{
  while( *s )
    uputchar( *s++ );
}


void uputsnl( char *s )
{
  uputs( s );
  uputchar( 0x0D );
}
