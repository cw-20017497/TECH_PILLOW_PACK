#ifndef __ROOM_WATER_H__
#define __ROOM_WATER_H__

#include "prj_type.h"
#include "level.h"

typedef struct _room_water_
{
    U8 InitFull;
    U8 Level;
    
    U16 FeedTime;
} RoomWater_T;

void  InitRoomWater(void);
void  GetRoomWaterData(RoomWater_T *pData);

void  SetRoomWaterLevel(U8 mu8Val );
U8    GetRoomWaterLevel(void);

/* 정수 탱크 최초 만수위 */
void  SetRoomWaterInitFull(U8 mu8Val );

/* 정수 탱크 최초 만수위 상태 확인
 * Return :
 *  TRUE : 최초 만수위 조건 O
 *  FALSE : 최초 만수위 조건 X
 */
U8    GetRoomWaterInitFull(void);

//  정수  입수 지연 시간
void  SetRoomWaterFeedTime(U16 mu16Time );
U16   GetRoomWaterFeedTime(void);

void  ControlRoomWaterLevel(void);

#endif /* __ROOM_WATER_H__ */
