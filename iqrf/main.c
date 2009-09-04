#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "iqrf.h"
#include "uart.h"

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

void port_setup(void)
{
	PORTB |= _BV(SPI_SCK)|_BV(SPI_MOSI) | _BV(PB4);
	
	DDRB |= _BV(SPI_SCK)|_BV(SPI_MOSI) | _BV(PB4);
	DDRB &= ~_BV(SPI_MISO);
	/* PD2 is used for SS */
	PORTD |= _BV(SPI_SS);
        DDRD |= _BV(SPI_SS);
}

int main(void)
{
	int i = 0;
	uint8_t stat;
	char buf[10];
	char i_buf[33];
        port_setup();
	uart_init();

	stdout = stdin = &uart_str;
	
	spi_init();
	printf("IQRF v1.0 Press enter...\n");
		
	for(;;)	{
	
		if (fgets(buf, sizeof buf - 1, stdin) == NULL)
        		break;
		
		iqrf_send_byte(buf[0]);
	
		if (fRxIQRF) {
			for (i = 0; i < fRxIQRF; i++) 
				sprintf(&i_buf[i], "%c", iqrfRx[i]);
		
			i_buf[fRxIQRF] = '\0';
			printf("%s\n", i_buf);
			fRxIQRF = 0;
		}
	}
	
	return 0;
}
