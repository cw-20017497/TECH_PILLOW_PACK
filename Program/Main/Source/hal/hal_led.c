#pragma interrupt INTTM01 r_tau0_channel1_interrupt

#include "hw.h"
#include "hal_led.h"

/*****************************************************************************
 * WdataArray
 *                        bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0
 * COM7  WdataArray[15] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[14] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM6  WdataArray[13] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[12] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM5  WdataArray[11] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[10] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM4  WdataArray[9] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[8] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM3  WdataArray[7] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[6] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM2  WdataArray[5] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[4] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM1  WdataArray[3] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[2] = row07 row06 row05 row04 row03 row02 row01 row00 
 * COM0  WdataArray[1] = row15 row14 row13 row12 row11 row10 row09 row08 
 *       WdataArray[0] = row07 row06 row05 row04 row03 row02 row01 row00 
 */
U8 gu8WdataArray[ DISPLAY_DATA_RAM_LENGTH ];

void HAL_InitLed(void)
{
   R_TAU0_Channel1_Start();   // Start Timer-01
}

void TurnOffAllLED(void)
{
    U8   i;

    for( i = 0; i < DISPLAY_DATA_RAM_LENGTH ; i++ )
    {
        HAL_SetByteDisplayBuffer( i, 0 );
    }

}


void TurnOnAllLED(void)
{
    U8 i;

    for( i = 0; i < DISPLAY_DATA_RAM_LENGTH ; i++ )
    {
        HAL_SetByteDisplayBuffer( i, 0xFF );
    }   
}

void CommonBitOnOff( U32 mu32Val, U8 mu8OnOff )
{
    if( mu8OnOff )
    {
        HAL_SetBitDisplayBuffer( mu32Val, ON );
    }
    else
    {
        HAL_SetBitDisplayBuffer( mu32Val, OFF );
    }
}

void HAL_SetBitDisplayBuffer( U32 mu32Led, U8 mu8OnOff )
{
    U8 mu8Com;
    U8 mu8Row;


    mu8Com = (U8)((mu32Led & 0x00FF0000) >> 16);
    mu8Row = (U8)(mu32Led & 0x0000FFFF);

    if( mu8OnOff != 0 )
    {
        gu8WdataArray[mu8Com]     |= mu8Row;
    }
    else
    {
        gu8WdataArray[mu8Com]     &= ~mu8Row;
    }
}

void HAL_SetByteDisplayBuffer( U8 mu8Index, U8 mu8Val )
{
    gu8WdataArray[ mu8Index ] = mu8Val;
}


// ?îÎ∞ç ?ú??100us timer interrupt
__interrupt static void r_tau0_channel1_interrupt(void)
{
}

