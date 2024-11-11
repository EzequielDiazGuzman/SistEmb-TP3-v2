#ifndef CMP_H
#define CMP_H

/***************************************************************************/ /**
   @file     CMP.h
   @brief    CMP header file
   @author   Grupo 3
  ******************************************************************************/

/******************************************************************************
 * INCLUDE HEADER FILES
 * ****************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "../../SDK/CMSIS/MK64F12.h"

/**
 * @brief 
 * 
 */
void CMP_init(void);

void CMP_configureFilterAndHysteresis(void);
void CMP_enableModuleAndOutput(void);
void CMP_setFilterPeriod(uint8_t period);
void CMP_configureDAC(void);
void CMP_selectInputs(void);

 #endif // CMP_H