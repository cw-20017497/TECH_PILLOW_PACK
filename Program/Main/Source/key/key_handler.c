#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "hot_water.h"
#include "cold_water.h"
#include "room_water.h"
#include "Temp.h"
#include "sound.h"
#include "eeprom.h"
#include "error.h"
#include "process_display.h"
#include "eol.h"
#include "fct.h"
#include "time_short.h"
#include "timer.h"


#if CONFIG_TEST
#include "comp.h"
#endif


static U8 ClearOneShot(void);
/* 온수 버튼 */
static U8 OnHotMake(void);
static U8 OffHotMake(void);
static U8 OnOffHotMake(void);

/* 냉수 버튼 */
static U8 OnColdMake(void);
static U8 OffColdMake(void);
static U8 OnOffColdMake(void);

/* 냉수 버튼 */
static U8 ClearMemento(void);


static U8 OnVersion(void);
static U8 OnOffMemento(void);

/* ?¨i?? e³??? ?¤i?? */
static U8 SelSpecial(void);



/* EEPROM i´?e¸°i™” */
static U8 OnEraseEeprom(void);


/* EOL */
//static U8 OnEolStart(void);
static U8 OnFrontStart(void);
static U8 OnFctStart(void);


// ?¼e? eª¨e“œ e|￢i?¤i??
KeyEventList_T KeyEventList[] =
{
    /* KEY,            Short,      2sec,            3sec,       5sec,           Pop,            1sec */
    /* SINGLE KEY */
    { K_HOT,            NULL,      NULL,            OnHotMake,  OffHotMake,     ClearOneShot,   NULL },
    { K_COLD,           NULL,      NULL,            OnColdMake, OffColdMake,    ClearOneShot,   NULL },

    /* MULTI KEY */
  //  { K_SPECIAL,        NULL,      NULL,            NULL,       NULL,           NULL,           SelSpecial }
    
    //{ K_TEST_TS,        NULL,      OnTimeShort,     NULL,       NULL,           NULL,  NULL },
    //{ K_TEST_EOL,       NULL,      OnEolStart,           NULL, NULL,        NULL,   NULL }
    //{ K_UNUSED_SAVE,    NULL,      OnOffUnusedSave,      NULL,  NULL,        NULL,  NULL },
    //{ K_ERASE_EEPROM,   NULL,      OnEraseEeprom,        NULL,  NULL,        NULL,  NULL },
};

// 테스트 모드(TIME SHORT) 리스트
KeyEventList_T KeyEventTimeShortList[] =
{
    /* KEY,            Short,    2sec,      3sec,       5sec,        Pop,      TS */
    /* SINGLE KEY */
    { K_HOT,            OnOffHotMake,       NULL,      NULL,       NULL,        NULL, },
    { K_COLD,           OnOffColdMake,      NULL,      NULL,       NULL,        NULL, },
};

U8 IsValidKeyCondition(U16 mu16Key)
{
    /* CONFIG HOT ALTITUDE */
    if( GetAltidueConfig() == TRUE )
    {
        if( mu16Key != K_HOT )
        {
            return FALSE;
        }
    }

#if 0
    /* MEMENTO DISPLAY */
    if( GetMementoDisp() == TRUE )
    {
        if( mu16Key != K_MEMENTO 
                && mu16Key != K_ROOM )
        {
            return FALSE;
        }
    }
#endif

    return TRUE;
}

void* GetKeyEventList(void)
{
    if( GetTimeShortStatus() == TRUE )
    {
        return KeyEventTimeShortList;
    }

    return KeyEventList;
}

U8 GetKeyEventListSize(void)
{
    if( GetTimeShortStatus() == TRUE )
    {
        return ( sizeof(KeyEventTimeShortList) / sizeof(KeyEventList_T) );
    }

    return ( sizeof(KeyEventList) / sizeof(KeyEventList_T) );
}

