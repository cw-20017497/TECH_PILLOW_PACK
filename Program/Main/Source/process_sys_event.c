#include <stdio.h>
#include <string.h>

#include "prj_type.h"
#include "timer.h"
#include "process_sys_event.h"

#include "process_display.h"
#include "relay.h"
#include "valve.h"

#include "temp.h"
#include "key.h"
#include "process_key.h"

#include "api_adc.h"
#include "hal_led.h"
#include "hal_serial.h"
#include "Display.h"
#include "process_make.h"

#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "eeprom.h"

#include "error.h"
#include "process_error.h"

#include "eol.h"
#include "process_eol.h"
#include "fct.h"
#include "process_fct.h"
#include "time_short.h"


#include "level_electrode.h"
#include "parser_tof.h"


typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void INTT_BaseTime_Handler ( void );
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_100ms_Handler( void );
static void Evt_1sec_Handler( void );
static void Evt_40sec_Handler( void );
static void Evt_1min_Handler( void );
static void Evt_Debug_Handler( void );
static void Evt_TOF_Handler( void );

/* EOL */
static void Evt_EOL_1ms_Handler(void);
static void Evt_EOL_10ms_Handler(void);
static void Evt_EOL_100ms_Handler(void);
static void Evt_EOL_1sec_Handler( void );

/* FCT */
static void Evt_FCT_1ms_Handler( void );
static void Evt_FCT_10ms_Handler( void );
static void Evt_FCT_100ms_Handler(void);
static void Evt_FCT_1sec_Handler( void );

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                    NORMAL,                     FCT,                    EOL */
    { TIMER_ID_1MS,                 Evt_1ms_Handler,            Evt_FCT_1ms_Handler,    Evt_EOL_1ms_Handler },
    { TIMER_ID_10MS,                Evt_10ms_Handler,           Evt_FCT_10ms_Handler,   Evt_EOL_10ms_Handler },
    { TIMER_ID_100MS,               Evt_100ms_Handler,          Evt_FCT_100ms_Handler,  Evt_EOL_100ms_Handler },
    { TIMER_ID_1SEC,                Evt_1sec_Handler,           Evt_FCT_1sec_Handler,   Evt_EOL_1sec_Handler },
    { TIMER_ID_40SEC,               Evt_40sec_Handler,          NULL,                   NULL },
    { TIMER_ID_TOF,               Evt_TOF_Handler,            NULL,                   NULL },

#if CONFIG_MMI
    { TIMER_ID_DEBUG,               Evt_Debug_Handler,          Evt_Debug_Handler,      Evt_Debug_Handler },
#endif
};
#define	SZ_LIST		( sizeof( SysEventList ) / sizeof( SysEvent_T ) )

void ProcessEventHandler( void )
{
    U8 i;
    Action_T fun;

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( IsExpiredTimer( SysEventList[ i ].timerId ) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer( SysEventList[ i ].timerId );

            // NORMAL or EOL 
            if( GetEolStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                fun = SysEventList[ i ].pFun_EOL; 
            }
            else if( GetFctStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                fun = SysEventList[ i ].pFun_FCT; 
            }
            else
            {
                // Get Normal Mode Event Handler
                fun = SysEventList[ i ].pFun;
            }

            // Call a event handl
            if( fun != NULL )
            {
                fun();
            }
        }
    }
}

static void INTT_BaseTime_Handler ( void )
{
    U8 mu8KeyMatrixCount;

    Evt_1msec_LED_Handler();

}

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    ProcessAdc();
    ProcessLevelElec();
}


static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);


    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    ProcessScanLevelElec();

    ControlValve();

    ControlRelayConcurrent();

}


static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);
    
    ProcessTemp();

    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */

    ControlRoomWaterLevel();  /* Control Room Tank Water Level */
    ControlColdWaterLevel();  /* Control Cold Tank Water Level */
    ControlHotWaterLevel();  /* Control Hot Tank Water Level */

    ProcessError();
    ProcessDisplay();

    UpdateEolTimer();

}


static void Evt_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

#if CONFIG_TEST_COLD_SENSOR
    if( GetColdWaterConfigMake() == TRUE )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
