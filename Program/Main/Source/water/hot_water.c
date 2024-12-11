#include "hot_water.h"
#include "room_water.h"
#include "cold_water.h"
#include "level.h"
#include "temp.h"
#include "eeprom.h"

#include "error.h"
#include "room_water.h"
#include "valve.h"
#include "relay.h"

/* ÎØ∏ÏÇ¨Ïö??à?Ñ ?∏Í∏∞ ?®ÎèÑ?ê ?∞Î•∏ ?ô?ë */
#define USE_SAVING_TEMP             0   /* 1 : ?¨Ïö? 0: ÎØ∏ÏÇ¨Ïö?*/
#define CONFIG_HOT_WATER_AIRFLOW    0   // ?®Ïàò ?±ÌÅ?airÎ°ú ?∏Ìï??òÎ™ªÎêú ?à?Ö Í∞ú?†

/* TARGET OFF TEMP - PROTECTION OVER-HEATING */
#define PROTECT_HOT_TEMP   98.0f

#define WAIT_TIME   5 // @1sec
#define AIR_FLOW_TIME   3000 // @100ms
#define WAIT_HEATING_TIME   900 // @1sec, 15x60=300


/*  ?Ãª??? ???? TEMP STATUS */
#define OVER_TEMP       0       
#define UNDER_TEMP      1
#define DEFAULT_TEMP_TIME   5   // @1sec, 5sec
#define AMBI_LIMIT_TEMP     20.0f

/* TARGET TEMP - NORMAL */
static const TEMP_T OnTempList[ ALTITUDE_LEVEL_NUM ]  = { 82.0f, 79.0f, 68.0f, 53.0f };
static const TEMP_T OffTempList[ ALTITUDE_LEVEL_NUM ] = { 93.0f, 90.0f, 84.0f, 58.0f };


HotWater_T Hot;


void  InitHotWater(void)
{
    Hot.InitWaitTime = WAIT_TIME;           // ?®ÎèÑ, ?ò?Ñ Ï¥àÍ∏??ΩÎäî ?úÍ∞Ñ ?ÄÍ∏?
    Hot.InitFull     = FALSE;
    Hot.Level        = LEVEL_LOW;
    
    Hot.ConfigMake   = TRUE;
    Hot.Make         = FALSE;
    Hot.MakeDelay    = WAIT_HEATING_TIME;

    Hot.Altitude     = ALTITUDE_LEVEL_0;

    Hot.Saving.Config   = FALSE;        // ÎØ∏ÏÇ¨Ïö??à?Ñ Ï§ëÏßÄ
    Hot.Saving.Run      = FALSE;

    Hot.Saving.OnTime    = UNUSED_SAVING_ON_10HOUR;
    Hot.Saving.RunTime   = 0U;

    Hot.Saving.TempStatus     = OVER_TEMP;
    Hot.Saving.TempUnderTime  = DEFAULT_TEMP_TIME;
    Hot.Saving.TempOverTime   = DEFAULT_TEMP_TIME;

}

void  GetHotWaterData( HotWater_T *pData )
{
    memcpy( pData, &Hot, sizeof( HotWater_T ));
}

void  SetHotWaterInitFull(U8 mu8Val )
{
    Hot.InitFull = mu8Val;
}

U8    GetHotWaterInitFull(void)
{
    return Hot.InitFull;
}


void  SetHotWaterUnusedOnTime(U16 mu16OnTime)
{
    Hot.Saving.OnTime = mu16OnTime;
}

U16  GetHotWaterUnusedOnTime(void)
{
    return Hot.Saving.OnTime;
}


void  SetHotWaterLevel(U8 mu8Val )
{
    Hot.Level = mu8Val;
}

U8    GetHotWaterLevel(void)
{
    return Hot.Level;
}

void  SetHotWaterConfigMake(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }
    Hot.ConfigMake = mu8Val;

    SaveEepromId( EEP_ID_CONF_MAKE_HOT );
}

U8    GetHotWaterConfigMake(void)
{
    return Hot.ConfigMake;
}


void  SetHotWaterMake(U8 mu8Val )
{
    Hot.Make = mu8Val;
}

U8    GetHotWaterMake(void)
{
    return Hot.Make;
}


void  SetHotWaterAltidue(U8 mu8Val )
{
    if( mu8Val < ALTITUDE_LEVEL_NUM )
    {
        Hot.Altitude = mu8Val;
        /* Save EEPROM */
        SaveEepromId( EEP_ID_HOT_ALTITUDE );
    }
}

