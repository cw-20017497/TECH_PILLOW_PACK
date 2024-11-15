#include "special_mode.h"


#define  SPECIAL_LIMIT_TIME         30U     /* 30sec, @1sec */
#define  SPECIAL_OPERATING_TIME     1800U   /* 30min, 60sec x 30min = 1800, @1sec */



typedef struct _special_mode_
{
    SpecialModeId_T Status;           /* Mode Status */
    U16 LimitTimer;                 /* Enter FCT Mode Timeout */
    U16 OperatingTimer;             /* Interval Timer */
    
} SpecialMode_T;

SpecialMode_T Special;


void InitSpeicalMode(void)
{
    Special.Status          = SPECIAL_NONE;
    Special.LimitTimer      = SPECIAL_LIMIT_TIME;
    Special.OperatingTimer  = SPECIAL_OPERATING_TIME;
}

void SetSpecialMode ( SpecialModeId_T status )
{
    Special.Status = status;
    Special.OperatingTimer = SPECIAL_OPERATING_TIME;
}

SpecialModeId_T GetSpecialMode ( void )
{
    return Special.Status;
}

U8 IsExpiredSepcialModeLimitTimer(void)
{
    if( Special.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

static void UpdateSpecialModeTimer(void)
{
    if( Special.LimitTimer != 0 )
    {
        Special.LimitTimer--;
    }

    if( Special.OperatingTimer != 0 )
    {
        Special.OperatingTimer--;
    }
    else
    {
        Special.Status = SPECIAL_NONE;
    }
}

void ProcessSpecialMode(void)
{
    UpdateSpecialModeTimer();
}
