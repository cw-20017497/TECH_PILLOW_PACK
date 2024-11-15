#ifndef __PARSER_EOL_H__
#define __PARSER_EOL_H__

#include "prj_type.h"
#include "comm.h"

enum
{
    PKT_EOL_NONE                = 0x00,

    PKT_EOL_MODEL               = 0x20, // START
    PKT_EOL_MODE                = 0x22,

    PKT_EOL_EEPROM              = 0x32,
    PKT_EOL_SENSOR_TEMP         = 0x35,     //0x1C,
    PKT_EOL_SENSOR_LEVEL        = 0x38,

    PKT_EOL_HEATER              = 0x50,     //0xA1,
    PKT_EOL_NO_LOAD             = 0x54,     //0x57,

    PKT_EOL_FEED_VALVE          = 0x58, //0x63,
    PKT_EOL_NOS_VALVE           = 0x5C,

    PKT_EOL_GAS_FEED_VALVE      = 0x78,  // feed open, nos open ( 200 )
    PKT_EOL_GAS_NOS_VALVE_CLOSE = 0x7D,  // feed open, nos open, hot close, room open ( 30 )
    PKT_EOL_GAS_NOS_VALVE_OPEN  = 0x83,  // feed open, nos close, hot open ( 30 )

    PKT_EOL_COMP                = 0x99,
    PKT_EOL_COMPLETE            = 0xF1,
};


I16 IsValidPkt_EOL( U8 *buf, I16 len );
I16 ParserPkt_EOL( U8 *buf, I16 len);
I16 Crc16_EOL( U8 *buf, I16 len );
I16 MakePkt_EOL( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_EOL_H__ */
