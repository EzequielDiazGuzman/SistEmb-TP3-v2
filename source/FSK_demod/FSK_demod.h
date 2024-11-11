
#ifndef _FSKDEM_H_
#define _FSKDEM_H_

#include <stdint.h>
#include <stdbool.h>

#include "MK64F12.h"

#include "../Queue.h"
#include "../PIT/PIT.h"
#include "../UART/uart.h"
#include "../FTM/FTM.h"
#include "../DMA/DMA.h"
#include "../CMP/CMP.h"


void FSK_demod_init();

bool FSK_demod_isDataReady();

char FSK_demod_getData(void);

#endif /* void _FSKDEM_H_ */