U8 gu8OneShotOnOff;
static U8 ClearOneShot (void)
{
    gu8OneShotOnOff = FALSE;

    return SOUND_NONE;
}

/* 온수 생성 On */
static U8 OnHotMake(void)
{
    // 고도 설정 모드 에서는 키 입력 허용 금지
    if( GetAltidueConfig() == TRUE )
    {
        return SOUND_NONE;
    }

    if( GetHotWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    SetHotWaterConfigMake( TRUE );
    gu8OneShotOnOff = TRUE;
    return SOUND_CONFIG_SETUP;
}

/* 온수 생성 Off */
static U8 OffHotMake(void)
{
    // 고도 설정 모드 에서는 키 입력 허용 금지
    if( GetAltidueConfig() == TRUE )
    {
        return SOUND_NONE;
    }

    if( GetHotWaterConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if ( gu8OneShotOnOff == TRUE )
    {
        gu8OneShotOnOff = FALSE;
        return SOUND_NONE;
    }

    SetHotWaterConfigMake( FALSE );
    return SOUND_CONFIG_CANCEL;
}

static U8 OnOffHotMake(void)
{
    if( GetHotWaterConfigMake() == FALSE )
    {
        SetHotWaterConfigMake( TRUE );
    }
    else
    {
        SetHotWaterConfigMake( FALSE );
    }

    return SOUND_NONE;
}


/* 냉수 생성 On */
static U8 OnColdMake(void)
{
    if( GetColdWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    // 냉각 ON하고, 즉시 냉수 온도 읽도록 함.
    SetColdWaterConfigMake( TRUE );
    gu8OneShotOnOff = TRUE;
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );

    return SOUND_CONFIG_SETUP;
}

static U8 OffColdMake(void)
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if ( gu8OneShotOnOff == TRUE )
    {
        gu8OneShotOnOff = FALSE;
        return SOUND_NONE;
    }

    SetColdWaterConfigMake( FALSE );
    return SOUND_CONFIG_CANCEL;
}

static U8 OnOffColdMake(void)
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        SetColdWaterConfigMake( TRUE );
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }
    else
    {
        SetColdWaterConfigMake( FALSE );
    }

    return SOUND_NONE;
}



static U8 ClearMemento(void)
{
    /* MEMENTO CLEAR */
    if( GetMementoDisp() == TRUE )
    {
        ClearMementoError();

        return SOUND_SELECT;
    }

    return SOUND_NONE;
}





/* e²??? ?œ?œ */
static U8 OnVersion(void)
{
    SetVersionDisp( 30 );

    return SOUND_SELECT;
}

/* 메멘토 표시 */
static U8 OnOffMemento(void)
{
    if( GetMementoDisp() == FALSE )
    {
        SetMementoDisp( TRUE );
    }
    else
    {
        SetMementoDisp( FALSE );
    }

    return SOUND_SELECT;
}

static U8 SelSpecial(void)
{
#if 0
    U8 mu8Ret;
    SpecialModeId_T mid;

    mu8Ret = IsExpiredSepcialModeLimitTimer();
    if ( mu8Ret == TRUE )
    {
        return SOUND_NONE;
    }

    SetSpecialMode ( SPECIAL_WAIT );
#endif
    return SOUND_CONFIG_SETUP;    
}





static U8 OnEraseEeprom(void)
{
    if( IsExpiredConfigTime() == FALSE )
    {
        EraseEeprom();
        return SOUND_SELECT;
    }

    return SOUND_NONE;
}



// EOL 검사 시작
static U8 OnEolStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // EOL 시작
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp( 50 );

    return SOUND_SETUP;
}

// FRONT 검사 시작
static U8 OnFrontStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // EOL 시작
    StartEol( EOL_TYPE_FRONT );
    StartDisplayInit();
    SetVersionDisp( 50 );

    return SOUND_SETUP;
}





// FCT e²€???œ?‘
static U8 OnFctStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredFctLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }


    // FCT 시작
    StartFct();
    return SOUND_SELECT;
}



