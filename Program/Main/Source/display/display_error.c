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



/* �ü� TH ���� */
static void ErrTempCold(U8 mu8Cmd)
{
    /* �ü� ��,�� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );
}


/* �¼� TH ���� */
static void ErrTempHot(U8 mu8Cmd)
{
    /* �¼� ��,�� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, OFF );
    CommonBitOnOff( HOT_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( HOT_WATER_STRONG, mu8Cmd );
}


/* ���� X, ���� O  */
static void ErrRoomLow(U8 mu8Cmd)
{
    /* �ü� �� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, mu8Cmd );
    CommonBitOnOff( COLD_WATER_STRONG, OFF );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );

}

/* �����÷ο� O */
static void ErrFeedValve(U8 mu8Cmd)
{
    /* �ü� ��, �¼� �� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, mu8Cmd );

}

/* ������ X, �����÷ο� O */
static void ErrRoomHigh(U8 mu8Cmd)
{
    /* �ü� �� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );

}

/* ������ X, ������ X, �����÷ο� O */
static void ErrRoomComplex(U8 mu8Cmd)
{
    /* �ü� �� ���� */
    CommonBitOnOff( COLD_WATER_WEAK, OFF );
    CommonBitOnOff( COLD_WATER_STRONG, mu8Cmd );
    CommonBitOnOff( HOT_WATER_WEAK, OFF );
    CommonBitOnOff( HOT_WATER_STRONG, OFF );
}

