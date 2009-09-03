#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"
#include "iqrf.h"
#include "uart.h"

FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);


int main(void)
{
	int i = 0;
	// inicializace SPI v master modu
	
	uart_init();
	stdout = stdin = &uart_str;
	printf("test init\n");
	spi_init();
	while(!iqrf_status()) {
		_delay_ms(1);	
	}
	
	if (fRxIQRF) {
		for (i = 0; i < fRxIQRF; i++)
			printf("rx_buff[%d]:%x\n", i, iqrfRx[i]);
	}

	// hlavni smycka programu
	iqrf_send_byte('3');
	for(;;)
	{
	}


	return 0;
}
