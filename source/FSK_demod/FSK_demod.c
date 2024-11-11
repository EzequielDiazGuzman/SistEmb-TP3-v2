/***************************************************************************/ /**
   @file     CMP.h
   @brief    CMP functions
   @author   Grupo 3
  ******************************************************************************/

#include "FSK_demod.h"

#define MAX_CNT 50000
#define LOW_F_THRESHHOLD 15500

typedef enum {
    PORT_eDisabled = 0x00,
    PORT_eDMARising = 0x01,
    PORT_eDMAFalling = 0x02,
    PORT_eDMAEither = 0x03,
    PORT_eInterruptDisasserted = 0x08,
    PORT_eInterruptRising = 0x09,
    PORT_eInterruptFalling = 0x0A,
    PORT_eInterruptEither = 0x0B,
    PORT_eInterruptAsserted = 0x0C,
} PORTEvent_t;

typedef enum {
    FSK_IDLE,
    FSK_WAIT,
    FSK_READ,
    FSK_RST,
} FSK_demod_states_t;

/********************************************************************
 * LOCAL FUNCTION DECLARATIONS
********************************************************************/

void FSK_demod_captureSignal(void);
void FSK_demod_processData(void);
static bool checkParity(uint8_t word, uint8_t parity);
void configureCMP(void);
void configureIC(void);
void configureDMA(void);

/********************************************************************
 * LOCAL VARIABLES
 ********************************************************************/

static FSK_demod_states_t FSK_demod_state = FSK_IDLE;
static uint8_t timer_ID;
static uint32_t delta_time = 0;
static uint16_t last_capture = 0;
static uint16_t IC_out = 0;
static queue_t queue = {{0}, 0, 0, false, true};

/********************************************************************
 * GLOBAL FUNCTIONS DEFINITIONS
 ********************************************************************/

void FSK_demod_init()
{
    CMP_init();
    FTM_Init();
    DMA_Init();

    timer_ID = createTimer(833.0f, FSK_demod_processData);

    configureCMP();
    configureIC();
    configureDMA();
}

void FSK_demod_captureSignal(void)
{
    static uint32_t period_counter = 0;

    delta_time = ((IC_out >= last_capture) ? (IC_out - last_capture ): (MAX_CNT + IC_out - last_capture));

    switch (FSK_demod_state)
    {
        case FSK_IDLE:
            if ((delta_time < LOW_F_THRESHHOLD) && (delta_time > 5000))
            {
                gpioWrite(PORTNUM2PIN(PB, 9), HIGH);
             	FSK_demod_state = FSK_WAIT;
            }
            break;
        case FSK_WAIT:
            if (period_counter == 1)
            {
                period_counter = 0;
                FSK_demod_state = FSK_READ;
                startTimer(timer_ID);
                gpioWrite(PORTNUM2PIN(PB, 9), LOW);
                break;
            }
            period_counter++;
            break;
        case FSK_RST:
        case FSK_READ:
            break;
        default:
            FSK_demod_state = FSK_IDLE;
            break;
    }

    last_capture = IC_out;
}

void FSK_demod_processData(void)
{
    static uint8_t output = 0;
    static uint32_t bitCounter = 0;
    static bool parity = 0;

    switch (FSK_demod_state)
    {
        case FSK_READ:
            // gpioWrite(PORTNUM2PIN(PB, 9), HIGH);
            if (bitCounter == 8)
            {
                parity |= delta_time > LOW_F_THRESHHOLD ? 1 : 0;
                bitCounter = 0;
                FSK_demod_state = FSK_RST;
                break;
            }
            output |= delta_time > LOW_F_THRESHHOLD ? (1 << (7 - bitCounter)) : 0;
            bitCounter++;
            // gpioWrite(PORTNUM2PIN(PB, 9), LOW);
            break;
        case FSK_RST:
            stopTimer(timer_ID);
            if (checkParity(output, parity))
                put(&queue, output);
            output = 0;
            parity = 0;
            FSK_demod_state = FSK_IDLE;
            // gpioWrite(PORTNUM2PIN(PB, 9), LOW);
            break;
        case FSK_WAIT:
        case FSK_IDLE:
        default:
            stopTimer(timer_ID);
            FSK_demod_state = FSK_IDLE;
            output = 0;
            parity = 0;
            break;
    }
}

bool FSK_demod_isDataReady()
{
    if (getFillLevel(&queue) != 0)
        return true;
    else
        return false;
}

char FSK_demod_getData(void)
{
    return getNext(&queue);
}

static bool checkParity(uint8_t word, uint8_t parity)
{
    uint8_t i = 0;
    uint8_t parityCalc = 0;
    for (i = 0; i < 8; i++)
        if (word >> i & 1)
            parityCalc ^= 1;

    return (!(parityCalc == parity));
}

void configureCMP (void)
{
    CMP_configureFilterAndHysteresis();
    CMP_enableModuleAndOutput();
    CMP_setFilterPeriod(0x20);
    CMP_configureDAC();
    CMP_selectInputs();
}

void configureIC (void)
{
    //  FTM Configuration
    FTM_SetPrescaler(FTM1, FTM_PSC_x1);
    FTM_SetInterruptMode(FTM1, FTM_CH_0, false); // Enable interrupts
    FTM_SetMod(FTM1, MAX_CNT);

    //	Set FTM as IC
    FTM_SetWorkingMode(FTM1, FTM_CH_0, FTM_mInputCapture); // MSA  / B
    FTM_SetInputCaptureEdge(FTM1, FTM_CH_0, FTM_eEither); // Capture on rising edge
    FTM_SetInputCaptureChannelSource(FTM1, CMP1_OUTPUT);
}

void configureDMA (void)
{
    FTM_DMAMode(FTM1, FTM_CH_0, true);
    FTM_SetInterruptMode(FTM1, FTM_CH_0, true);

    DMA_SetSourceModulo(DMA_CH1, 0);
    DMA_SetDestModulo(DMA_CH1, 0);
    DMA_SetSourceAddr(DMA_CH1, (uint32_t) & (FTM1->CONTROLS[FTM_CH_0].CnV));
    DMA_SetDestAddr(DMA_CH1, (uint32_t)(&IC_out));
    DMA_SetSourceAddrOffset(DMA_CH1, 0);
    DMA_SetDestAddrOffset(DMA_CH1, 0);
    DMA_SetSourceLastAddrOffset(DMA_CH1, 0);
    DMA_SetDestLastAddrOffset(DMA_CH1, 0);
    DMA_SetSourceTransfSize(DMA_CH1, DMA_TransSize_16Bit);
    DMA_SetDestTransfSize(DMA_CH1, DMA_TransSize_16Bit);
    DMA_SetMinorLoopTransCount(DMA_CH1, 2);
    DMA_SetCurrMajorLoopCount(DMA_CH1, 1);
    DMA_SetStartMajorLoopCount(DMA_CH1, 1);
    DMA_SetEnableRequest(DMA_CH1, true);
    DMAMUX_ConfigChannel(DMA_CH1, true, false, kDmaRequestMux0FTM1Channel0);
    DMA_SetChannelInterrupt(DMA_CH1, true, &FSK_demod_captureSignal);

    FTM_ClearInterruptFlag(FTM1, FTM_CH_0);
    FTM_ClearOverflowFlag(FTM1);
    FTM_StartClock(FTM1);
}
