/***************************************************************************/ /**
   @file     CMP.h
   @brief    CMP functions
   @author   Grupo 3
  ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 * ****************************************************************************/

#include "CMP.h"

/*****************************************************************************
 * TYPEDEF AND MACRO DEFINITIONS
 ****************************************************************************/

enum PORT_MUX {
	PORT_mAnalog,
	PORT_mGPIO,
    PORT_mAlt2,
    PORT_mAlt3,
    PORT_mAlt4,
    PORT_mAlt5,
    PORT_mAlt6,
    PORT_mAlt7,
};

/******************************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 * ****************************************************************************/

void CMP_init(void)
{
	// Enable clock for CMP module
    SIM->SCGC4 |= SIM_SCGC4_CMP_MASK;

	// PTC3: CMP1_IN
	PORTC->PCR[3] = PORT_PCR_MUX(0) | PORT_PCR_IRQC(0);
	// PTC4: CMP1_OUT
	PORTC->PCR[4] = PORT_PCR_MUX(6) | PORT_PCR_IRQC(0) | PORT_PCR_DSE(true);
}

void CMP_configureFilterAndHysteresis(void)
{
	CMP1->CR0 = CMP_CR0_FILTER_CNT(0x07) | CMP_CR0_HYSTCTR(0x03); // Filtro de 3 muestras e histeresis de 30mv
}

void CMP_enableModuleAndOutput(void)
{
	CMP1->CR1 = CMP_CR1_EN(true) | CMP_CR1_OPE(1) | CMP_CR1_PMODE(1); // Config High Speed, salida filtrada Habilitado el pin y el modulo
}

void CMP_setFilterPeriod(uint8_t period)
{
	CMP1->FPR = period; // Cuantos ciclos de bus clock quiero usar para que el filtro filtre (Periodo de filtrado)
}

void CMP_configureDAC(void)
{
	CMP1->DACCR = CMP_DACCR_DACEN(1) | CMP_DACCR_VOSEL(31) | CMP_DACCR_VRSEL(1); // Habilito el DAC y hago que divida Vref por la mitad, Vref es Vin1
}

void CMP_selectInputs(void)
{
	CMP1->MUXCR = CMP_MUXCR_PSEL(1) | CMP_MUXCR_MSEL(7);
}