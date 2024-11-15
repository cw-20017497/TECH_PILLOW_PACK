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


/* 1. 압축기 대기 시간 0sec
 * 2. 수위 센서 감지 시간 1sec
 * 3. 조도 감지 지연 시간 1sec
 * 4. 키 버튼 설정 시간 1sec
 * 5. 연속 추출 시간 10min
 * 6. 미사용 절전 시간 60sec
 * 7. 아이스 도어 강제 close 주기 60sec
 * 8. 서비스 점검 시간 60sec
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
    //1. 수위 센서 감지 시간 1sec
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     1 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      1 );   // @100ms

    //2. 정수 수위 센서 에러 카운트 최대값 설정
    SetRoomErrCountLimit( 1, 1 );

    //3. 압축기 대기 시간 5sec
    SetCompProtectOffTime( 5 );  

    //4. 냉수 최초 만수위 설정 
    SetColdWaterInitFull( TRUE );
    SetColdWaterConfigMake( TRUE );

    //5. 온수 최초 만수위 설정
    SetHotWaterInitFull( TRUE );
    SetHotWaterConfigMake( TRUE );
}

static void Running(void)
{
    // ??i¶?e¸?eμ￢i†� ?œe°? i´?e¸°i™”
    if( GetCompProtectOffTime() > 5 )
    {
        SetCompProtectOffTime( 5 );
    }

//    // 연속 추출
//    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
//    {
//        //SetWaterOutFixedAmoutTargetTime( 60000UL );   // @10ms, 10min 
//    }
//
//    // e?¸i?￢i?????? ?œe°?
//    if( GetHotWaterUnusedOnTime() > 60 )
//    {
//        SetHotWaterUnusedOnTime( 60 );
//    }

//    // 냉수 수위 지연 시간
//    if( GetColdWaterFeedTime() > 1 )
//    {
//        SetColdWaterFeedTime( 1 );
//    }

    // 정수 수위 지연 시간
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
    // 메멘토 기능 해제 
    //SetMementoDisp( FALSE );

    // 수위 센서 감지 시간
    InitTankConfDetectTime();

    // ???? ???? ?¼i?œ ?�??i¹´i?´i??iμœ?€e°’ ?¤i??
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT, DEFAULT_ROOM_ERR_COUNT_HIGH_AND_COMPLEX );
}

void ProcessTimeShort(void)
{
    static U8 mu8InitStart = FALSE;
    static U8 mu8InitStop = TRUE;


    // Start, 1번만 실행
    if( mu8InitStart == FALSE && ts.Status == TRUE )
    {
        mu8InitStart = TRUE;
        mu8InitStop = FALSE;
        Start();
        //SetVersionDisp( 30 );
    }

    // Stop, 1번만 실행
    if( mu8InitStop == FALSE && ts.Status == FALSE )
    {
        mu8InitStop = TRUE;
        Stop();
    }

    // 테스트 모드가 종료
    if( ts.Status == FALSE )
    {
        return ;
    }

    // 테스트 모드에서는 냉수 온도 센서 상시 읽기
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );

    // 전체 검사 시간 초과, 테스트 모드 해제
    if( ts.TestTimer == 0 )
    {
        ts.Status = FALSE;
    }

    Running();
}