U8    GetHotWaterAltidue(void)
{
    return Hot.Altitude;
}




void UpdateHotMakeDelay(void)
{
    Hot.MakeDelay = 0U;
}


void  ControlHotWaterLevel(void)
{
    /* Update Water Level */
    Hot.Level = GetTankLevel( LEVEL_ID_HOT );

    if( Hot.Level == LEVEL_HIGH )
    {
        Hot.InitFull = TRUE;
    }
}


static U8 IsValidMake(void)
{
    if( Hot.InitWaitTime != 0 )
    {
        return FALSE;
    }

    /* Turn Off make, dont want to make */
    if( Hot.ConfigMake == FALSE )
    {
        return FALSE ;
    }

    if( Hot.InitFull == FALSE )
    {
        return FALSE;
    }

    /* Turn Off make, error */
    if( IsErrorTypeHotWater() == TRUE )
    {
        return FALSE;
    }

    /* Turn On make, Level */
    if( Hot.Level == LEVEL_MID 
        || Hot.Level == LEVEL_HIGH )
    {
        return TRUE ;
    }

    return FALSE;
}

TEMP_T   GetTargetHotOnTemp(U8 mu8Altitude)
{
    if( mu8Altitude >= ALTITUDE_LEVEL_NUM )
    {
        mu8Altitude = 0;
    }

    return OnTempList[ mu8Altitude ];
}

TEMP_T   GetTargetHotOffTemp(U8 mu8Altitude )
{
    if( mu8Altitude >= ALTITUDE_LEVEL_NUM )
    {
        mu8Altitude = 0;
    }

    return OffTempList[ mu8Altitude ];
}

static U8 CheckLevelOnHeating(void)
{
    static U8 mu8PrevStatus = LEVEL_LOW;
    U8 mu8CurStatus;


    // Check condi, detect low level
    mu8CurStatus = GetTankLevel( LEVEL_ID_ROOM );
    if( mu8PrevStatus != mu8CurStatus )
    {
        if( mu8PrevStatus == LEVEL_LOW 
                && mu8CurStatus != LEVEL_LOW )
        {
            Hot.MakeDelay = WAIT_HEATING_TIME;
        }
        mu8PrevStatus = mu8CurStatus;
    }


    // Check condi, init full 
    if( GetRoomWaterInitFull() == FALSE )
    {
        Hot.MakeDelay = WAIT_HEATING_TIME;

        return FALSE;
    }

    // Counting delay..
    if( Hot.MakeDelay != 0 )
    {
        Hot.MakeDelay--;
        Hot.Make = FALSE;

        return FALSE;   // Wait Heating..
    }

    return TRUE;    // Ok heating..
}

static void UpdateTime(void)
{
    if( Hot.InitWaitTime != 0 )
    {
        Hot.InitWaitTime--;
    }

    /* ?Ãª??? ???? ???? ?√∞? */
    if( Hot.Saving.Run == TRUE )
    {
        if( Hot.Saving.RunTime < 60000U )
        {
            Hot.Saving.RunTime++;
        }
    }
}




#define HEAT_TIME               1800     // 3min @100ms
#define AIR_VALVE_ON_TIME       50       // 5sec @100ms
U16 HeatingTime     = HEAT_TIME;
U16 ValveOpenTime   = 0;

void ControlAirValve(void)
{
    U8 mu8OnOff = OFF;

    // air πÎ∫Í∞° ¥›»˘ ªÛ≈¬ø°º≠ »˜∆√ Ω√∞£ ∞ÀªÁ
    if( IsTurnOnRelay( RELAY_HEATER ) == TRUE 
            && IsOpenValve( VALVE_AIR ) == FALSE )
    {
        if( HeatingTime != 0 )
        {
            HeatingTime--;
        }
        else
        {
            ValveOpenTime = AIR_VALVE_ON_TIME;
        }
    }
    else
    {
        HeatingTime = HEAT_TIME;
    }


    // 1. ±ﬁºˆ ¡ﬂ ¡¶æÓ
    if( IsOpenValve( VALVE_FEED ) == TRUE )
    {
        mu8OnOff = ON;
    }
    else 
    {
        mu8OnOff = OFF;
    }


    // 2. ±ﬁºˆ ¡ﬂ ¡¶æÓ
    if( ValveOpenTime != 0 )
    {
        ValveOpenTime--;
        mu8OnOff = ON;
    }


    // open/clse air valve;
    if( mu8OnOff == ON )
    {
        OpenValve( VALVE_AIR );
    }
    else
    {
        CloseValve( VALVE_AIR );
    }
}

