#include "hw.h"
#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "eeprom.h"
#include "tof.h"
#include "parser.h"
#include "process_sys_event.h"

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_100MS, 100);
    StartTimer( TIMER_ID_TOF,  SEC(5));
}


void main( void )
{
    Delay_MS( 500 );

    R_WDT_Restart();

    InitSystem();
    InitStartTimers();
    EI();

    /* EEPROM */
    R_WDT_Restart();
    //InitEeprom();

    /* TOF */
    InitToF();
    
    while(1)
    {
        R_WDT_Restart();

        ProcessEventHandler();

        /* COMM */
        RecvPacketHandler();
        SendPacketHandler();
    }
}
