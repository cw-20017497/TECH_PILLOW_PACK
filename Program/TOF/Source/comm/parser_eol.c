/***********************************************************************************************
 * INCLUDE 
 */

#include "hw.h"
#include "parser_eol.h"
#include "util.h"
#include "eol.h"
#include "process_eol.h"
#include "timer.h"

#include "temp.h"
#include "err_temp.h"
#include "level.h"
#include "relay.h"
#include "valve.h"
#include "room_water.h"
#include "cold_water.h"
#include "comp.h"

#include "hal_adc.h"
#include "hal_fan_motor.h"
#include "power_saving.h"
#include "error.h"
#include "process_display.h"

#include <stdio.h>
#include <stdlib.h>

/***********************************************************************************************
 * DEFINITION 
 */
#define  PKT_STX     0x01
#define  PKT_ETX     0x04

#define  PKT_ACK     0x06
#define  PKT_NAK     0x15

#define	MIN_PKT_SZ  6	/* STX + TYPE + ETX + CRC(2byes) */


#define MK_DATA( _buf, _len, _s, _val ) \
    do{\
    _len += sprintf( (char *)&_buf[ _len ], _s, _val );\
    }while(0)


static I16 MkTemp(TEMP_T mTemp, U8 *pBuf, I16 mi16Len)
{
    U16 mu16Temp;

    mu16Temp = (U16)( mTemp * 10.0f );

    pBuf[ mi16Len++ ] = GET_HIGH_BYTE( mu16Temp );
    pBuf[ mi16Len++ ] = GET_LOW_BYTE( mu16Temp );

    return mi16Len;
}

static U8	check_crc( U8 *buf, I16 len )
{
    U8 i = 0;
    U8 mu8Chksum = 0;

    for( i = 0; i < len - 3 ; i++ )
    {
        mu8Chksum ^= buf[ i ];
    }

    return mu8Chksum;
}

static U8 IsTrueChksum( U8 *buf, I16 len )
{
    U8 mu8Cal;
    U8 mu8Val;


    mu8Cal = check_crc( buf, len );
    mu8Val = ConvAsc2Byte( buf[ len - 3 ], buf[ len - 2 ] );
    if( mu8Cal != mu8Val )
    {
        return FALSE;
    }

    return TRUE;
}

// 수신 받은 패킷의 유효성 검사
I16 IsValidPkt_EOL( U8 *buf, I16 len )
{
    U8 mu8Chksum = 0;

    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( buf[0] != PKT_STX 
            || buf[ len - 1] != PKT_ETX )
    {
        return FALSE;
    }

    if( IsTrueChksum( buf, len ) == FALSE )
    {
        return FALSE;
    }

    // 검사 모드 진입 후
    // 또는 검사 종료 후에는 통신 메시지를 수신 받지 않는다.
    if( GetEolMode() == EOL_MODE_DONE )
    {
        return FALSE;
    }

    if( GetEolStatus() == FALSE )
    {
        // Check expired timer... exit..
        if( IsExpiredEolLimitTimer() == TRUE )
        {
            return FALSE;
        }


        // EOL 시작
        StartEol( EOL_TYPE_LOAD );
        StartDisplayInit();
        SetVersionDisp( 50 );
    }

    return TRUE;
}


typedef	void (*ActionParser_t)(void);
typedef struct _parser_list_t
{
    U8 Type;
    ActionParser_t ParserPkt;
} parser_list_t;

//static void ParserStart(void);
static void ParserSensor(void);
static void ParserNoLoad(void);
static void ParserFanMotor(void);
static void ParserFeedValve(void);
static void ParserNosValve(void);
static void ParserHeater(void);
static void ParserGasFeedValve(void);
static void ParserGasNosValveClose(void);
static void ParserGasNosValveOpen(void);
static void ParserComp(void);
static void ParserComplete(void);

const static parser_list_t parser_list[] = 
{
    { PKT_EOL_MODEL,                 NULL },
    { PKT_EOL_MODE,                  NULL },

    { PKT_EOL_EEPROM,                ParserSensor },
    { PKT_EOL_SENSOR_TEMP,           NULL },
    { PKT_EOL_SENSOR_LEVEL,          NULL },
      
    { PKT_EOL_HEATER,                ParserHeater        },
    { PKT_EOL_NO_LOAD,               ParserNoLoad           },

    { PKT_EOL_FEED_VALVE,            ParserFeedValve        },
    { PKT_EOL_NOS_VALVE,             ParserNosValve         },

    { PKT_EOL_GAS_FEED_VALVE,        ParserGasFeedValve     },
    { PKT_EOL_GAS_NOS_VALVE_CLOSE,   ParserGasNosValveClose },
    { PKT_EOL_GAS_NOS_VALVE_OPEN,    ParserGasNosValveOpen  },

    { PKT_EOL_COMP,                  ParserComp             },
    { PKT_EOL_COMPLETE,              ParserComplete         }
                                     
    //{ PKT_EOL_FAN_MOTOR,             ParserFanMotor         },
};
#define SZ_PARSER_TABLE (sizeof( parser_list ) / sizeof( parser_list_t ))