void  MakeHotWater(void)
{
    UpdateTime();
    ControlAirValve();

    // Get Target On , Target Off, Current Temp 
    Hot.TempTargetOn  = GetTargetHotOnTemp( Hot.Altitude );
    Hot.TempTargetOff = GetTargetHotOffTemp( Hot.Altitude );
    Hot.TempCurrent       = GetTemp( TEMP_ID_HOT_WATER );


    //if( CheckLevelOnHeating() == FALSE )
    //{
    //    Hot.Make = FALSE;
    //    return ;
    //}

    if( IsValidMake() == FALSE )
    {
        Hot.Make = FALSE;
        return ;
    }

    // Check Turn Off 
    if( Hot.Make == TRUE )
    {
        if( Hot.TempTargetOff <= Hot.TempCurrent )
        {
            Hot.Make = FALSE;
        }
    }
    // Check Turn On 
    else
    {
        if( Hot.TempTargetOn >= Hot.TempCurrent )
        {
            Hot.Make = TRUE;
        }
    }

    /* Protection, OVER-HEAINTG */
    if( Hot.TempCurrent >= PROTECT_HOT_TEMP )
    {
        Hot.Make = FALSE;
    }
}

/* ?Ãª??? ???? ???? ???? */
void SetUnusedSavingConfig(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    Hot.Saving.Config = mu8Val;
}
U8 GetUnusedSavingConfig(void)
{
    return Hot.Saving.Config;
}

/* ?Ãª??? ???? ???? ???? */
void SetUnusedSaving(U8 mu8Val )
{
    Hot.Saving.Run = mu8Val;
}

U8 GetUnusedSaving(void)
{
    return Hot.Saving.Run;
}


void ResetUnusedSavingTime(void)
{
    Hot.Saving.Run = FALSE;

    Hot.Saving.RunTime = 0;
    Hot.Saving.OnTime  = UNUSED_SAVING_ON_6HOUR;
}

#if USE_SAVING_TEMP
// ?∏Í∏∞ ?®ÎèÑ?ê ?∞Î•∏ ÎØ∏ÏÇ¨Ïö??à?Ñ ?ô?ë ?§Ï†ï
static void CountTempDelayTime(void)
{
    /* ?‹±? ?¬µ? ??»≠ ?√∞? ???? ???? */
    if( GetTemp( TEMP_ID_AMBIENT ) > 20.0f )
    {
        if( Hot.Saving.TempOverTime != 0 )
        {
            Hot.Saving.TempOverTime--;
        }
        Hot.Saving.TempUnderTime = DEFAULT_TEMP_TIME;
    }
    else
    {
        if( Hot.Saving.TempUnderTime != 0 )
        {
            Hot.Saving.TempUnderTime--;
        }
        Hot.Saving.TempOverTime = DEFAULT_TEMP_TIME;
    }

    /* ?‹±? ?¬µ? ???? ??¡§ */
    if( Hot.Saving.TempOverTime == 0 )
    {
        Hot.Saving.TempStatus = OVER_TEMP;
    }
    else if( Hot.Saving.TempUnderTime == 0 )
    {
        Hot.Saving.TempStatus = UNDER_TEMP;
    }
}
#endif


void ProcessUnusedSaving(void)
{
#if USE_SAVING_TEMP
    CountTempDelayTime();
#endif

    /* ÎØ∏ÏÇ¨Ïö??à?Ñ ?¥Ï†ú */
    if( Hot.Saving.Config == FALSE )
    {
        ResetUnusedSavingTime();
        return ;
    }

    /* ?‹±? ?¬µ? 20'C ?????? ???? ??¡¶ */
#if USE_SAVING_TEMP
    if( Hot.Saving.Run == TRUE )
    {
        if( Hot.Saving.TempStatus == UNDER_TEMP )
        {
            ResetUnusedSavingTime();
            return ;
        }
    }
#endif

    /* ?±ÌÅ??ò?Ñ Î≥Ä?î -> ?¨Ïö©Ïûê Ï∂îÏ∂ú ?ô?ë */
    if( GetRoomWaterLevel() != LEVEL_HIGH )
    {
        ResetUnusedSavingTime();
        return ;
    }

    if( Hot.Saving.OnTime != 0 )
    {
        Hot.Saving.OnTime--;
    }
    else 
    {
#if USE_SAVING_TEMP
        if( Hot.Saving.TempStatus == OVER_TEMP )
#endif
        {
            Hot.Saving.Run = TRUE;
        }
    }
}
