#include "spi.h"
#include <util/delay.h>

uint8_t i;
volatile uint8_t slowModeDelay = 0;

void spi_init(void)
{
	//SPI_PORT_DDR |= _BV(SPI_SCK)|_BV(SPI_MOSI);
	//SPI_PORT_DDR &= ~_BV(SPI_MISO);

	//SPI_PORT |= _BV(SPI_SCK)|_BV(SPI_MOSI);
	//SPI_PORT &= ~_BV(SPI_MISO);
	
	SPCR = 0x52;
	SPSR = 0x00;

	//DDRD |= _BV(SPI_SS);
	//PORTD |= _BV(SPI_SS);
}

/* send byte to SPI */
void spi_send_byte(uint8_t cData)
{
	PORTD &= ~_BV(PD2);
	_delay_us(30);
	SPDR = cData;
	loop_until_bit_is_set(SPSR, SPIF);
	_delay_us(30);
	PORTD |= _BV(PD2);
	/* if slow mode add some delay acc. spec. */
	_delay_us(150+2*slowModeDelay);
}

/* send and retrieve byte */
uint8_t spi_transcieve_byte(uint8_t cData)
{
	PORTD &= ~_BV(PD2);
	_delay_us(30);
	SPDR = cData;
	loop_until_bit_is_set(SPSR, SPIF);
	cData = SPDR;
	_delay_us(30);
	PORTD |= _BV(PD2);
	_delay_us(150+2*slowModeDelay);

	return cData;
}
