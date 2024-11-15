#include "time_short.h"

#include "comp.h"
#include "level.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "err_room_level.h"
#include "timer.h"
#include "temp.h"
#include "process_display.h"


/* 1. ¾ĞÃà±â ´ë±â ½Ã°£ 0sec
 * 2. ¼öÀ§ ¼¾¼­ °¨Áö ½Ã°£ 1sec
 * 3. Á¶µµ °¨Áö Áö¿¬ ½Ã°£ 1sec
 * 4. Å° ¹öÆ° ¼³Á¤ ½Ã°£ 1sec
 * 5. ¿¬¼Ó ÃßÃâ ½Ã°£ 10min
 * 6. ¹Ì»ç¿ë ÀıÀü ½Ã°£ 60sec
 * 7. ¾ÆÀÌ½º µµ¾î °­Á¦ close ÁÖ±â 60sec
 * 8. ¼­ºñ½º Á¡°Ë ½Ã°£ 60sec
 */

#define  DEFAULT_LIMIT_TIME_VALUE      1800U    /* 30min, @1sec */
#define  DEFAULT_TEST_TIME_VALUE       1800U    /* 60sec x 30min = 1800, @1sec */


typedef struct _time_short_
{
    U8 Status;          /* START or STOP */
    U16 LimitTimer;     /* Enter FCT Mode Timeout */
    U16 TestTimer;      /* Interval Full Test Timer */
} TimeShort_T;

TimeShort_T ts;


void InitTimeShort(void)
{
    ts.Status       = FALSE; 
    ts.LimitTimer   = DEFAULT_LIMIT_TIME_VALUE;
    ts.TestTimer    = DEFAULT_TEST_TIME_VALUE;
}

void StartTimeShort(void)
{
    ts.Status = TRUE;
}

void StopTimeShort(void)
{
    ts.Status = FALSE;
}

U8 IsExpiredTimeShortLimitTimer(void)
{
    if( ts.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

U8 GetTimeShortStatus(void)
{
    return ts.Status;
}

void SetTimeShortTestTimer(U16 mu16Time)
{
    ts.TestTimer = mu16Time;
}

U16 GetTimeShortTestTimer(void)
{
    return ts.TestTimer;
}


void UpdateTimeShortTimer(void)
{
    if( ts.LimitTimer != 0 )
    {
        ts.LimitTimer--;
    }
    if( ts.TestTimer != 0 )
    {
        ts.TestTimer--;
    }
}

static void Start(void)
{
    //1. ¼öÀ§ ¼¾¼­ °¨Áö ½Ã°£ 1sec
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      1 );   // @100ms

    //2. Á¤¼ö ¼öÀ§ ¼¾¼­ ¿¡·¯ Ä«¿îÆ® ÃÖ´ë°ª ¼³Á¤
    SetRoomErrCountLimit( 1, 1 );

    //3. ¾ĞÃà±â ´ë±â ½Ã°£ 5sec
    SetCompProtectOffTime( 5 );  

    //4. ³Ã¼ö ÃÖÃÊ ¸¸¼öÀ§ ¼³Á¤ 
    SetColdWaterInitFull( TRUE );
    SetColdWaterConfigMake( TRUE );

    //5. ¿Â¼ö ÃÖÃÊ ¸¸¼öÀ§ ¼³Á¤
    SetHotWaterInitFull( TRUE );
    SetHotWaterConfigMake( TRUE );
}

static void Running(void)
{
    // ??i¢Ò?e¢¬?e¥ì¡şi¢Ó ?©«e¡Æ? i¢¥?e¢¬¡Æi¢â¡±
    if( GetCompProtectOffTime() > 5 )
    {
        SetCompProtectOffTime( 5 );
    }

//    // ¿¬¼Ó ÃßÃâ
//    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
//    {
//        //SetWaterOutFixedAmoutTargetTime( 60000UL );   // @10ms, 10min 
//    }
//
//    // e?¢¬i?¡şi?????? ?©«e¡Æ?
//    if( GetHotWaterUnusedOnTime() > 60 )
//    {
//        SetHotWaterUnusedOnTime( 60 );
//    }

//    // ³Ã¼ö ¼öÀ§ Áö¿¬ ½Ã°£
//    if( GetColdWaterFeedTime() > 1 )
//    {
//        SetColdWaterFeedTime( 1 );
//    }

    // Á¤¼ö ¼öÀ§ Áö¿¬ ½Ã°£
    if( GetRoomWaterFeedTime() > 1 )
    {
        SetRoomWaterFeedTime( 1 );
    }

    if( GetRoomWaterLevel() == LEVEL_LOW )
    {
        SetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_COMPLEX, 0 );
    }

}

static void Stop(void)
{
    // ¸Ş¸àÅä ±â´É ÇØÁ¦ 
    //SetMementoDisp( FALSE );

    // ¼öÀ§ ¼¾¼­ °¨Áö ½Ã°£
    InitTankConfDetectTime();

    // ???? ???? ?¨ùi?©« ???i©ö¢¥i?¢¥i??i¥ì©«?¢æe¡Æ¡¯ ?¢´i??
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT, DEFAULT_ROOM_ERR_COUNT_HIGH_AND_COMPLEX );
}

void ProcessTimeShort(void)
{
    static U8 mu8InitStart = FALSE;
    static U8 mu8InitStop = TRUE;


    // Start, 1¹ø¸¸ ½ÇÇà
    if( mu8InitStart == FALSE && ts.Status == TRUE )
    {
        mu8InitStart = TRUE;
        mu8InitStop = FALSE;
        Start();
        //SetVersionDisp( 30 );
    }

    // Stop, 1¹ø¸¸ ½ÇÇà
    if( mu8InitStop == FALSE && ts.Status == FALSE )
    {
        mu8InitStop = TRUE;
        Stop();
    }

    // Å×½ºÆ® ¸ğµå°¡ Á¾·á
    if( ts.Status == FALSE )
    {
        return ;
    }

    // Å×½ºÆ® ¸ğµå¿¡¼­´Â ³Ã¼ö ¿Âµµ ¼¾¼­ »ó½Ã ÀĞ±â
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );

    // ÀüÃ¼ °Ë»ç ½Ã°£ ÃÊ°ú, Å×½ºÆ® ¸ğµå ÇØÁ¦
    if( ts.TestTimer == 0 )
    {
        ts.Status = FALSE;
    }

    Running();
}