// 수신 받은 패킷 파싱
I16 ParserPkt_EOL( U8 *buf, I16 len)
{
    U8 mu8Index = 0;
    U8 mu8PktType = 0;
    ActionParser_t	pParser = NULL;


    // Get Packet
    mu8PktType = ConvAsc2Byte( buf[1], buf[2] );
    for( mu8Index = 0; mu8Index < SZ_PARSER_TABLE; mu8Index++ )
    {
        if( mu8PktType == parser_list[ mu8Index ].Type )
        {
            pParser = parser_list[ mu8Index ].ParserPkt;
            if( pParser != NULL )
            {
                pParser();
            }

            // ACK 
            SetCommHeader( COMM_ID_EOL, mu8PktType );
            StartTimer( TIMER_ID_COMM_EOL_TX, 0 );
            break;
        }
    }

    return 0;
}

static void TurnOffAllLoad(void)
{
    // Turn off Valves
    CloseValve( VALVE_ALL );
    OpenValve( VALVE_NOS );

    // Turn off Relay
    TurnOffRelayAll();

    // Turn off Dc FAN
    HAL_TurnOffFanMotor();
}

//static void ParserStart(void)
//{
//    // Check expired timer... exit..
//    if( IsExpiredEolLimitTimer() == TRUE )
//    {
//        return ;
//    }
//
//    if( GetEolStatus() == FALSE )
//    {
//        // EOL 시작
//        StartEol( EOL_TYPE_LOAD );
//        StartDisplayInit();
//        SetVersionDisp( 50 );
//    }
//}

static void ParserSensor(void)
{
    SetEolMode( EOL_MODE_CHECK_SENSOR );
}

static void ParserNoLoad(void)
{
    SetEolMode( EOL_MODE_CHECK_LOAD );

    TurnOffAllLoad();
}


static void ParserFanMotor(void)
{
    TurnOffAllLoad();

    HAL_TurnOnFanMotor();
}


static void ParserFeedValve(void)
{
    TurnOffAllLoad();

    OpenValve( VALVE_FEED );
}

static void ParserNosValve(void)
{
    TurnOffAllLoad();

    CloseValve( VALVE_NOS );
}


static void ParserHeater(void)
{
    static U8 mu8ProtectHeaterFlag = 0; 

    TurnOffAllLoad();

    if( mu8ProtectHeaterFlag == 0 )
    {
        mu8ProtectHeaterFlag = 1;
        TurnOnRelay( RELAY_HEATER );
    }

}

static void ParserGasFeedValve(void)
{
    OpenValve( VALVE_FEED );
    OpenValve( VALVE_NOS );
}

static void ParserGasNosValveClose(void)
{
    CloseValve( VALVE_NOS );
}

static void ParserGasNosValveOpen(void)
{
    OpenValve( VALVE_NOS );
}


static void ParserComp(void)
{
    TurnOffAllLoad();

    // TURN ON COMP & DC FAN
    TurnOnRelay( RELAY_COMP );
    HAL_TurnOnFanMotor();

    StartEolIceSystem();
}


static void ParserComplete(void)
{
    SetEolMode( EOL_MODE_DONE );

    TurnOffAllLoad();
}


/* Make checksum */
I16	Crc16_EOL( U8 *buf, I16 len )
{
    U8 i = 0;
    U8 mu8Chksum = 0;
    U8 crc_buf[5];
    

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu8Chksum = check_crc( buf, len);
    sprintf( (char *)crc_buf, "%02X", mu8Chksum );

    buf[ len - 3 ] = crc_buf[0];
    buf[ len - 2 ] = crc_buf[1];

    return len;
}

typedef	I16	(*Action_t)( U8 *buf, U8 mu8PktType );
typedef struct _make_list_t
{
    U8 Type;
    Action_t MakePkt;
} make_list_t;
static I16	MakePkt_Model( U8 *buf, U8 mu8PktType );
static I16	MakePkt_Eeprom( U8 *buf, U8 mu8PktType );
static I16	MakePkt_SensorTemp( U8 *buf, U8 mu8PktType );
static I16	MakePkt_SensorLevel( U8 *buf, U8 mu8PktType );
static I16	MakePkt_Comp( U8 *buf, U8 mu8PktType );
static I16	MakePkt_NoPayLoad( U8 *buf, U8 mu8PktType );

