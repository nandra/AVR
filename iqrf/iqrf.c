#include "iqrf.h"
#include <util/delay.h>


uint8_t iqrfTx[35], iqrfRx[35], crcm, ptype, i, temp;

volatile uint8_t fRxIQRF = 0, fErrIQRF = 0, IQRF_ErrCount = 0;

uint8_t iqrf_send_buff(uint8_t *iqrfTx, uint8_t delka)
{
	uint8_t stav;
	
	// smycka dokud neni SPI ready s omezenim na 10 pruchodu
	i = 0;
	while(!iqrf_status())
	{
		i++;
		if(i>=10)
			return 0;
	}
	
	// typ prikazu - zapis dat o delce <delka>
	ptype = 0x80+delka;
	
	// prvni byte urcuje vyslani prikazu (0xF0)
	spi_send_byte(0xF0);
	// druhy byte urcuje ze jde o FD prenos (0x80) + delka zpravy
	spi_send_byte(ptype);
	
	// zacatek vypoctu CRCM
	crcm = 0x5F ^ 0xF0 ^ ptype;

	// payload
	for(i=0;i<delka;i++)
	{
		// postupne pocitame CRCM
		crcm ^= iqrfTx[i];

		// a odesilame byty po SPI
		spi_send_byte(iqrfTx[i]);
	}
	
	// nakonec posleme CRCM, coz je XOR vsech prenasenych bytu a bytu 0x5F
	spi_send_byte(crcm);

	// nakonec zkontrolujeme, jestli modul prijal data v poradku,
	// pokud ano, vrati 3F, pokud ne, vrati 3E

 	// stav by mel byt 0x3F v pripade CRC ok, nebo 0x3E v pripade CRC Err
	// odectenim se z hodnot udela TRUE, nebo FALSE
	stav = spi_transcieve_byte(0x00) - 0x3E;

	return stav;
}

// pomocna fce na odeslani jedineho bytu, ktera vyuziva vyse uvedenou fci
uint8_t iqrf_send_byte(uint8_t cData)
{
	iqrfTx[0] = cData;
	return iqrf_send_buff(iqrfTx,1);

}


// fce na vyzvednuti dat z IQRF
uint8_t iqrf_get_data(uint8_t delka)
{
	uint8_t crcs ;
	
	// typ prikazu - cteni dat o delce <delka>
	ptype = 0x00+delka;

	// musme pocitat CRC pro master i slave, data odesilame a zaroven prijimame
	crcs = ptype ^ 0x5F;
	
	crcm = 0xF0 ^ ptype ^ 0x5F;
	
	// prvni byte urcuje vyslani prikazu (0xF0)
	spi_send_byte(0xF0);
	// druhy byte urcuje ze jde o HD prenos (0x00) + delka zpravy
	spi_send_byte(ptype);

	// postupne vyzvednuti vsech bytu
	for(i=0;i<delka;i++)
	{
		// vyslani bytu 0x00, jde nam jen o data z IQRF
		iqrfRx[i] = spi_transcieve_byte(0x00);

		// prubezny vypocet CRCS
		crcs ^= iqrfRx[i];

	}
	
	// nakonec posleme CRCM, coz je XOR vsech prenasenych bytu a bytu 0x5F
	// a ulozime si prijate CRCS, ktere vypocital modul IQRF
	iqrfRx[delka] = spi_transcieve_byte(crcm);

	// pokud souhlasi CRCS vypoctene a prijate
	if(crcs	== iqrfRx[delka])
	{
		fRxIQRF = delka; // flag zaroven obsahuje pocet prijatych bytu
		
		// vratime true
		temp = 1;

	}
	else
	{
		// CRCS nesedi, vratime false
		temp = 0;
	}

	// par zdrzovacich bytu
	spi_send_byte(0x00);
	spi_send_byte(0x00);	
	
	// fce vraci stav
	return temp;

}


uint8_t iqrf_status(void)
{
	uint8_t stav;	

	// IQRF vrati svuj stav po prijeti 0x00
	stav = spi_transcieve_byte(0x00);
	
	switch(stav)
	{
	case 0x00:	// SPI vypnuto softwarove	disableSPI();
	case 0x07:	// SPI uzivatelsky zakazano stopSPI();
	case 0xFF:	// HW chyba
	case 0x81:	// prgramming mode
	case 0x82:	// debug mode

		// za techto okolnosti nelze komunikovat, vratime false
		return 0;
	break;

	case 0x83:	// slow mode, musime pridat prodlevu mezi odesilane byty
		slowModeDelay = 200;
		return 1;
	break;

	case 0x80:	// SPI ceka na prikaz
		slowModeDelay = 0;
		return 1;
	break;

	case 0x3E:	// plny buff CRC ERR
		IQRF_ErrCount++;
	case 0x3F:	// plny buff CRC ok
		// plny buffer v IQRF, posleme zdrzovaci byte a koncime
		spi_send_byte(0x00);
		return 0;
	break;
			
	// pokud je to jina hodnota, melo by se jedna o cekajici data
	default:
		// staqv - 0x40 udava pocet bytu k vyzvednuti, pokud je v rozmezi 0 - 42 dekadicky
		stav -= 0x40;
		
		if( (stav > 0) && (stav < 42) )
		{
			// v bufferu jsou data a je treba je vyzvednout pocet = stav
			iqrf_get_data(stav); // v pripade uspechu nastavi fce flag fRxIQRF
		
		}
		// ale SPI je mozna stale obsazene, takze vratime FALSE
		return 0;
	}
		
		
	return 0;
}

