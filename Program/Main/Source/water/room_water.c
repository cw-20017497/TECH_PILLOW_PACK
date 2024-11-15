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

//  ����  �Լ� ���� �ð�
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

/* ���� ��ũ ���� ���� */ 
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

    // ���� �������� �ƴϸ� Feed Time�� 0���� �ʱ�ȭ ��Ų��.
    // �߼��� ���ǿ��� Feed Time���� ���� �������� �ȵǴ� ������ ���� �� �ֱ� �����̴�.
    if( Room.InitFull == FALSE )
    {
        Room.FeedTime = 0;
    }

    if( Room.Level == LEVEL_LOW )
    {
        // ������, ��� ������
        OpenValve( VALVE_FEED );
        OpenValve( VALVE_AIR );
    }
    else if( Room.Level == LEVEL_MID 
            && Room.FeedTime == 0 )
    {
        // �߼���, ������ �ð� �ʰ� �� ������...
        OpenValve( VALVE_FEED );
        OpenValve( VALVE_AIR );
    }

    /* CHECK FEED TIME DELAY 
     *  -. ����, �¼�, �ü� ��ũ �Լ� ����� ���� �ð��� ���� ���� �ð� �̻��� ���,
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

