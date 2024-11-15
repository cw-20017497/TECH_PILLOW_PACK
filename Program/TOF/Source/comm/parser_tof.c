/***********************************************************************************************
 * INCLUDE 
 */
#include <stdio.h>
#include <stdlib.h>

#include "parser_tof.h"
#include "util.h"
#include "timer.h"
#include "tof.h"

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


static I16 ParserReq(U8 *buf);
static I16 ParserReqCali(U8 *buf);

typedef I16 (*action_t)( U8 *buf );
typedef struct _parser_list_t
{
    U8 Type;
    action_t Parser;
} parser_list_t;
const static parser_list_t parser_list[] = 
{
    { PKT_TOF_REQ,          ParserReq },
    { PKT_TOF_REQ_CALI,     ParserReqCali },
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


extern U8 dbg_start;
static I16 ParserReq(U8 *buf)
{
    dbg_start = buf[0];

    // ACK 
    SetCommHeader( COMM_ID_TOF, PKT_TOF_ACK );
    StartTimer( TIMER_ID_COMM_UART_1_TX, 0 );

    return TRUE;
}

extern U8 dbg_offset;
extern U8 dbg_xtalk;
extern U8 dbg_floor;
static I16 ParserReqCali(U8 *buf)
{
    if( GetStatusOffsetCalibration() == FALSE )
    {
        dbg_offset  = TRUE;
        return FALSE;
    }

    if( GetStatusXtalkCalibration() == FALSE )
    {
        dbg_xtalk   = TRUE;
        return FALSE;
    }


    dbg_xtalk = FALSE;
    dbg_xtalk = FALSE;

    // ACK 
    SetCommHeader( COMM_ID_TOF, PKT_TOF_ACK_CALI );
    StartTimer( TIMER_ID_COMM_UART_1_TX, 0 );
    return TRUE;
}


typedef struct _make_list_t
{
    U8  Type;
    action_t    MakePkt;
} make_list_t;

static I16 MakePktAck( U8 *buf );
static I16 MakePktAckCali( U8 *buf );
const static make_list_t make_list[] = 
{
    { PKT_TOF_ACK,          MakePktAck  },
    { PKT_TOF_ACK_CALI,     MakePktAckCali  },
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



//extern U8 tof_start;
static I16 MakePktAck( U8 *buf )
{
    I16 mi16Len = 0;
    U16 range = 0;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_TOF_ACK;
    
    // START/STOP SENSING
    buf[ mi16Len++ ] = GetStatusOffsetCalibration();
    buf[ mi16Len++ ] = GetStatusXtalkCalibration();
    buf[ mi16Len++ ] = dbg_start;

    range = (U16) GetRange( 0 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );


    range = (U16) GetRange( 1 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );

    range = (U16) GetRange( 2 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );

    range = (U16) GetRange( 3 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );

    range = (U16) GetRange( 4 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );

    range = (U16) GetRange( 5 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );

    range = (U16) GetRange( 6 );
    buf[ mi16Len++ ] = GET_HIGH_BYTE( range );
    buf[ mi16Len++ ] = GET_LOW_BYTE( range );


    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;

    return mi16Len;
}

static I16 MakePktAckCali( U8 *buf )
{
    I16 mi16Len = 0;

    // STX 
    buf[ mi16Len++ ] = STX;

    // MESSAGE TYPE
    buf[ mi16Len++ ] = PKT_TOF_ACK_CALI;
    

    // CRC-16
    buf[ mi16Len++ ] = 0;
    buf[ mi16Len++ ] = 0;

    buf[ mi16Len++ ] = ETX;

    return mi16Len;
}


