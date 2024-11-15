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
/* �¼� ��ư */
static U8 OnHotMake(void);
static U8 OffHotMake(void);
static U8 OnOffHotMake(void);

/* �ü� ��ư */
static U8 OnColdMake(void);
static U8 OffColdMake(void);
static U8 OnOffColdMake(void);

/* �ü� ��ư */
static U8 ClearMemento(void);


static U8 OnVersion(void);
static U8 OnOffMemento(void);

/* ?��i?? e��??? ?��i?? */
static U8 SelSpecial(void);



/* EEPROM i��?e����i�⡱ */
static U8 OnEraseEeprom(void);


/* EOL */
//static U8 OnEolStart(void);
static U8 OnFrontStart(void);
static U8 OnFctStart(void);


// ?��e? e����e���� e|��i?��i??
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

// �׽�Ʈ ���(TIME SHORT) ����Ʈ
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

/* �¼� ���� On */
static U8 OnHotMake(void)
{
    // �� ���� ��� ������ Ű �Է� ��� ����
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

/* �¼� ���� Off */
static U8 OffHotMake(void)
{
    // �� ���� ��� ������ Ű �Է� ��� ����
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


/* �ü� ���� On */
static U8 OnColdMake(void)
{
    if( GetColdWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    // �ð� ON�ϰ�, ��� �ü� �µ� �е��� ��.
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





/* e��??? ?��?�� */
static U8 OnVersion(void)
{
    SetVersionDisp( 30 );

    return SOUND_SELECT;
}

/* �޸��� ǥ�� */
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



// EOL �˻� ����
static U8 OnEolStart(void)
{
    // EOL �׽�Ʈ ���� �ð��� ����Ǹ�,
    // ������ ���� Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // ���� EOL �׽�Ʈ ���̸�, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // EOL ����
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp( 50 );

    return SOUND_SETUP;
}

// FRONT �˻� ����
static U8 OnFrontStart(void)
{
    // EOL �׽�Ʈ ���� �ð��� ����Ǹ�,
    // ������ ���� Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // ���� EOL �׽�Ʈ ���̸�, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // EOL ����
    StartEol( EOL_TYPE_FRONT );
    StartDisplayInit();
    SetVersionDisp( 50 );

    return SOUND_SETUP;
}





// FCT e����???��?��
static U8 OnFctStart(void)
{
    // EOL �׽�Ʈ ���� �ð��� ����Ǹ�,
    // ������ ���� Pass
    if( IsExpiredFctLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }


    // FCT ����
    StartFct();
    return SOUND_SELECT;
}



