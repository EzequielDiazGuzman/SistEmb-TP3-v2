/***************************************************************************/ /**
   @file     +Nombre del archivo (ej: template.c)+
   @brief    +Descripcion del archivo+
   @author   +Nombre del autor (ej: Salvador Allende)+
  ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "FSK_mod.h"
#include "../PIT/PIT.h"
#include "../PWM/PWM.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

// +ej: unsigned int anio_actual;+

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

void Mod_PISR(void);
int intToBinaryHART(int num);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
/** Generated using Dr LUT - Free Lookup Table Generator
 * https://github.com/ppelikan/drlut
 **/
// Formula: sin(2*pi*t/T)
static int16_t lut[660] = {
    190, 191, 193, 195, 197, 198, 200,
    202, 204, 205, 207, 209, 211, 212,
    214, 216, 217, 219, 221, 223, 224,
    226, 228, 230, 231, 233, 235, 236,
    238, 240, 241, 243, 245, 247, 248,
    250, 252, 253, 255, 256, 258, 260,
    261, 263, 265, 266, 268, 269, 271,
    272, 274, 276, 277, 279, 280, 282,
    283, 285, 286, 288, 289, 291, 292,
    294, 295, 297, 298, 299, 301, 302,
    304, 305, 306, 308, 309, 310, 312,
    313, 314, 316, 317, 318, 319, 321,
    322, 323, 324, 325, 327, 328, 329,
    330, 331, 332, 333, 335, 336, 337,
    338, 339, 340, 341, 342, 343, 344,
    345, 346, 347, 347, 348, 349, 350,
    351, 352, 353, 353, 354, 355, 356,
    357, 357, 358, 359, 359, 360, 361,
    361, 362, 363, 363, 364, 364, 365,
    366, 366, 367, 367, 367, 368, 368,
    369, 369, 370, 370, 370, 371, 371,
    371, 372, 372, 372, 372, 373, 373,
    373, 373, 373, 373, 374, 374, 374,
    374, 374, 374, 374, 374, 374, 374,
    374, 374, 374, 374, 374, 373, 373,
    373, 373, 373, 373, 372, 372, 372,
    372, 371, 371, 371, 370, 370, 370,
    369, 369, 368, 368, 367, 367, 367,
    366, 366, 365, 364, 364, 363, 363,
    362, 361, 361, 360, 359, 359, 358,
    357, 357, 356, 355, 354, 353, 353,
    352, 351, 350, 349, 348, 347, 347,
    346, 345, 344, 343, 342, 341, 340,
    339, 338, 337, 336, 335, 333, 332,
    331, 330, 329, 328, 327, 325, 324,
    323, 322, 321, 319, 318, 317, 316,
    314, 313, 312, 310, 309, 308, 306,
    305, 304, 302, 301, 299, 298, 297,
    295, 294, 292, 291, 289, 288, 286,
    285, 283, 282, 280, 279, 277, 276,
    274, 272, 271, 269, 268, 266, 265,
    263, 261, 260, 258, 256, 255, 253,
    252, 250, 248, 247, 245, 243, 241,
    240, 238, 236, 235, 233, 231, 230,
    228, 226, 224, 223, 221, 219, 217,
    216, 214, 212, 211, 209, 207, 205,
    204, 202, 200, 198, 197, 195, 193,
    191, 190, 188, 186, 184, 182, 181,
    179, 177, 175, 174, 172, 170, 168,
    167, 165, 163, 162, 160, 158, 156,
    155, 153, 151, 149, 148, 146, 144,
    143, 141, 139, 138, 136, 134, 132,
    131, 129, 127, 126, 124, 123, 121,
    119, 118, 116, 114, 113, 111, 110,
    108, 107, 105, 103, 102, 100, 99,
    97, 96, 94, 93, 91, 90, 88,
    87, 85, 84, 82, 81, 80, 78,
    77, 75, 74, 73, 71, 70, 69,
    67, 66, 65, 63, 62, 61, 60,
    58, 57, 56, 55, 54, 52, 51,
    50, 49, 48, 47, 46, 44, 43,
    42, 41, 40, 39, 38, 37, 36,
    35, 34, 33, 32, 32, 31, 30,
    29, 28, 27, 26, 26, 25, 24,
    23, 22, 22, 21, 20, 20, 19,
    18, 18, 17, 16, 16, 15, 15,
    14, 13, 13, 12, 12, 12, 11,
    11, 10, 10, 9, 9, 9, 8,
    8, 8, 7, 7, 7, 7, 6,
    6, 6, 6, 6, 6, 5, 5,
    5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 6,
    6, 6, 6, 6, 6, 7, 7,
    7, 7, 8, 8, 8, 9, 9,
    9, 10, 10, 11, 11, 12, 12,
    12, 13, 13, 14, 15, 15, 16,
    16, 17, 18, 18, 19, 20, 20,
    21, 22, 22, 23, 24, 25, 26,
    26, 27, 28, 29, 30, 31, 32,
    32, 33, 34, 35, 36, 37, 38,
    39, 40, 41, 42, 43, 44, 46,
    47, 48, 49, 50, 51, 52, 54,
    55, 56, 57, 58, 60, 61, 62,
    63, 65, 66, 67, 69, 70, 71,
    73, 74, 75, 77, 78, 80, 81,
    82, 84, 85, 87, 88, 90, 91,
    93, 94, 96, 97, 99, 100, 102,
    103, 105, 107, 108, 110, 111, 113,
    114, 116, 118, 119, 121, 123, 124,
    126, 127, 129, 131, 132, 134, 136,
    138, 139, 141, 143, 144, 146, 148,
    149, 151, 153, 155, 156, 158, 160,
    162, 163, 165, 167, 168, 170, 172,
    174, 175, 177, 179, 181, 182, 184,
    186, 188};

