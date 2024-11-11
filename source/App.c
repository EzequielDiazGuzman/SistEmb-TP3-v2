/***************************************************************************/ /**
   @file     App.c
   @brief    Application functions
   @author   Nicolás Magliola
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <gpio.h>
#include "board.h"
#include "UART/uart.h"
#include "FSK_mod/FSK_mod.h"
#include "PIT/PIT.h"
#include "FSK_demod/FSK_demod.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 *******************************************************************************
						GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
UARTxConfig_t config = {
	UART_MODE_NORMAL,
	1200,
	UART_DATA_BITS_8,
	UART_PARITY_ODD,
	UART_STOP_BITS_1};

void delay(uint32_t a)
{
	while (a)
	{
		a--;
	}
}

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init(void)
{
	initPIT();

	initFSKMod();
	FSK_demod_init();

	UARTinit(UART_0, &config);

	gpioMode(PORTNUM2PIN(PB, 9), OUTPUT);
	gpioWrite(PORTNUM2PIN(PB, 9), LOW);
}

uint8_t c1 = 0;
uint8_t c2 = 0;

/* Función que se llama constantemente en un ciclo infinito */
void App_Run(void)
{
	if (UARTgetStatus(UART_0) == UARTx_NEW_CHAR)
	{
		UARTgetChar(UART_0, &c1);
		putArrayFSKMod(&c1, 1);
		gpioWrite(PORTNUM2PIN(PB, 9), HIGH);
	}

	if (FSK_demod_isDataReady())
	{
		c2 = FSK_demod_getData();
		gpioWrite(PORTNUM2PIN(PB, 9), LOW);
		//		int i = 0;
		UARTsendString(UART_0, &c2, 1);
	}
}

/*******************************************************************************
 *******************************************************************************
						LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/*******************************************************************************
 ******************************************************************************/
