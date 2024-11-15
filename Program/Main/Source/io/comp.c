#include "comp.h"
#include "relay.h"
#include "Temp.h"

#include <string.h>

// ??i¢“?e¢¨??°∆i¢”ç ?¢•i?? ?©´e°∆? ?©´?©´ 
#define CONFIG_PROTECT_ON_TIME      1       // 1: ?°∆i¢”ç ?¢•i?? ?©´e°∆? ?©´?©´ ?°˛i?? 0: e?¢¨i?°˛i??

/* PROTECT TIME */
#define  PROTECT_OFF_TIME  300UL            // 5min( 5min x 60sec  = 300sec )

#define  PROTECT_ON_COLD_TEMP    5.0f       // PROTECT_ON_TIME_1ST && PROTECT_ON_COLD_TEMP A¢“°∆C®ˆA Comp OFF
#define  PROTECT_FIRST_ON_TIME   7800UL     // 130min ( 130min x 60sec =  7800sec )
#define  PROTECT_SECOND_ON_TIME  9000UL     // 150min ( 150min x 60sec =  9000sec )

/* ON/OFF TIME LIMITED */
#define  MAX_TIME          60000UL  // 1sec


Compressor_T  Comp;


void InitComp(void)
{
    Comp.Mode        = COMP_MODE_NONE;
    Comp.Cold_OnOff   = OFF;
    Comp.Ice_OnOff    = OFF;
    Comp.ForcedOnOff = OFF;

    Comp.ProtectOffTime   = PROTECT_OFF_TIME;

    Comp.OnTime      = 0;
    Comp.OffTime     = 0;
}

void GetCompData( Compressor_T *pData )
{
    memcpy( pData, &Comp, sizeof( Compressor_T ));
}

void SetCompMode(U8 mu8Mode)
{
    Comp.Mode = mu8Mode;
}

U8 GetCompMode(U8 mu8Mode)
{
    return Comp.Mode;
}

void SetCompProtectOffTime(U16 mu16Val)
{
    Comp.ProtectOffTime = mu16Val;
}

U16 GetCompProtectOffTime(void)
{
    return Comp.ProtectOffTime;
}

U8 IsOverCompProtectOnTime(void)
{
#if CONFIG_PROTECT_ON_TIME
    TEMP_T mColdTemp;
    
    mColdTemp = GetTemp( TEMP_ID_COLD_WATER );

    if( Comp.OnTime > PROTECT_FIRST_ON_TIME 
        && mColdTemp < PROTECT_ON_COLD_TEMP ) 
    {
        return TRUE; // ∫∏»£ Ω√∞£ √ ∞˙
    }
#endif

    return FALSE; // ∫∏»£ Ω√∞£ ¿Ã≥ª
}

U8 IsOverCompProtectSecondOnTime(void)
{
#if CONFIG_PROTECT_ON_TIME
    if( Comp.OnTime > PROTECT_SECOND_ON_TIME )
    {
        return TRUE; // ∫∏»£ Ω√∞£ √ ∞˙
    }
#endif

    return FALSE; // ∫∏»£ Ω√∞£ ¿Ã≥ª
}

void TurnOnComp(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        Comp.Cold_OnOff = ON;
    }
    else
    {
        Comp.Ice_OnOff = ON;
    }
}

void TurnOffComp(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        Comp.Cold_OnOff = OFF;
    }
    else
    {
        Comp.Ice_OnOff = OFF;
    }
}

U8 GetCompOnOffType(U8 mu8Type)
{
    if( mu8Type == COMP_TYPE_COLD )
    {
        return Comp.Cold_OnOff;
    }

    return Comp.Ice_OnOff;
}

void TurnOnForcedComp(void)
{
    Comp.ForcedOnOff = ON;
}

void TurnOffForcedComp(void)
{
    Comp.ForcedOnOff = OFF;
}


U8 GetCompOnOff(void)
{
    if( IsTurnOnRelay( RELAY_COMP ) == TRUE )
    {
        return ON;
    }

    return OFF;
}


U8 IsExpiredCompProtectTime(void)
{
    if( IsTurnOnRelay( RELAY_COMP ) == FALSE 
            && Comp.ProtectOffTime != 0 )
    {
        return FALSE;   // COMP OFF ªÛ≈¬
    }

    return TRUE;    // COMP ON ªÛ≈¬
}

static void UpdateCompTimer(void)
{
    // æ–√‡±‚∞° OFF¿Ã∏È, Update off timer value
    // ±◊∑∏¡ˆ æ ¿∏∏È √ ±‚»≠
    if( IsTurnOnRelay( RELAY_COMP ) == FALSE )
    {
        if( Comp.ProtectOffTime != 0 )
        {
            Comp.ProtectOffTime--;
        }
    }
    else
    {
        Comp.ProtectOffTime = PROTECT_OFF_TIME;
    }
}

static void ControlComp(void)
{
    U8 mu8OnOff = OFF;


    // 0. Get OnOff Command
    if( Comp.Cold_OnOff == ON )
    {
        mu8OnOff = ON;
    }

    if( Comp.Ice_OnOff == ON )
    {
        mu8OnOff = ON;
    }


    // 1. Protect Comp Off Time, Priority-2
    if( IsExpiredCompProtectTime() == FALSE )
    {
        mu8OnOff = OFF;
    }

    // 2. Forced OnOff, Prioirty-1
    if( Comp.Mode == COMP_MODE_FORCED )
    {
        mu8OnOff = Comp.ForcedOnOff;
    }
    
    
    // 3. Control Compressor 
    if( mu8OnOff == ON )
    {
        // Turn on Comp
        TurnOnRelay( RELAY_COMP );

        // Count On Timer
        Comp.OffTime = 0;
        if( Comp.OnTime < MAX_TIME )
        {
            Comp.OnTime++;
        }

        // Reload Protect Comp Off Time
        Comp.ProtectOffTime = PROTECT_OFF_TIME;

    }
    else 
    {
        // Reload Protect Comp Off Time 
        if( IsTurnOnRelay( RELAY_COMP ) == TRUE )
        {
            Comp.ProtectOffTime = PROTECT_OFF_TIME;
        }
        
        // Turn Off Comp
        TurnOffRelay( RELAY_COMP );

        // Count Off timer
        Comp.OnTime = 0;
        if( Comp.OffTime < MAX_TIME )
        {
            Comp.OffTime++;
        }
    }
}

void ProcessComp(void)
{
    ControlComp();
    UpdateCompTimer();
}
