#include "main.h"


void start_meas( void ){
  if( W1_IN & 1<< W1_PIN ){
    w1_command( CONVERT_T, NULL );
    W1_OUT |= 1<< W1_PIN;
    W1_DDR |= 1<< W1_PIN;			// parasite power on

  }else{
    uputsnl( "Short Circuit !" );
  }
}


void read_meas( void )
{
  uchar id[8], diff;
  uchar s[30];
  uchar i;
  uint temp;

  for( diff = SEARCH_FIRST; diff != LAST_DEVICE; ){
    diff = w1_rom_search( diff, id );

    if( diff == PRESENCE_ERR ){
      uputsnl( "No Sensor found" );
      break;
    }
    if( diff == DATA_ERR ){
      uputsnl( "Bus Error" );
      break;
    }
    if( id[0] == 0x28 || id[0] == 0x10 ){	// temperature sensor
      uputs( "ID: " );
      for( i = 0; i < 8; i++ ){
	sprintf( s, "%02X ", id[i] );
	uputs( s );
      }
      w1_byte_wr( READ );			// read command
      temp = w1_byte_rd();			// low byte
      temp |= (uint)w1_byte_rd() << 8;		// high byte
      if( id[0] == 0x10 )			// 9 -> 12 bit
        temp <<= 3;
      sprintf( s, "  T: %04X = ", temp );	// hex value
      uputs( s );
      sprintf( s, "%4d.%01døC", temp >> 4, (temp << 12) / 6553 ); // 0.1øC
      uputsnl( s );
    }
  }
  uputsnl( "" );
}
