#include "spi.h"
#include <util/delay.h>

uint8_t i;
volatile uint8_t slowModeDelay = 0;

void spi_init(void)
{
	SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
	SPCR &= ~_BV(SPR0);

	SPSR = 0x00;
}

/* send byte to SPI */
void spi_send_byte(uint8_t cData)
{
	PORTD &= ~_BV(SPI_SS);
	_delay_us(30);
	SPDR = cData;
	loop_until_bit_is_set(SPSR, SPIF);
	_delay_us(30);
	PORTD |= _BV(SPI_SS);
	/* if slow mode add some delay acc. spec. */
	_delay_us(150+2*slowModeDelay);
}

/* send and retrieve byte */
uint8_t spi_transcieve_byte(uint8_t cData)
{
	PORTD &= ~_BV(SPI_SS);
	_delay_us(30);
	SPDR = cData;
	loop_until_bit_is_set(SPSR, SPIF);
	cData = SPDR;
	_delay_us(30);
	PORTD |= _BV(SPI_SS);
	_delay_us(150+2*slowModeDelay);

	return cData;
}
