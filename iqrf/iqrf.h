#ifndef IQRF_H
#define IQRF_H

#include "spi.h"

extern uint8_t iqrfTx[35], iqrfRx[35];
volatile extern uint8_t fRxIQRF, fErrIQRF, IQRF_ErrCount;

extern uint8_t iqrf_send_buff(uint8_t *iqrfTx,uint8_t len);
extern uint8_t iqrf_send_byte(uint8_t cData);
extern uint8_t iqrf_status(void);

#endif
