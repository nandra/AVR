#include "iqrf.h"
#include <util/delay.h>


uint8_t iqrfTx[35], iqrfRx[35], crcm, ptype, i, temp;

volatile uint8_t fRxIQRF = 0, fErrIQRF = 0, IQRF_ErrCount = 0;

uint8_t iqrf_send_buff(uint8_t *iqrfTx, uint8_t len)
{
	uint8_t stav = 0;
	
	/* check SPI status max. 10 loops */
	i = 0;
	while (!iqrf_status()) {
		i++;
		if(i>=10)
			goto end;
	}
	
	
	ptype = 0x80+len;
	spi_send_byte(0xF0);
	spi_send_byte(ptype);
	
	crcm = 0x5F ^ 0xF0 ^ ptype;

	/* payload */
	for (i=0; i < len; i++)	{
		crcm ^= iqrfTx[i];
		spi_send_byte(iqrfTx[i]);
	}
	
	spi_send_byte(crcm);

	/* state should be 0x3E (CRC err) or 0x3F (CRC OK) */
 	
	stav = spi_transcieve_byte(0x00) - 0x3E;
end:
	return stav;
}

/* send 1 byte */
uint8_t iqrf_send_byte(uint8_t cData)
{
	iqrfTx[0] = cData;
	return iqrf_send_buff(iqrfTx,1);
}


/* get data from iqrf */
uint8_t iqrf_get_data(uint8_t len)
{
	uint8_t crcs ;
	
	ptype = 0x00+len;

	/* crc slave */
	crcs = ptype ^ 0x5F;
	/* crc master */
	crcm = 0xF0 ^ ptype ^ 0x5F;
	
	spi_send_byte(0xF0);
	spi_send_byte(ptype);

	for (i = 0; i < len; i++) {
		iqrfRx[i] = spi_transcieve_byte(0x00);
		crcs ^= iqrfRx[i];
	}
	
	iqrfRx[len] = spi_transcieve_byte(crcm);

	
	if (crcs == iqrfRx[len]) {
		fRxIQRF = len;
		temp = 1;
	} else {
		temp = 0;
	}

	/* according spec. some dummy bytes */
	spi_send_byte(0x00);
	spi_send_byte(0x00);	
	
	return temp;

}


uint8_t iqrf_status(void)
{
	uint8_t stav;	

	status = spi_transcieve_byte(0x00);
	
	switch (status) {
	case 0x00:	/* SPI disabled SW */
	case 0x07:	/* SPI disabled HW*/
	case 0xFF:	/* HW chyba */
	case 0x81:	/* programming mode */
	case 0x82:	/* debug mode */
		return 0;
	break;

	case 0x83:	/* slow mode - add delay */
		slowModeDelay = 200;
		return 1;
	break;

	case 0x80:	/* SPI ready */
		slowModeDelay = 0;
		return 1;
	break;

	case 0x3E:	/* full buff CRC ERR */
		IQRF_ErrCount++;
	case 0x3F:	/* full buff CRC ok */
		spi_send_byte(0x00);
		return 0;
	break;
			
	/* data available */
	default:
		status -= 0x40;
		
		if ((status > 0) && (status < 42)) 
			iqrf_get_data(status); 
				
		return 0;
	}
			
	return 0;
}

