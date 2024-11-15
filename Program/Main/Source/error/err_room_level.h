#ifndef __ERR_ROOM_LEVEL_H__
#define __ERR_ROOM_LEVEL_H__

#include "prj_type.h"


#define  DEFAULT_ROOM_ERR_COUNT                     3
#define  DEFAULT_ROOM_ERR_COUNT_HIGH_AND_COMPLEX    5

void SetRoomErrCountLimit(U8 mu8Count, U8 mu8CountOverAndComplex);

U8 CheckErrRoomOvf(U8 mu8Error);
U8 ReleaseErrRoomOvf(U8 mu8Error);

U8 CheckErrRoomComplex(U8 mu8Error);
U8 ReleaseErrRoomComplex(U8 mu8Error);

U8 CheckErrRoomLow(U8 mu8Error);
U8 ReleaseErrRoomLow(U8 mu8Error);

U8 CheckErrRoomHigh(U8 mu8Error);
U8 ReleaseErrRoomHigh(U8 mu8Error);

#endif /* __ERR_ROOM_LEVEL_H__ */
