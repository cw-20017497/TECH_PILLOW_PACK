#ifndef __SPECIAL_MODE_H__
#define __SPECIAL_MODE_H__

#include "prj_type.h"

typedef enum
{
    SPECIAL_NONE,
    SPECIAL_WAIT,
    SPECIAL_HOT_ALTITUDE,

    SPECIAL_MAX
}SpecialModeId_T;

void InitSpeicalMode(void);

void SetSpecialMode ( SpecialModeId_T status );
SpecialModeId_T GetSpecialMode ( void );
U8 IsExpiredSepcialModeLimitTimer(void);

void ProcessSpecialMode(void);

#endif /* __TIME_SHORT_H__ */
