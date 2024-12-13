/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_tof.h"
#include "util.h"
#include "timer.h"
#include "sound.h"

/***********************************************************************************************
 * DEFINITION 
 */

#define STX         0x11
#define ETX         0x88


#define MIN_PKT_SZ      4

static U16 Rx_CRC_CCITT(U8 *puchMsg, U16 usDataLen)
{
    U8 i = 0;
    U16 wCRCin = 0x0000;
    U16 wCPoly = 0x1021;
    U8 wChar = 0;

    while(usDataLen--)
    {
        wChar = *(puchMsg++);
        wCRCin ^= ((U16)wChar << 8);
        for(i = 0; i < 8; i++)
        {
            if (wCRCin & 0x8000)
            {
                wCRCin = (wCRCin << 1) ^ wCPoly;
            }
            else
            {
                wCRCin = wCRCin << 1;
            }
        }
    }

    return (wCRCin);
}

static U8   check_crc( U8 *buf, I16 len )
{
    U16 crc16 = 0;

    crc16 = GET_UINT_WORD( buf[len-2], buf[len-3] );
    if( crc16 != Rx_CRC_CCITT( buf, (U16)( len - 3 ) ) )
    {
        return FALSE;
    }

    return TRUE;
}

I16 IsValidPkt_TOF( U8 *buf, I16 len )
{
    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( check_crc( buf, len ) == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


static I16 ParserAck(U8 *buf);
static I16 ParserAckCali(U8 *buf);

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t Parser;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { PKT_TOF_ACK,          ParserAck },
    { PKT_TOF_ACK_CALI,     ParserAckCali },
};

#define SZ_PS_TABLE ( sizeof( parser_list ) / sizeof( parser_list_t ))

I16 ParserPkt_TOF( U8 *buf, I16 len)
{
    U8 mu8Type;
    action_t	pParser;
    U8 i;


    for( i = 0; i < SZ_PS_TABLE; i++ )
    {
        mu8Type = parser_list[ i ].Type;
        pParser = parser_list[ i ].Parser;

        if( mu8Type == buf[1] )
        {
            if( pParser != NULL )
            {
                len = pParser( &buf[2] );
            }
            break;
        }
    }

    return len;
}

I16 Crc16_TOF( U8 *buf, I16 len )
{
    U16 mu16Chksum = 0;
    

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    mu16Chksum = Rx_CRC_CCITT( buf, (U16)(len - 3));
    buf[ len - 3 ] = GET_HIGH_BYTE(mu16Chksum);
    buf[ len - 2 ] = GET_LOW_BYTE(mu16Chksum);

    return len;
}


U8 start = FALSE;
extern U8 InitPowerOn;
extern I16 tof_sensor[7];

#define TOF_STATUS_INIT     0       // 전원 리셋.. 최초 통신 전
#define TOF_STATUS_RUN      1       // 통신 수신
#define TOF_STATUS_STOP     2       // 통신 미수신
extern U8 tof_status;
static I16 ParserAck(U8 *buf)
{

    //if( buf[0] != TRUE || buf[1] != TRUE )
    //{
    //    return TRUE;
    //}

    //if( InitPowerOn == FALSE )
    //{
    //    InitPowerOn = TRUE;
    //    Sound( SOUND_EFFLUENT_END );
    //}

//    start = buf[2];

    // sensor data..
    tof_sensor[0] = GET_UINT_WORD( buf[4], buf[3] );
//    tof_sensor[1] = GET_UINT_WORD( buf[6], buf[5] );
//    tof_sensor[2] = GET_UINT_WORD( buf[8], buf[7] );
//    tof_sensor[3] = GET_UINT_WORD( buf[10], buf[9] );
//    tof_sensor[4] = GET_UINT_WORD( buf[12], buf[11] );
//    tof_sensor[5] = GET_UINT_WORD( buf[14], buf[13] );
//    tof_sensor[6] = GET_UINT_WORD( buf[16], buf[15] );


    tof_status = TOF_STATUS_RUN;
    StartTimer( TIMER_ID_COMM_TOF_ERR, SEC(1) );
    return TRUE;
}

extern U8 PowerOnAck;
static I16 ParserAckCali(U8 *buf)
{
    PowerOnAck = TRUE;

//    // ACK 
//    SetCommHeader( COMM_ID_TOF, PKT_TOF_ACK_CALI );
//    StartTimer( TIMER_ID_COMM_UART_1_TX, 0 );
    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktReq( U8 *buf );
static I16 MakePktReqCali( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_TOF_REQ,          MakePktReq  },
    { PKT_TOF_REQ_CALI,     MakePktReqCali  },
};
#define SZ_TABLE    ( sizeof( make_list ) / sizeof( make_list_t ))

I16 MakePkt_TOF( CommHeader_T *p_comm, U8 *buf )
{
    U8 mu8Type;
    action_t	p_make_pkt;
    U8 i;
    I16	len	= -1;


    for( i = 0; i < SZ_TABLE; i++ )
    {
        mu8Type = make_list[ i ].Type;
        p_make_pkt = make_list[ i ].MakePkt;

        if( mu8Type == (U8)p_comm )
        {
            if( p_make_pkt != NULL )
            {
                len = p_make_pkt( buf );
            }
            break;
        }
    }

    return len;
}



extern U8 tof_start;
static I16 MakePktReq( U8 *buf )
{
    I16 mi16Len = 0;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_TOF_REQ;
    
    // START/STOP SENSING
    buf[ mi16Len++ ] = tof_start;

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;

    return mi16Len;
}

static I16 MakePktReqCali( U8 *buf )
{
    I16 mi16Len = 0;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_TOF_REQ_CALI;
    

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;

    return mi16Len;
}


