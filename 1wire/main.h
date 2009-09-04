/************************************************************************/
/*                                                                      */
/*                      1-Wire Example					*/
/*                                                                      */
/*              Author: Peter Dannegger                                 */
/*                      danni@specs.de                                  */
/*                                                                      */
/************************************************************************/
#ifndef _main_h_
#define _main_h_
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>
#include <stdlib.h>
#include <stdio.h>

//#define	XTAL		11059200L
#define XTAL	9216000L

#define	BAUD	19200
#define bauddivider (uint)(1.0 * XTAL / BAUD / 16 - 0.5)


#define uchar unsigned char
#define uint unsigned int
#define bit uchar
#define idata
#define code


#define W1_PIN	PA0
#define W1_IN	PINA
#define W1_OUT	PORTA
#define W1_DDR	DDRA

#include "1wire.h"
#include "delay.h"
#include "tempmeas.h"
#include "timebase.h"
#include "uart.h"

#endif
