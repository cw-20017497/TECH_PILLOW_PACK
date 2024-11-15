#ifndef __PARSER_DEBUG_H__
#define __PARSER_DEBUG_H__

#include "prj_type.h"
#include "comm.h"

I16 IsValidPkt_Debug( U8 *buf, I16 len );
I16 ParserPkt_Debug( U8 *buf, I16 len);
I16 Crc16_Debug( U8 *buf, I16 len );
I16 MakePkt_Debug( CommHeader_T *p_comm, U8 *buf );

#endif /* __PARSER_DEBUG_H__ */
