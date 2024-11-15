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


/* 1. ����� ��� �ð� 0sec
 * 2. ���� ���� ���� �ð� 1sec
 * 3. ���� ���� ���� �ð� 1sec
 * 4. Ű ��ư ���� �ð� 1sec
 * 5. ���� ���� �ð� 10min
 * 6. �̻�� ���� �ð� 60sec
 * 7. ���̽� ���� ���� close �ֱ� 60sec
 * 8. ���� ���� �ð� 60sec
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
    //1. ���� ���� ���� �ð� 1sec
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      1 );   // @100ms

    //2. ���� ���� ���� ���� ī��Ʈ �ִ밪 ����
    SetRoomErrCountLimit( 1, 1 );

    //3. ����� ��� �ð� 5sec
    SetCompProtectOffTime( 5 );  

    //4. �ü� ���� ������ ���� 
    SetColdWaterInitFull( TRUE );
    SetColdWaterConfigMake( TRUE );

    //5. �¼� ���� ������ ����
    SetHotWaterInitFull( TRUE );
    SetHotWaterConfigMake( TRUE );
}

static void Running(void)
{
    // ??i��?e��?e���i�Ӎ ?��e��? i��?e����i�⡱
    if( GetCompProtectOffTime() > 5 )
    {
        SetCompProtectOffTime( 5 );
    }

//    // ���� ����
//    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
//    {
//        //SetWaterOutFixedAmoutTargetTime( 60000UL );   // @10ms, 10min 
//    }
//
//    // e?��i?��i?????? ?��e��?
//    if( GetHotWaterUnusedOnTime() > 60 )
//    {
//        SetHotWaterUnusedOnTime( 60 );
//    }

//    // �ü� ���� ���� �ð�
//    if( GetColdWaterFeedTime() > 1 )
//    {
//        SetColdWaterFeedTime( 1 );
//    }

    // ���� ���� ���� �ð�
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
    // �޸��� ��� ���� 
    //SetMementoDisp( FALSE );

    // ���� ���� ���� �ð�
    InitTankConfDetectTime();

    // ???? ???? ?��i?�� ?�??i����i?��i??i�쩫?��e�ơ� ?��i??
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT, DEFAULT_ROOM_ERR_COUNT_HIGH_AND_COMPLEX );
}

void ProcessTimeShort(void)
{
    static U8 mu8InitStart = FALSE;
    static U8 mu8InitStop = TRUE;


    // Start, 1���� ����
    if( mu8InitStart == FALSE && ts.Status == TRUE )
    {
        mu8InitStart = TRUE;
        mu8InitStop = FALSE;
        Start();
        //SetVersionDisp( 30 );
    }

    // Stop, 1���� ����
    if( mu8InitStop == FALSE && ts.Status == FALSE )
    {
        mu8InitStop = TRUE;
        Stop();
    }

    // �׽�Ʈ ��尡 ����
    if( ts.Status == FALSE )
    {
        return ;
    }

    // �׽�Ʈ ��忡���� �ü� �µ� ���� ��� �б�
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );

    // ��ü �˻� �ð� �ʰ�, �׽�Ʈ ��� ����
    if( ts.TestTimer == 0 )
    {
        ts.Status = FALSE;
    }

    Running();
}
