#ifndef __PARSER_TOF_H__
#define __PARSER_TOF_H__

#include "prj_type.h"
#include "comm.h"


#define PKT_TOF_REQ         0x01
#define PKT_TOF_ACK         0x81

#define PKT_TOF_REQ_CALI    0x02
#define PKT_TOF_ACK_CALI    0x82

I16 IsValidPkt_TOF( U8 *buf, I16 len );
I16 ParserPkt_TOF( U8 *buf, I16 len);
I16 Crc16_TOF( U8 *buf, I16 len );
I16 MakePkt_TOF( CommHeader_T *p_comm, U8 *buf );


#endif /* __PARSER_TOF_H__ */
