#include "key_special_handler.h"

#include "key_common_handler.h"
#include "key.h"

#include "process_display.h"
#include "eeprom.h"
#include "sound.h"
#include "hot_water.h"
#include "special_mode.h"
#include "time_short.h"

/* ¿Â¼ö °íµµ ¸ðµå */
static U8 OnOffAltitude(void);
static U8 OnAltitude(void);
static U8 SelAltitude(void);


/* Test ¸ðµå */
static U8 OnTimeShort(void);
static U8 OnTestShortCompTime(void);

const static KeyEventList_T Special_KeyEventList[] =

{
    /* KEY,            Short,           2sec,            3sec,       5sec,           Pop,   1sec */
    /* SINGLE KEY */
    { K_HOT,            SelAltitude,    NULL,            NULL,       NULL,           NULL,  OnAltitude      },
    { K_COLD,           NULL,           NULL,            NULL,       NULL,           NULL,  OnTimeShort     }

};

U8 IsValidSpecialKeyCondition(U16 mu16Key)
{
    return TRUE;
}

void* GetSpecialKeyEventList(void)
{
    return (void *)Special_KeyEventList;
}

U8 GetSpecialKeyEventListSize(void)
{
    return ( sizeof(Special_KeyEventList) / sizeof(KeyEventList_T) );
}

static U8 OnOffAltitude(void)
{
    U8 mu8Sound;

    if( GetAltidueConfig() == TRUE )
    {
        SetAltitudeConfig( FALSE );
        SaveEepromId( EEP_ID_HOT_ALTITUDE );
        
        return SOUND_CANCEL;
    }

    SetAltitudeConfig( TRUE );
    return SOUND_SETUP;
}

static U8 OnAltitude(void)
{
    U8 mu8GetAltitudeConfig = GetAltidueConfig();
    if ( mu8GetAltitudeConfig == FALSE )
    {
        SetAltitudeConfig( TRUE );
        SetSpecialMode ( SPECIAL_HOT_ALTITUDE );
        return SOUND_SETUP;
    }
}

static U8 SelAltitude(void)
{
    if( GetAltidueConfig() == TRUE )
    {
        U8 mu8Altitude;

        mu8Altitude = GetHotWaterAltidue();
        mu8Altitude++;
        if( mu8Altitude >= ALTITUDE_LEVEL_NUM )
        {
            mu8Altitude = ALTITUDE_LEVEL_0;
        }
        SetHotWaterAltidue( mu8Altitude );
    }

    return SOUND_SELECT;
}

// TIME SHORT ?Œ?¤íŠ?ëª¨ë“œ
static U8 OnTimeShort(void)
{
    U8 mu8Ret;
    SpecialModeId_T mid;

    mu8Ret = IsExpiredTimeShortLimitTimer();
    if ( mu8Ret == TRUE )
    {
        return SOUND_NONE;
    }

    StartTimeShort();
    SetSpecialMode ( SPECIAL_NONE );

    return SOUND_SELECT;
}

static U8 OnTestShortCompTime(void)
{
#if CONFIG_TEST
    SetCompProtectOffTime(0);
#endif
    return SOUND_SELECT;
}

