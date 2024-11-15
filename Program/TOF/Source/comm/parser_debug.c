/***********************************************************************************************
 * INCLUDE 
 */
#include "hw.h"
#include "level.h"
#include "relay.h"
#include "valve.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "comp.h"
#include "temp.h"
#include "hal_adc.h"
#include "power_saving.h"
#include "error.h"
#include "feed_pump.h"
#include "pump.h"

#include "parser_debug.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

/***********************************************************************************************
 * DEFINITION 
 */
#define MIN_PKT_SZ      6           /* HEADER + TAIL */


static U8   check_crc( U8 *buf, U8 len )
{
#if 1
    U16 crc16 = 0;

    crc16 = ( ( (U16)buf[ len - 1 ] ) << 8 ) & 0xFF00;
    crc16 |=    ( buf[ len - 2 ] );

    if( crc16 != crc16_cal( buf, (U8)( len - 2 ) ) )
    {
        return FALSE;
    }

#endif
    return TRUE;
}

I16 IsValidPkt_Debug( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

#if 1
    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }
    if( check_crc( buf, len ) == FALSE )
    {
        return FALSE;
    }
#endif

    return TRUE;
}


typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    action_t    parser_pkt;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { NULL },
};

#define SZ_PS_TABLE ( sizeof( parser_list ) / sizeof( parser_list_t ))

I16 ParserPkt_Debug( U8 *buf, I16 len)
{
    // COMP PROTECT
    if( buf[0] == '1' )
    {
        SetCompProtectOffTime(1);
    }
    // SLEEP 
    else if( buf[0] == '7' )
    {
        SetSavingConfTimerSleep(1);
        SetSavingConfTimerWakeUp(1);
    }
    else if( buf[0] == '2' )
    {
        SetColdWaterExtraMakeTime( 1 );
    }
    else if( buf[0] == '3' )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
    else if( buf[0] == '4' )
    {
        SetPumpOnCurrentTime( 100 );
    }
    else if( buf[0] == '5' )
    {
        SetRoomWaterFeedTime( 1 );
    }

//    else if( buf[0] == '8' )
//    {
//        SetHotWaterUnusedOnTime( 0 );
//    }
//    else if( buf[0] == 'C' )
//    {
//        if( buf[1] == '1' && buf[2] == '1' )
//        {
//            // 외기 온도 오버라이드 설정
//            SetOverrideTemp( TEMP_ID_AMBIENT, TRUE, 30.0f );
//        }
//        else if( buf[1] == '1' && buf[2] == '0' )
//        {
//            // 외기 온도 오버라이드 해제
//            SetOverrideTemp( TEMP_ID_AMBIENT, FALSE, 30.0f );
//        }
//
//
//#if 0
//        SetOverrideTemp( TEMP_ID_AMBIENT, buf[2], mTemp );
//        SetOverrideTemp( TEMP_ID_ROOM_WATER, buf[3], mTemp );
//        SetOverrideTemp( TEMP_ID_COLD_WATER, buf[4], mTemp );
//        SetOverrideTemp( TEMP_ID_HOT_WATER, buf[5], mTemp );
//        SetOverrideTemp( TEMP_ID_EVA_1, buf[7], mTemp );
//        SetOverrideTemp( TEMP_ID_EVA_2, buf[8], mTemp );
//#endif
//    }

    return 0;
}

I16 Crc16_Debug( U8 *buf, I16 len )
{
   return len;
}

typedef struct _make_list_t
{
    U8 Tx;
    action_t    make_pkt;
} make_list_t;

static I16 MakePktType_1( U8 *buf );
static I16 MakePktType_2( U8 *buf );
static make_list_t make_list[] = 
{
    { TRUE, MakePktType_1 },
    { TRUE, MakePktType_2 }
};

#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_Debug( CommHeader_T *p_comm, U8 *buf )
{
    static U8 mCount = 0;
    action_t    p_make_pkt;
    I16 len = -1;


    p_make_pkt = make_list[ mCount ].make_pkt;
    if( p_make_pkt != NULL )
    {
        if( make_list[ mCount ].Tx == TRUE )
        {
            len = p_make_pkt( buf );
        }
        else
        {
            mCount = 0;

            return len;
        }
    }

    mCount++;
    if( mCount >= SZ_TABLE )
    {
        mCount = 0;
    }

    return len;
}


