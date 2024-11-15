#include "err_room_level.h"
#include "level.h"

// 정수 에러 최대 카운터
U8 gu8ErrCountLimit = DEFAULT_ROOM_ERR_COUNT;

void SetRoomErrCountLimit(U8 mu8Count)
{
    gu8ErrCountLimit = mu8Count;
}

U8 GetRoomErrCountLimit(void)
{
    return gu8ErrCountLimit;
}


// 물넘침 에러
U8 CheckErrRoomOvf(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_OVF );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 ReleaseErrRoomOvf(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_OVF );
    if( mu8Count == 0 )
    {
        return FALSE; // 해제
    }
    return mu8Error;
}


// 복합 센서 에러
U8 CheckErrRoomComplex(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_COMPLEX );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 ReleaseErrRoomComplex(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_COMPLEX );
    if( mu8Count == 0 )
    {
        return FALSE; // 해제
    }
    return mu8Error;
}

// 만수위 에러
U8 CheckErrRoomHigh(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_HIGH );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 ReleaseErrRoomHigh(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_HIGH );
    if( mu8Count == 0 )
    {
        return FALSE; // 해제 
    }
    return mu8Error;
}

// 저수위 센서 
U8 CheckErrRoomLow(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_LOW );
    if( mu8Count >= 1 )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 ReleaseErrRoomLow(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_LOW );
    if( mu8Count == 0 )
    {
        return FALSE; // 에러 해제
    }
    return mu8Error;
}
