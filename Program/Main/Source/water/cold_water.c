#include "cold_water.h"
#include "room_water.h"
#include "level.h"
#include "temp.h"
#include "valve.h"
#include "error.h"
#include "comp.h"
#include "eeprom.h"


/* TARGET OFF TEMP - PROTECTION SUB-COOL */
#define PROTECT_COLD_TEMP   0.0f
#define COLD_OFF_TEMP       4.5f     

/* TARGET TEMP - NORMAL */
static const TEMP_T OnTemp  = 6.8f; 
static const TEMP_T OffTemp = COLD_OFF_TEMP;

/* COLD IN DELAY TIME, WHEN COLD TANK LEVEL IS RELEASED HIGH LEVEL AND COLD OUT */
#define COLD_TANK_FEED_TIME 600U /* 60sec @100ms */

//#define EXTRA_MAKE_TIME   300U /* 5min @1sec */
#define EXTRA_MAKE_TIME_LONG     1020U /* 17min @1sec */
#define EXTRA_MAKE_TIME_SHORT    540U  /* 9min @1sec */
#define EXTRA_MAKE_CHECK_TIME    60U   /* 1min @1sec */

ColdWater_T Cold;


void  InitColdWater(void)
{
    Cold.InitFull       = FALSE;
    Cold.Level          = LEVEL_HIGH;
    Cold.ConfigMake     = TRUE;
    Cold.Make           = FALSE;
    Cold.Mode           = COLD_MODE_INIT;
    Cold.TempTargetOn   = OnTemp;
    Cold.TempTargetOff  = OffTemp;
    Cold.TempCurrent    = GetTemp( TEMP_ID_COLD_WATER );
    Cold.FeedTime       = COLD_TANK_FEED_TIME;
    Cold.ExtraFeedTime  = EXTRA_FEED_TIME;

    Cold.ExtraMake      = FALSE;
    Cold.ExtraMakeTime  = 0;
    Cold.ExtraMakeCheckTime = 0;
}


void  GetColdWaterData(ColdWater_T *pData)
{
    memcpy( pData, &Cold, sizeof( ColdWater_T ));
}

void  SetColdWaterInitFull(U8 mu8Val )
{
    Cold.InitFull = mu8Val;
}

U8    GetColdWaterInitFull(void)
{
    return Cold.InitFull;
}


void  SetColdWaterLevel(U8 mu8Val )
{
    Cold.Level = mu8Val;
}

U8    GetColdWaterLevel(void)
{
    return Cold.Level;
}

void  SetColdWaterConfigMake(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    Cold.ConfigMake = mu8Val;

    SaveEepromId( EEP_ID_CONF_MAKE_COLD );
}

U8    GetColdWaterConfigMake(void)
{
    return Cold.ConfigMake;
}


void  SetColdWaterMake(U8 mu8Val )
{
    Cold.Make = mu8Val;
}

U8    GetColdWaterMake(void)
{
    return Cold.Make;
}

// ³Ã¼ö ÀÔ¼ö Áö¿¬ ½Ã°£
void  SetColdWaterFeedTime(U16 mu16Time )
{
    Cold.FeedTime = mu16Time;
}

U16    GetColdWaterFeedTime(void)
{
    return Cold.FeedTime;
}

void SetColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode |= mu8Mode;
}

void ClearColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode &= ~mu8Mode;
}