extern U8 gu8FanOnOff;
static I16 MakePktType_1( U8 *buf )
{
    I16 len = 0;

    RoomWater_T mRoomData;
    ColdWater_T mColdData;
    HotWater_T mHotData;
    Compressor_T  mCompData;
    FeedPump_T  mFeedPump;
    Pump_T  mPump;


    /* PACKET TYPE */
    len = sprintf( (char *)&buf[ len ], "TYPE_1=");
    /* ROOM LEVEL */
    GetRoomWaterData( &mRoomData );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%d:%d:%d@", 
            GET_ROOM_TANK_LOW(), 
            GET_ROOM_TANK_HIGH(), 
            GET_ROOM_TANK_OVERFLOW(), 
            mRoomData.Level,
            mRoomData.InitFull,
            ( mRoomData.FeedTime / 10 )
            );

    /* COLD LEVEL */
    len += sprintf( (char *)&buf[ len ], "%d:%d@", 
            GetTankLevel( LEVEL_ID_COLD ), 
            GetColdWaterInitFull()
            );

    /* VALVE */
    len += sprintf( (char *)&buf[ len ], "%d:%d@", 
            !GET_STATUS_VALVE_NOS(),
            GET_STATUS_VALVE_FEED()
            );

    /* RELAY */
    len += sprintf( (char *)&buf[ len ], "%d:%d@", 
            IsTurnOnRelay( RELAY_COMP ),
            IsTurnOnRelay( RELAY_HEATER )
            );

    /* COLD WATER */
    GetColdWaterData( &mColdData );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%.1f:%.1f:%.1f:%d:%u@", 
            mColdData.ConfigMake,
            mColdData.Make,
            mColdData.TempTargetOn,
            mColdData.TempTargetOff,
            mColdData.TempCurrent,
            mColdData.ExtraMake,
            mColdData.ExtraMakeTime
            );

    /* HOT WATER */
    GetHotWaterData( &mHotData );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%.1f:%.1f:%.1f:%d:%d:%d:%u@", 
            mHotData.ConfigMake,
            mHotData.Make,
            mHotData.TempTargetOn,
            mHotData.TempTargetOff,
            mHotData.TempCurrent,
            mHotData.Level,
            mHotData.Altitude,
            mHotData.InitFull,
            mHotData.InitWaitTime
            );

    /* COMP & DC FAN */
    GetCompData( &mCompData );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%lu:%lu:%d@", 
            GetCompOnOff(),          // COMP
            mCompData.ProtectOffTime,
            mCompData.OnTime,
            mCompData.OffTime,
            gu8FanOnOff
            //GET_STATUS_FAN_MOTOR_ONOFF()  
            );

    /* FEED PUMP */
    GetFeedPumpData( &mFeedPump );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%d:%d@", 
            mFeedPump.Conn,
            mFeedPump.OnOff,
            mFeedPump.OnStep,
            mFeedPump.OffStep,
            GET_PUMP_ONOFF()
            );

    /* PUMP */
    GetPumpData( &mPump );
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%lu:%lu@", 
            mPump.OnOff,
            mPump.OnStep,
            mPump.OnPumpCmd,
            (mPump.OnTargetTime / 10UL) ,
            (mPump.OnCurrentTime / 10UL)
            );

    /* END */
    len += sprintf( (char *)&buf[ len ], "\r\n" );

    return len;
}


static I16 MakePktType_2( U8 *buf )
{
    I16 len = 0;
    PowerSaving_T mSavingData;
    

    GetSavingData( &mSavingData );

    /* PACKET TYPE */
    len = sprintf( (char *)&buf[ len ], "TYPE_2=");

    /* ADC */
    len += sprintf( (char *)&buf[ len ], "%u:%u:%u:",
            HAL_GetAdcValue( ANI_TEMP_HOT_WATER ),
            HAL_GetAdcValue( ANI_TEMP_COLD_WATER ), 
            HAL_GetAdcValue( ANI_SENSOR_PHOTO )
            );

    /* ADC - TEMPERTURE */
    len += sprintf( (char *)&buf[ len ], "%.1f:%.1f@", 
            GetTemp( TEMP_ID_HOT_WATER ),
            GetTemp( TEMP_ID_COLD_WATER )
            );

    // TEMP SENSOR TIME
    len += sprintf( (char *)&buf[ len ], "%d:%d@",
            GetTempSensorTime( TEMP_ID_COLD_WATER ),
            GetTempSensorTime( TEMP_ID_HOT_WATER )
            );

    /* POWER SAVING */
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%d:",
            mSavingData.Config,
            mSavingData.Status,
            mSavingData.Dark2Light,
            mSavingData.Light2Dark
            );

    /* POWER SAVING */
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%d:%d:%d@",
            mSavingData.IsDark,
            mSavingData.PhotoAdc,
            mSavingData.AdcLimitSleep,
            mSavingData.AdcLimitWakeUp,
            mSavingData.TimerSleep,
            mSavingData.TimerWakeUp
            );

    /* ERROR */
    len += sprintf( (char *)&buf[ len ], "%d:%d:%d:%d:%d:%d@",
            IsError( ERR_TEMP_COLD_WATER ),
            IsError( ERR_TEMP_HOT_WATER ),
            IsError( ERR_ROOM_OVF ),
            IsError( ERR_ROOM_COMPLEX ),
            IsError( ERR_ROOM_LOW_LEVEL ),
            IsError( ERR_ROOM_HIGH_LEVEL )
            );


    /* END */
    len += sprintf( (char *)&buf[ len ], "\r\n" );


    return len;

}


