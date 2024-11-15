#include "hw.h"
#include "hal_led.h"
#include "display.h"

#include "error.h"
#include "display_error.h"


static void ErrTempCold(U8 mu8Cmd);
static void ErrTempHot(U8 mu8Cmd);

static void ErrRoomLow(U8 mu8Cmd);
static void ErrFeedValve(U8 mu8Cmd);
static void ErrRoomHigh(U8 mu8Cmd);
static void ErrRoomComplex(U8 mu8Cmd);

typedef void(*Action_T)(U8 mu8Cmd);
typedef struct _error_display_list_
{
    ErrorId_T  Id;
    Action_T   DispAction;

} ErrDisplay_T;

ErrDisplay_T ErrDisplayList[] = 
{
    { ERR_TEMP_COLD_WATER,    ErrTempCold },
    { ERR_TEMP_HOT_WATER,     ErrTempHot },

    //{ ERR_ROOM_OVF,           ErrFeedValve },  
    //{ ERR_ROOM_COMPLEX,       ErrRoomComplex },
    //{ ERR_ROOM_HIGH_LEVEL,    ErrRoomHigh },
    { ERR_ROOM_LOW_LEVEL,     ErrRoomLow  },
};

#define  SZ_LIST  ( sizeof(ErrDisplayList) / sizeof(ErrDisplay_T ))


void DisplayError(U8 mu8ErrCode, U8 mu8Cmd )
{
    U8 i;
    Action_T pAction = NULL;


    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( ErrDisplayList[ i ].Id == mu8ErrCode )
        {
            pAction = ErrDisplayList[ i ].DispAction;
            if( pAction != NULL )
            {
                pAction( mu8Cmd );
            }
            break;
        }
    }
}



/* 냉수 TH 에러 */
static void ErrTempCold(U8 mu8Cmd)
{
    /* 냉수 강,약 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );
}


/* 온수 TH 에러 */
static void ErrTempHot(U8 mu8Cmd)
{
    /* 온수 강,약 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, OFF );
    CommonBitOnOff( HOT_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( HOT_WATER_STRONG, mu8Cmd );
}


/* 저수 X, 만수 O  */
static void ErrRoomLow(U8 mu8Cmd)
{
    /* 냉수 약 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( COLD_WATER_STRONG, OFF );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );

}

/* 오버플로우 O */
static void ErrFeedValve(U8 mu8Cmd)
{
    /* 냉수 강, 온수 강 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, mu8Cmd );

}

/* 만수위 X, 오버플로우 O */
static void ErrRoomHigh(U8 mu8Cmd)
{
    /* 냉수 강 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );

}

/* 저수위 X, 만수위 X, 오버플로우 O */
static void ErrRoomComplex(U8 mu8Cmd)
{
    /* 냉수 강 점멸 */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );
}