U8 IsSetColdWaterMakeMode(U8 mu8Mode )
{
    if( (Cold.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetColdWaterMakeMode(void )
{
    return Cold.Mode;
}


// ³Ã¼ö ÀÔ¼ö Áö¿¬ ½Ã°£
void  SetColdWaterExtraMakeTime(U16 mu16Time )
{
    Cold.ExtraMakeTime = mu16Time;
}

U16   GetColdWaterExtraMakeTime(void)
{
    return Cold.ExtraMakeTime;
}



/*
 * ³Ã¼ö ÅÊÅ© ¼öÀ§ Á¦¾î
 */
void  ControlColdWaterLevel(void)
{
    Cold.Level = GetTankLevel( LEVEL_ID_COLD );

    if( Cold.Level == LEVEL_HIGH )
    {
        SetColdWaterInitFull( TRUE );
    }
}


static U8 IsValidMake(void)
{
    /* Turn Off make, dont want to make */
    if( Cold.ConfigMake == FALSE )
    {
        return FALSE ;
    }


    /* ³Ã¼ö ÅÊÅ©°¡ ÃÖÃÊ ¸¸¼öÀ§¸¦ ÇÏÁö ¸øÇÑ °æ¿ì */
    if( Cold.InitFull == FALSE )
    {
        return FALSE;
    }

    /* ¾ÐÃà±â ±¸¼Ó ½Ã°£ */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    /* 130min && 5¥ì¥ì AICI */
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }

    /* 150min */
    if ( IsOverCompProtectSecondOnTime() == TRUE )
    {
        return FALSE;
    }
    
    /* Turn Off make, Error */
    if( IsErrorTypeColdWater() == TRUE )
    {
        return FALSE;
    }

    return TRUE;  // Yes, valid
}


TEMP_T   GetTargetColdOnTemp(void)
{

    return OnTemp;
}

TEMP_T   GetTargetColdOffTemp(void)
{
    return OffTemp;
}


// ³Ã°¢ Ãß°¡ Å¸ÀÌ¸Ó decounter
static void UpdateExtraMakeTime(void)
{
    if( GetCompOnOff() == OFF )
    {
        return ;
    }

    if ( Cold.ExtraMake == TRUE )
    {
        if ( Cold.ExtraMakeTime != 0  )
        {
            Cold.ExtraMakeTime--;
        }
        else
        {
            Cold.ExtraMakeCheckTime = 0;
        }
    }
}

static void SetExtraMakeCheckTime ( void )
{
    if ( Cold.ExtraMakeCheckTime < EXTRA_MAKE_CHECK_TIME )
    {
        Cold.ExtraMakeCheckTime++;
    }
    else if ( Cold.ExtraMakeCheckTime == EXTRA_MAKE_CHECK_TIME )
    {
        Cold.ExtraMakeCheckTime++;
        
        if ( Cold.TempCurrent > 30.0f )
        {
            Cold.ExtraMakeTime = EXTRA_MAKE_TIME_LONG;
        }
        else if ( Cold.TempCurrent > 15.0f )
        {
            Cold.ExtraMakeTime = EXTRA_MAKE_TIME_SHORT;
        }
        else
        {
            Cold.ExtraMakeTime = 0;
        }
    }
    else
    {
    }
}


// ?¢¶?? ???i¢®¡Æe¡¾¢¥ e©÷¢æ??
void  MakeColdWater(void)
{
    U8 mu8Make = FALSE;

    UpdateExtraMakeTime();

    // Get Current Make Status
    mu8Make = GetColdWaterMake();

    // Get Temperture
    Cold.TempTargetOn  = GetTargetColdOnTemp();
    Cold.TempTargetOff = GetTargetColdOffTemp();
    Cold.TempCurrent   = GetTemp( TEMP_ID_COLD_WATER );


    // Ãß°¡ Á¦ºù Á¶°ÇÀÎ °æ¿ì,
    // ¿Âµµ Á¦¾î FALSE Á¶°ÇÀ¸·Î °Ë»çÇØ¾ßÇÑ´Ù. 
    if( Cold.ExtraMake == TRUE )
    {
        mu8Make = FALSE;
    }

    if( mu8Make == TRUE )
    {
        // ³Ã¼ö ¿Âµµ°¡ ±âÁØ ÀÌÇÏ°¡ µÇ¸é Ãß°¡ ³Ã°¢ ÈÄ Á¾·á..
        if( Cold.TempTargetOff >= Cold.TempCurrent )
        {
            // Start Off Timer...
            Cold.ExtraMake = TRUE;
            mu8Make = FALSE;
        }
        
        /* ³Ã°¢ ½ÃÀÛ 1ºÐ ÈÄ Ãß°¡ Á¦ºù½Ã°£ °áÁ¤ */
        SetExtraMakeCheckTime();
    }
    else
    {
        // Cold ReStart
        if( Cold.TempTargetOn <= Cold.TempCurrent )
        {
            mu8Make = TRUE;
        }
    }

    // Extra Make Cold 
    if( Cold.ExtraMakeTime != 0 )
    {
        mu8Make = TRUE;
        
        if ( Cold.TempCurrent <= PROTECT_COLD_TEMP )        /* A©¬¡Æ¢® ©øA¡Æ¡Ë A©¬ ©øA¨ùo¢¯A¥ì¥ì 0¥ì¥ì AICI¢¬e A©¬¡Æ¢® ©øA¡Æ¡Ë A¢´Ao */
        {
            mu8Make = FALSE;
            Cold.ExtraMakeTime = 0;
            Cold.ExtraMakeCheckTime = 0;

        }
    }
    else
    {
        Cold.ExtraMake = FALSE;
    }

#if 0
    // Protection Off: 1¥ì¥ì ¡Æ¡§Ao¨öA
    if( Cold.TempCurrent <= PROTECT_COLD_TEMP )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
    }
#endif

    // Config Off
    if( IsValidMake() == FALSE )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
        Cold.ExtraMakeCheckTime = 0;
    }

    // Set new make status
    SetColdWaterMake( mu8Make );
}

