#ifndef __COMP_H__
#define __COMP_H__

#include "prj_type.h"

typedef struct _compressor_
{
    /* COMMAND */
    U8 Mode;         // Command mode
    U8 Cold_OnOff;   // Cold mode - onoff command
    U8 Ice_OnOff;    // Ice mode - onoff command
    U8 ForcedOnOff;  // Forced mode - onoff command

    U16 ProtectOffTime;

    U32 OnTime;
    U32 OffTime;
} Compressor_T;

void InitComp(void);

void GetCompData( Compressor_T *pData );

#define  COMP_MODE_NONE     0
#define  COMP_MODE_FORCED   1
void SetCompMode(U8 mu8Mode);
U8 GetCompMode(U8 mu8Mode);

void SetCompProtectOffTime(U16 mu16Val);
U16 GetCompProtectOffTime(void);
U8 IsOverCompProtectOnTime(void);
U8 IsOverCompProtectSecondOnTime(void);

/* COMP TYPE */
#define  COMP_TYPE_COLD    0
#define  COMP_TYPE_ICE     1
void TurnOnComp(U8 mu8Type);
void TurnOffComp(U8 mu8Type);
U8 GetCompOnOffType(U8 mu8Type);

void TurnOnForcedComp(void);
void TurnOffForcedComp(void);

// Comp On/Off Command
// RETURN : ON or OFF
U8 GetCompOnOff(void);

U8 IsExpiredCompProtectTime(void);
void ProcessComp(void);

#endif /* __COMP_H__ */
