#include "room_water.h"
#include "level.h"
#include "valve.h"
#include "error.h"


#define ROOM_FEED_TIME 9000U /* 15min (600*15=9,000)@100ms */

RoomWater_T Room;


void  InitRoomWater(void)
{
    Room.InitFull       = FALSE;
    Room.Level          = LEVEL_HIGH;
    Room.FeedTime       = ROOM_FEED_TIME;
}

void  GetRoomWaterData(RoomWater_T *pData)
{
    memcpy( pData, &Room, sizeof( RoomWater_T ));
}

void  SetRoomWaterLevel(U8 mu8Val )
{
    Room.Level = mu8Val;
}

U8    GetRoomWaterLevel(void)
{
    return Room.Level;
}

void  SetRoomWaterInitFull(U8 mu8Val )
{
    Room.InitFull = mu8Val;
}

U8    GetRoomWaterInitFull(void)
{
    return Room.InitFull;
}

static U8  IsErrorRoomWaterLevel(void)
{
    if( IsErrorTypeNosFeed() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

//  정수  입수 지연 시간
void  SetRoomWaterFeedTime(U16 mu16Time )
{
    Room.FeedTime = mu16Time;
}

U16   GetRoomWaterFeedTime(void)
{
    return Room.FeedTime;
}

static void CloseNosFeedValve(void)
{
    CloseValve( VALVE_FEED );
    CloseValve( VALVE_NOS );
    CloseValve( VALVE_AIR );
}

/* 정수 탱크 수위 관리 */ 
void  ControlRoomWaterLevel(void)
{
    /* Update Watere Level */
    Room.Level = GetTankLevel( LEVEL_ID_ROOM );


    if( IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_HIGH ) == TRUE 
            || IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_OVF ) == TRUE )
    {
        SetRoomWaterInitFull( TRUE );
        Room.FeedTime = ROOM_FEED_TIME;
    }


    /* ERROR */
    if( IsErrorRoomWaterLevel() == TRUE )
    {
        CloseNosFeedValve();
        return ;
    }

    /* OVERFLOW - YES */
    if( (Room.Level & LEVEL_DETECT_OVF) == LEVEL_DETECT_OVF )
    {
        CloseNosFeedValve();
        return ;
    }

    /* OVERFLOW - NO */
    OpenValve( VALVE_NOS );


    /* LEVEL HIGH */
    if( (Room.Level & LEVEL_DETECT_HIGH ) == LEVEL_DETECT_HIGH )
    {
        CloseValve( VALVE_FEED );
        CloseValve( VALVE_AIR );
        return ;
    }

    // 최초 만수위가 아니면 Feed Time을 0으로 초기화 시킨다.
    // 중수위 조건에서 Feed Time으로 인해 재정수가 안되는 문제가 있을 수 있기 때문이다.
    if( Room.InitFull == FALSE )
    {
        Room.FeedTime = 0;
    }

    if( Room.Level == LEVEL_LOW )
    {
        // 저수위, 즉시 재정수
        OpenValve( VALVE_FEED );
        OpenValve( VALVE_AIR );
    }
    else if( Room.Level == LEVEL_MID 
            && Room.FeedTime == 0 )
    {
        // 중수위, 재정수 시간 초과 후 재정수...
        OpenValve( VALVE_FEED );
        OpenValve( VALVE_AIR );
    }

    /* CHECK FEED TIME DELAY 
     *  -. 정수, 온수, 냉수 탱크 입수 밸브의 열린 시간의 합이 일정 시간 이상인 경우,
     */
    if( Room.Level == LEVEL_LOW  
            || Room.Level == LEVEL_MID  )
    {
        if( Room.FeedTime != 0 )
        {
            Room.FeedTime--;
        }
    }
}

