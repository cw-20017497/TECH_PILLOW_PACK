#include "hw.h"
#include "process_eol.h"
#include "eol.h"


#include "key.h"
#include "eeprom.h"
#include "util.h"
#include "timer.h"

#include "valve.h"
#include "relay.h"
#include "temp.h"
#include "hal_adc.h"
#include "level.h"
#include "cold_water.h"
#include "hot_water.h"


typedef struct _eol_ice_system_
{
    U8 Start;       // ?ú?ë, ?ïÏßÄ
    U8 Status;      // ?ÑÎ£å(TRUE), Í∑∏Î†áÏßÄ ?ä?ºÎ©¥ FALSE
    U8 Step;

    U16 WaitTime;       // @100ms
} EolIceSys_T;

EolIceSys_T   EolIce;   // ?âÎß??†Î°ú ?ú?§ÌÖú Í≤Ä??



static void CheckEeprom(void)
{
    static U8 mu8Init = FALSE;
    static U8 mu8RetryCount = 5;
    U8 mu8Ret = FALSE;


    if( mu8Init == TRUE )
    {
        return ;
    }

    // Check eeprom
    mu8Ret = TestEeprom();
    if( mu8Ret == TRUE )
    {
        mu8Init = TRUE;

        SetEolCheckStatus( EOL_CHK_ID_EEPROM, 0xA5 );  // OK
    }
    else
    {
        SetEolCheckStatus( EOL_CHK_ID_EEPROM, 0x01 );  // ERROR
    }

    if( --mu8RetryCount == 0 )
    {
        mu8Init = TRUE;
    }
}

static void CheckSensor(void)
{
    CheckEeprom();
}


// ?ºÏùå ?ºÎçî ?§ÌÅ¨Î? ?ô?ë.
// ?Ñ?¥Ïä§ÎèÑ ?Ñ??OPEN/CLOSE Î∞òÎ≥??ô?ë.

void StartEolIceSystem(void)
{
    EolIce.Start = TRUE;
}


static void InnerProcessFront(void)
{
    // EEPROM
    CheckEeprom();

    // Visual Test

    return ;
}

static void ModeReady(void)
{
    // Check EOL Input 
    CheckSensor();
}

static void ModeInit(void)
{
    TurnOffRelay( RELAY_COMP );
    TurnOffRelay( RELAY_HEATER );

    CloseValve( VALVE_ALL );
    OpenValve( VALVE_NOS );

}

static void InnerProcessLoad(void)
{
    U8 mu8Mode;

    mu8Mode = GetEolMode();
    switch( mu8Mode )
    {
        case EOL_MODE_READY:
            ModeReady();

            SetEolMode( EOL_MODE_INIT );

            break;

        case EOL_MODE_INIT:

            // ?µÏã† ?ú???ÄÍ∏?
            ModeInit();

            break;

        case EOL_MODE_CHECK_SENSOR:

            break;

        case EOL_MODE_CHECK_LOAD:
            break;

        case EOL_MODE_CHECK_VACCUM:
        case EOL_MODE_CHECK_GAS:
            break;

        case EOL_MODE_DONE:
            break;

        default:
            break;
    }

}

#define PROTECT_TIME        50      //@100msec
static void EolProtectHeater ( void )
{
    static U8 HeaterEx = OFF;
    static U8 HeaterCur = OFF;
    static U8 HeaterTime = 0;

    HeaterCur = IsTurnOnRelay( RELAY_HEATER );

    if ( HeaterEx != HeaterCur )
    {
        HeaterEx = HeaterCur;
        HeaterTime = PROTECT_TIME;
    }
    else 
    {
        if ( HeaterCur == ON )
        {
            if ( HeaterTime != 0 )
            {
                HeaterTime--;
            }
            else
            {
                TurnOffRelay( RELAY_HEATER );
            }
        }
        else
        {
            TurnOffRelay( RELAY_HEATER );
        }
    }

}

void ProcessEol(void)
{
    // check expired test time out
    if( GetEolTestTimer() == 0 )
    {
        Reset();
        return ;
    }


    if( GetEolType() == EOL_TYPE_FRONT )
    {
        InnerProcessFront();
    }
    else
    {
        InnerProcessLoad();
        EolProtectHeater();
    }
}


