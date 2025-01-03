#include "err_room_level.h"
#include "level.h"

// ?? ???μ΅? μΉ΄μ΄ν?
U8 gu8ErrCountLimit = DEFAULT_ROOM_ERR_COUNT;
U8 gu8ErrCountLimitHighAndComplex = DEFAULT_ROOM_ERR_COUNT_HIGH_AND_COMPLEX;


void SetRoomErrCountLimit(U8 mu8Count, U8 mu8CountOverAndComplex)
{
    gu8ErrCountLimit = mu8Count;
    gu8ErrCountLimitHighAndComplex = mu8CountOverAndComplex;
}


// λ¬ΌλμΉ????
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
        return FALSE; // ?΄μ 
    }
    return mu8Error;
}


// λ³΅ν??Όμ ???
U8 CheckErrRoomComplex(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_COMPLEX );
    if( mu8Count >= gu8ErrCountLimitHighAndComplex )
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
        return FALSE; // ?΄μ 
    }
    return mu8Error;
}

// λ§?? ???
U8 CheckErrRoomHigh(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_HIGH );
    if( mu8Count >= gu8ErrCountLimitHighAndComplex )
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
        return FALSE; // ?΄μ  
    }
    return mu8Error;
}

// ??? ?Όμ 
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
        return FALSE; // ????΄μ 
    }
    return mu8Error;
}