const static make_list_t make_list[] = 
{
    { PKT_EOL_MODEL,                 MakePkt_Model },
    { PKT_EOL_MODE,                  MakePkt_NoPayLoad },

    { PKT_EOL_EEPROM,                MakePkt_Eeprom },
    { PKT_EOL_SENSOR_TEMP,           MakePkt_SensorTemp },
    { PKT_EOL_SENSOR_LEVEL,          MakePkt_SensorLevel },

    { PKT_EOL_HEATER,                MakePkt_NoPayLoad },
    { PKT_EOL_NO_LOAD,               MakePkt_NoPayLoad },

    { PKT_EOL_FEED_VALVE,            MakePkt_NoPayLoad },
    { PKT_EOL_NOS_VALVE,             MakePkt_NoPayLoad },

    { PKT_EOL_GAS_FEED_VALVE,        MakePkt_NoPayLoad },
    { PKT_EOL_GAS_NOS_VALVE_CLOSE,   MakePkt_NoPayLoad },
    { PKT_EOL_GAS_NOS_VALVE_OPEN,    MakePkt_NoPayLoad },

    { PKT_EOL_COMP,                  MakePkt_Comp },
    { PKT_EOL_COMPLETE,              MakePkt_NoPayLoad }

    //{ PKT_EOL_FAN_MOTOR,             MakePkt_NoPayLoad },
};

#define	SZ_MAKE_TABLE	( sizeof( make_list ) / sizeof( make_list_t ))

I16	MakePkt_EOL( CommHeader_T *pComm, U8 *buf )
{
    U8 mu8Type;
    Action_t	p_make_pkt;
    U8 i;
    I16	len	= -1;


    for( i = 0; i < SZ_MAKE_TABLE; i++ )
    {
        mu8Type = make_list[ i ].Type;
        p_make_pkt = make_list[ i ].MakePkt;

        if( mu8Type == (U8)pComm )
        {
            if( p_make_pkt != NULL )
            {
                len = p_make_pkt( buf, mu8Type );
            }
            break;
        }
    }

    return len;
}


static I16	MakePkt_Model( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", MODEL_CODE );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_Eeprom( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GetEolCheckStatus( EOL_CHK_ID_EEPROM ) );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", 0x03 );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

#define TEMP_ERROR      0xFF        // open, short
#define TEMP_UNDER      0x00        // 영하
static U8 ConvEolTemp(U8 mu8AdcId, TEMP_T mTemp)
{
    U8 mu8Ret = TEMP_ERROR;
    //TEMP_T mTemp = 0.0f;
    U16 mu16Adc = 0;
        

    //mTemp = GetTemp( mu8TempId );
    mu16Adc = HAL_GetAdcValue( mu8AdcId );
    if( GetTempErrorType( mu16Adc ) != STATUS_NORMAL )
    {
        mu8Ret = TEMP_ERROR;
    }
    else
    {
        if( mTemp > 0.0f )
        {
            mu8Ret = (U8)mTemp;
        }
        else
        {
            mu8Ret = TEMP_UNDER;
        }
    }

    return mu8Ret;
}

static I16	MakePkt_SensorTemp( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;
    U16 mu16Adc;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );

#if 0
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", ConvEolTemp( ANI_TEMP_COLD_WATER,  GetTemp( TEMP_ID_COLD_WATER) ));
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", ConvEolTemp( ANI_TEMP_HOT_WATER,   GetTemp( TEMP_ID_HOT_WATER) ));
#else
    mu16Adc = HAL_GetAdcValue( ANI_TEMP_COLD_WATER );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_HIGH_BYTE( mu16Adc ) );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_LOW_BYTE( mu16Adc ));

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_HOT_WATER );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_HIGH_BYTE( mu16Adc ) );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_LOW_BYTE( mu16Adc ));
#endif

    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_SensorLevel( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;


    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_ROOM_TANK_LOW() );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_ROOM_TANK_HIGH() );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_ROOM_TANK_OVERFLOW() );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}

static I16	MakePkt_Comp( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;
    U16 mu16Adc;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );

    mu16Adc = HAL_GetAdcValue( ANI_TEMP_COLD_WATER );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_HIGH_BYTE( mu16Adc ) );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", GET_LOW_BYTE( mu16Adc ));

    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}


static I16	MakePkt_NoPayLoad( U8 *buf, U8 mu8PktType )
{
    I16 mi16Len = 0;

    buf[ mi16Len++ ] = PKT_STX;
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", PKT_ACK );
    mi16Len += sprintf( (char *)&buf[ mi16Len ], "%02X", mu8PktType );
    buf[ mi16Len++ ] = 0;     // CRC - UPPER
    buf[ mi16Len++ ] = 0;     // CRC - LOWER
    buf[ mi16Len++ ] = PKT_ETX;

    return mi16Len;
}


