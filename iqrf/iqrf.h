#ifndef IQRF_H
#define IQRF_H

#include "spi.h"

extern uint8_t iqrfTx[35], iqrfRx[35];
volatile extern uint8_t fRxIQRF, fErrIQRF, IQRF_ErrCount;

// odeslani zpravy dane delky, fce se postara o rezijni byty
extern uint8_t iqrf_send_buff(uint8_t *iqrfTx,uint8_t delka);

// odeslani jednoho bytu
extern uint8_t iqrf_send_byte(uint8_t cData);

// kontrola stavu IQRF
extern uint8_t iqrf_status(void);

#endif
