#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

/* SPI PORT */
#define SPI_PORT 	PORTB
#define SPI_PORT_DDR 	DDRB

/* SPI pins */
#define SPI_SS   PD2
#define SPI_SCK  PB7
#define SPI_MOSI PB5
#define SPI_MISO PB6

volatile extern uint8_t slowModeDelay;

extern void spi_init(void);
extern void spi_send_byte(uint8_t cData);
extern uint8_t spi_transcieve_byte(uint8_t cData);

#endif
