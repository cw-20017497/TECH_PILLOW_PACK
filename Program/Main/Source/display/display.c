#include "hw.h"

#include "display.h"
#include "hal_led.h"

#define NOP_COUNT                   100     /* 32MHz -> 100�� ���� 3.125us*/
U8 gu8Sel = 0;      /* Matrix LED Scan(Source) Counter */
extern U8 gu8WdataArray[ DISPLAY_DATA_RAM_LENGTH ];

void Evt_1msec_LED_Handler(void)
{
    U8 i;       /* NOP() Count */

    /* DELAY TIME
     * LED MATRIX�� ����Ī �Ҷ� ����ϰ� �ٸ� LED���� ������ ���� �����ϱ� ����.
     */
    INIT_LED_SCAN();
    INIT_LED_SELECT();

    for ( i=0; i < NOP_COUNT; i++)
    {
        NOP();
    }

    /* SELECT COM */
    switch(gu8Sel)
    {
        case 0 : SCAN_1 = 0; break;
        case 1 : SCAN_2 = 0; break;

        default : SCAN_2 = 0; break;
    }

    /* LED ALL OFF */
        /* SELECT SEG */
    SELECT_1 = ((gu8WdataArray[gu8Sel] & 0x01));
    SELECT_2 = ((gu8WdataArray[gu8Sel] & 0x02)>>1);

    if(gu8Sel < 1) 
    {
        gu8Sel++;
    }
    else 
    {
        gu8Sel = 0;
    }
} 


void DispColdWaterWeak( U8 mu8OnOff )
{
    CommonBitOnOff( COLD_WATER_WEAK, mu8OnOff );
}

void DispColdWaterStrong( U8 mu8OnOff )
{
    CommonBitOnOff( COLD_WATER_STRONG, mu8OnOff );
}

void DispHotWaterWeak( U8 mu8OnOff )
{
    CommonBitOnOff( HOT_WATER_WEAK, mu8OnOff );
}

void DispHotWaterStrong( U8 mu8OnOff )
{
    CommonBitOnOff( HOT_WATER_STRONG, mu8OnOff );
}


