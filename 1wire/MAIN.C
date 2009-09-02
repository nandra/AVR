#include "main.h"


int main( void )
{
  init_timer();
  uinit();
  sei();
  uputsnl( "1-Wire-Reader:" );
  second = 0;

  for(;;){				// main loop
    if( second == 1 ){
      start_meas();
      second = 2;
    }
    if( second == 3 ){
      read_meas();
      second = 0;
    }
  }
}
