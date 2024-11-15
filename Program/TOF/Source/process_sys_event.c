#include <stdio.h>
#include <string.h>

#include "prj_type.h"
#include "timer.h"
#include "hal_serial.h"
#include "tof.h"
#include "parser.h"
#include "parser_tof.h"
#include "process_sys_event.h"


typedef void(*Action_T)(void);
typedef struct _sys_event_
{
    U8   timerId;
    Action_T pFun;
    Action_T pFun_FCT;
    Action_T pFun_EOL;
} SysEvent_T;

/* NORMAL */
static void Evt_1ms_Handler( void );
static void Evt_10ms_Handler( void );
static void Evt_100ms_Handler( void );
static void Evt_tof_Handler( void );

const static SysEvent_T	SysEventList[] =
{
    /* TIMER ID,                    NORMAL,                     FCT,        EOL */
    { TIMER_ID_1MS,                 Evt_1ms_Handler,            NULL,       NULL },
    { TIMER_ID_10MS,                Evt_10ms_Handler,           NULL,       NULL },
    { TIMER_ID_100MS,               Evt_100ms_Handler,          NULL,       NULL },
    { TIMER_ID_TOF,                 Evt_tof_Handler,            NULL,       NULL },
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
            //if( GetEolStatus() == TRUE )
            //{
            //    // Get Eol Test Mode Event Handler
            //    fun = SysEventList[ i ].pFun_EOL; 
            //}
            //else if( GetFctStatus() == TRUE )
            //{
            //    // Get Eol Test Mode Event Handler
            //    fun = SysEventList[ i ].pFun_FCT; 
            //}
            //else
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

static void Evt_1ms_Handler( void )
{
    StartTimer( TIMER_ID_1MS, 1);


}


static void Evt_10ms_Handler( void )
{
    StartTimer( TIMER_ID_10MS, 10);

    ProcessToF();
}


static void Evt_100ms_Handler(void)
{
    StartTimer( TIMER_ID_100MS, 100);
    
}


U8 the_range_id  = 0;
U32 dbg_delay = 10;
U8 dbg_start = 1;
static void Evt_tof_Handler( void )
{
    if( dbg_start )
    {
        // 1. start
        StartRange( the_range_id++ );

        if( the_range_id >= MAX_TOF_ID )
        {
            the_range_id = 0;
        }
    }

    StartTimer( TIMER_ID_TOF, dbg_delay );
}


void TimerIsrCallback(void)
{

}

void InitSystem(void)
{
    /* IO */
    HAL_InitComm();
  
    RegisterTimerISR( TimerIsrCallback  );
}

