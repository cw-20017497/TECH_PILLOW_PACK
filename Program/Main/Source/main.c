#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "eeprom.h"
#include "parser.h"

#include "process_sys_event.h"

/* Init Timer */
static void InitStartTimers( void )
{
    InitTimer();

    StartTimer( TIMER_ID_1MS,   1);
    StartTimer( TIMER_ID_10MS,  10);
    StartTimer( TIMER_ID_100MS, 100);
    StartTimer( TIMER_ID_1SEC,  SEC(1));

    StartTimer( TIMER_ID_TOF,   SEC(6) );

    // 40sec는 Cold Temp Sensor 감지 시간으로 초기 10분간 호출되지 않는다.
    StartTimer( TIMER_ID_40SEC, SEC(600));  
    //StartTimer( TIMER_ID_1MIN,  SEC(1));
#if !CONFIG_JIG_RBK_FRONT
    StartTimer( TIMER_ID_DEBUG, 100);
#endif
    StartTimer( TIMER_ID_COMM_TOF_ERR, SEC(10) );
}



void main( void )
{
    Delay_MS( 1500 );

    R_WDT_Restart();

    InitSystem();
    InitStartTimers();
    EI();

    /* EEPROM */
    R_WDT_Restart();
    InitEeprom();

    while(1)
    {
        R_WDT_Restart();

        ProcessEventHandler();

        /* COMM */
        RecvPacketHandler();
        SendPacketHandler();
    }
}