#endif

    /* Make Cold Water */
    MakeColdWater();

    /* Make Hot Water */
    MakeHotWater();
    ProcessUnusedSaving();

    /* Process making  cold water & ice */
    ProcessMake();

    ControlRelayExclusive();


    /* EEPROM Writting */
    ProcessEeprom();

    UpdateFctTimer();


    // Time Short 
    UpdateTimeShortTimer();
    ProcessTimeShort();
}

static void Evt_40sec_Handler( void )
{
    StartTimer( TIMER_ID_40SEC, SEC(40) );

    // 10초 주기로 1초 동안만 냉수 센서 읽기
    if( GetColdWaterConfigMake() == TRUE )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
}

U8 InitPowerOn = FALSE;
U8 PowerOnAck = FALSE;

U8 tof_start = TRUE;
#define TOF_RANGE_ERROR     22222
I16 tof_sensor[7]= 
{ 
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR,
    TOF_RANGE_ERROR
};
U16 timer_delay = 100;

static void Evt_TOF_Handler( void )
{
    //if( InitPowerOn == FALSE )
    //{
    //    // 1. calibration 
    //    if( PowerOnAck == FALSE )
    //    {
    //        SetCommHeader( COMM_ID_TOF, PKT_TOF_REQ_CALI );
    //        StartTimer( TIMER_ID_COMM_TOF_TX, 0 );
    //    }
    //    else
    //    {
    //        // 2. sensing
    //        SetCommHeader( COMM_ID_TOF, PKT_TOF_REQ );
    //        StartTimer( TIMER_ID_COMM_TOF_TX, 0 );
    //    }
    //    StartTimer( TIMER_ID_TOF, 300 );    // wait ack 300ms...
    //}

    //else
    {
        // 2. sensing
        SetCommHeader( COMM_ID_TOF, PKT_TOF_REQ );
        StartTimer( TIMER_ID_COMM_UART_1_TX, 0 );

        StartTimer( TIMER_ID_TOF, timer_delay );
    }

}

static void Evt_1min_Handler( void )
{
    StartTimer( TIMER_ID_1MIN, SEC(60) );
}

static void Evt_Debug_Handler( void )
{
    StartTimer( TIMER_ID_COMM_DEBUG_TX, 0 );
    StartTimer( TIMER_ID_DEBUG, 150 );

}

void InitSystem(void)
{
    /* IO */
    InitRelay();
    InitValve();
    InitKey();
    InitAdc();
    HAL_InitLed();
    HAL_InitComm();

    InitDisplay();


    /* WATER */
    InitRoomWater();
    InitColdWater();
    InitHotWater();

    /* ERROR */
    InitErrorList();
    
    InitProcessMake();

    InitEol();
    InitFct();
    InitTimeShort();


    // iμœi´? ?‰?? ?¼i?œ i´?e¸??½e¸°
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    /* baseTimer ½CCa CO¼o μi·I */
    RegisterTimerISR( INTT_BaseTime_Handler );
}

static void Evt_FCT_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);

    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    ProcessAdc();
}

static void Evt_EOL_1ms_Handler(void)
{
    StartTimer( TIMER_ID_1MS, 1);

    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    ProcessAdc();
}

static void Evt_EOL_10ms_Handler(void)
{
    U8 mu8EolMode;

    StartTimer( TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();
    

    ControlValve();

    ControlRelayConcurrent();

}

static void Evt_EOL_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);

    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */

    ProcessDisplay();

    /* EOL */
    UpdateEolTimer();
    ProcessEol();

    ControlRelayExclusive();
}


static void Evt_EOL_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    // EOL에서 heater call 
    //ControlRelayExclusive();
}


/* FCT */
static void Evt_FCT_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    ProcessFctLoadTest();

    ControlValve();
}


static void Evt_FCT_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);
}

static void Evt_FCT_1sec_Handler( void )
{
    StartTimer( TIMER_ID_1SEC, SEC(1) );

    UpdateFctTimer();
    ControlRelayExclusive();
    ControlRelayConcurrent();

    ProcessFct();
}

