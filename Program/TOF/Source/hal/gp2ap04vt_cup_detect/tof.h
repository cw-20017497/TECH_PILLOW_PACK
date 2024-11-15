#ifndef __TOF__
#define __TOF__

#include "prj_type.h"


#define TOF_ID_1    0
#define MAX_TOF_ID  1

void InitToF(void);

void DoCalibration(void);

void StartRange(U8 id);

U8 CheckStartRange(U8 id );

void StopRange(U8 id);

I16 GetRange(U8 id);

U8 GetStatusOffsetCalibration(void);

U8 GetStatusXtalkCalibration(void);

void ProcessToF(void);

#endif /* __TOF__ */