static queue_t FSKModulation_queue = {{0}, 0, 0, false, true};
static uint8_t actualIndex = 0;
static uint16_t actualModWord;
static bool isSendingWord = false;
static uint8_t parityCalc = 0;
static uint8_t Timerid = 0;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void initFSKMod(void)
{
  PWM_Init();
  PWM_SetTickPerPeriod(379);
  PWM_GenWaveform(lut, 660, 6);
  Timerid = createTimer(833.0, &Mod_PISR);
  startTimer(Timerid);
}

void putArrayFSKMod(queuedata_t *data, uint32_t dataAmount)
{
  putArray(&FSKModulation_queue, data, dataAmount);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

// la interrupcion solo deberia setear el M en cada PIT
void Mod_PISR(void)
{
  // If i'm not sending any word and there is one ready, I charge it on my queue
  if (isSendingWord == false && (getFillLevel(&FSKModulation_queue) != 0))
  {
    isSendingWord = true;
    actualModWord = getNext(&FSKModulation_queue);
    actualIndex = 0;
  }

  // If I started sending a word I'll change M accordingly
  if (isSendingWord == true)
  {
    switch (actualIndex)
    {
		case 0:
		  PWM_SetWaveformOffset(11); // Start bit
		  break;
		case 10:
		  PWM_SetWaveformOffset(6); // Stop bit
		  isSendingWord = false;
		  parityCalc = 0;
		  break;
		case 9:
		{
		  if (parityCalc == 0)
		  {
			PWM_SetWaveformOffset(11);
		  }
		  else
		  {
			PWM_SetWaveformOffset(6);
		  }
		  break;
		}
		default:
		{
		  int bit = (actualModWord >> (8 - actualIndex)) & 1;
		  if (bit == 0)
		  {
			PWM_SetWaveformOffset(11);
		  }
		  else
		  {
			parityCalc ^= 1;
			PWM_SetWaveformOffset(6);
		  }
		  break;
		}
    }
    actualIndex++;
  }
}
