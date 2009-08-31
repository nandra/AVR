//**************************************************************
//******** FUNCTIONS FOR SERIAL COMMUNICATION USING UART *******
//**************************************************************
//Controller: ATmega8 (Clock: 8 Mhz-internal)
//Compiler: AVR-GCC
//Version : 2.0
//Author: CC Dharmani, Chennai (India)
// www.dharmanitech.com
//Date: 26 Feb 2009
//**************************************************************

//**************************************************
// ***** HEADER FILE : UART_routines.h ******
//**************************************************

#ifndef _UART_ROUTINES_H_
#define _UART_ROUTINES_H_

#define TX_NEWLINE {transmitByte(0x0d); transmitByte(0x0a);}
/*
#define CHAR 2 
#define INT 4
#define LONG 8*/

unsigned char receiveByte(void);
void transmitByte(unsigned char);

void transmitHex( unsigned char dataType, unsigned long data );
void transmitString_F(char*);
void transmitString(unsigned char*);
void uart_init(void);

#endif
