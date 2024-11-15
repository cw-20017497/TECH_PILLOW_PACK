#include "display.h"
#include "process_display.h"
#include "hal_led.h"

#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "Temp.h"

#include "time_short.h"

#include "error.h"
#include "sound.h"
#include "eeprom.h"

#include "display_error.h"
#include "display_eol.h"





typedef struct _display_
{
    U8 Mode;       // NORMAL / VERSION / UART TEST / ...

    U8 Init;            /* FALSE : �ʱ�ȭ ��, TRUE : �ʱ�ȭ �Ϸ� */
    U8 Version;         /* ���� ǥ�� FLAG */
    U8 VersionOnTime;   /* ���� ǥ�� �ð� @100msec */

    U16 BlinkTime_5s;   /* LED ���� Ÿ�̸� 0.5sec */
    U16 BlinkTime_1s;   /* LED ���� Ÿ�̸� 1sec */
    U16 BlinkTime_4s;   /* LED ���� Ÿ�̸� 3sec on, 1sec off */

    /* �� ���� */
    U8 AltiConfig;      /* �� ���� ���� FLAG */

    /* �̻�� ���� */
    U8 UnusedSaveConfig;      /* �̻�� ����  ���� ���� FLAG */
    U16 UnusedSaveTimeOut;    /* �̻�� ����  ���� �ڵ� ���� �ð� Ÿ�̸� ( @100ms ) */

    /* ��� �ð� ���� */
    U8 AdjustHourConfig;
    U16 AdjustHourTimeOut;

    /* Memento */
    U8 Memento;         /* �޸��� ���� FLAG */
    U8 MementoStep;     /* �޸��� ǥ�� ���� STEP */
    U8 MementoTimer;    /* �޸��� ǥ�� ���� �ð� Ÿ�̸� */

    U16 ConfigTime;     /* ��� ���� ���� ���� Ÿ�̸� */
} Display_T;

Display_T   Disp;


static void BlinkLED( void (*pFun)(U8 mu8OnOff), U16 mu16Timer, U16 mu16Tick );
static void BlinkLEDReverse( void (*pFun)(U8 mu8OnOff), U16 mu16Timer, U16 mu16Tick );

static void DisplayHotTempLevel(void);
static void DisplayColdTempLevel(void);
static void DisplayWaterLevel(void);
static U8 DisplayAnimation ( void );
static void UpdateTimer(void);
static void ProcessDisplayBoot(void);
static void ProcessDisplayConfHotAltitude(void);
static void ProcessDisplayNormalMode(void);
static void ProcessDisplayErrorMode(void);
static void ProcessDisplayVersionMode(void);
static void ProcessDisplayMementoMode(void);
static void ProcessDisplayTimeShort(void);
static void ProcessDisplaySpecialMode(void);

void InitDisplay(void)
{
    //Disp.Mode = DISP_MODE_VERSION;
    Disp.BlinkTime_5s = 0;
    Disp.BlinkTime_1s = 0;
    Disp.BlinkTime_4s = 0;

    /* Booting Display - Showing Blink Front Display */
#if 1
    Disp.Init = FALSE;
#else
    Disp.Init = TRUE; 
#endif

    /* Hot Water Config Altitude */
    Disp.AltiConfig  = FALSE;
    
    /* Unused Save */
    Disp.UnusedSaveConfig = FALSE; 
    Disp.UnusedSaveTimeOut = 0;   

    /* ��� �ð� ���� */
    Disp.AdjustHourConfig  = FALSE;
    Disp.AdjustHourTimeOut = 0;

    /* Version */
#if 0
    Disp.Version = TRUE;
    Disp.VersionOnTime = 50;
#else
    Disp.Version = FALSE;
    Disp.VersionOnTime = 0;
#endif


    Disp.ConfigTime  = 300;
    /* TURN OFF ALL LED */
    TurnOffAllLED();
}

U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;
}


/* LED ���� ���� �Լ� 
 * Param
 *  - pFun : LED ���� �Լ� ������ �ּ�
 *  - mu16Timer : Ÿ�̸� ���� 
 *  - mu16Tick  : LED ���ҵ� �ϴ� ���� �ð� ��
 */
static void BlinkLED( void (*pFun)(U8 mu8OnOff), U16 mu16Timer, U16 mu16Tick )
{
    if( pFun == NULL )
    {
        return ;  // ���� ó��..
    }

    if( mu16Timer > mu16Tick ) 
    {
        pFun( ON );  
    }
    else
    {
        pFun( OFF );
    }
}
static void BlinkLEDReverse( void (*pFun)(U8 mu8OnOff), U16 mu16Timer, U16 mu16Tick )
{
    if( pFun == NULL )
    {
        return ;  // ���� ó��..
    }

    if( mu16Timer > mu16Tick ) 
    {
        pFun( OFF );  
    }
    else
    {
        pFun( ON );
    }
}




/* �¼� �� ���� */
U8 GetAltidueConfig(void)
{
    return Disp.AltiConfig;
}

void SetAltitudeConfig(U8 mu8Conf )
{
    Disp.AltiConfig = mu8Conf;
}

/* �¼� �̻�� ���� ���� */
U8 GetDispUnusedSaveConfig(void)
{
    return Disp.UnusedSaveConfig;
}

void SetDispUnusedSaveConfig(U8 mu8Conf )
{
    Disp.UnusedSaveConfig = mu8Conf;
    if ( mu8Conf == TRUE )
    {
        Disp.UnusedSaveTimeOut = 20;  // 2sec..
    }
}


/* ��� �ð� ���� */
U8 GetDispAdjustHourConfig(void)
{
    return Disp.AdjustHourConfig;
}

void SetDispAdjustHourConfig(U8 mu8Conf )
{
    Disp.AdjustHourConfig = mu8Conf;
    if ( mu8Conf == TRUE )
    {
        Disp.AdjustHourTimeOut = 50;  // 5sec..
    }
}



U16 IsExpiredConfigTime(void)
{
    if( Disp.ConfigTime != 0 )
    {
        return FALSE;   // ���� �ȵ�.
    }

    return TRUE;  // �����.
}

/* ���� ǥ�� */
void SetVersionDisp(U8 mu8DispSec)
{
    Disp.Version = TRUE;
    Disp.VersionOnTime = mu8DispSec;
}

/* �޸��� ���� */
U8 GetMementoDisp(void)
{
    return Disp.Memento;
}

void SetMementoDisp(U8 mu8Conf )
{
    Disp.Memento        = mu8Conf;
    Disp.MementoStep    = 0;
    Disp.MementoTimer   = 0;
}

void ProcessDisplay(void)
{
    UpdateTimer();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        return ;
    }

    /* DISPLAY VERSION */
    if( Disp.Version == TRUE )
    {
        ProcessDisplayVersionMode();
        return ;
    }

    /* TEST - EOL */
    if( GetEolStatus() == TRUE )
    {
        ProcessDisplayEol();
        return ;
    }

    /* MEMENTO */
    if( Disp.Memento == TRUE )
    {
        ProcessDisplayMementoMode();
        return ;
    }

    /* ERRROR MODE */
    if( GetErrorId() != ERR_NONE )
    {
        ProcessDisplayErrorMode();
        return ;
    }

  

    /* TIME SHORT */
    if( GetTimeShortStatus() == TRUE )
    {
        ProcessDisplayTimeShort();
        return ;
    }

    /* NORMAL MODE */
    ProcessDisplayNormalMode();
}

/* Ÿ�̸� ���� �Լ�  @100ms
 */
static void UpdateTimer(void)
{
    /* Update Blink Timer */
    if( Disp.BlinkTime_5s == 0 )
    {
        Disp.BlinkTime_5s = 6;
    }
    Disp.BlinkTime_5s--;

    if( Disp.BlinkTime_1s == 0 )
    {
        Disp.BlinkTime_1s = 11;
    }
    Disp.BlinkTime_1s--;

    if( Disp.BlinkTime_4s == 0 )
    {
        Disp.BlinkTime_4s = 41;
    }
    Disp.BlinkTime_4s--;


    if( Disp.ConfigTime != 0 )
    {
        Disp.ConfigTime--;
    }

    if( Disp.VersionOnTime != 0 )
    {
        Disp.VersionOnTime--;
    }
}

/* ���÷��� �ʱ�ȭ ǥ�� */
static void ProcessDisplayBoot(void)
{
    static U8 mu8Count = 5;
    static U8 mu8Toggle = 0;

    if( Disp.BlinkTime_5s == 0 )
    {
        mu8Toggle = !mu8Toggle;

        mu8Count--;
        if( mu8Count == 0 )
        {
            mu8Count = 5;
            Disp.Init = TRUE;
            return;
        }
    }

    if( mu8Toggle == 0 )
    {
        TurnOffAllLED();
    }
    else
    {
        TurnOnAllLED();
    }
}


static void ProcessDisplaySpecialMode(void)
{
    BlinkLED( DispHotWaterWeak, Disp.BlinkTime_1s, 5 );
    BlinkLED( DispColdWaterWeak, Disp.BlinkTime_1s, 5 );
    BlinkLEDReverse( DispHotWaterStrong, Disp.BlinkTime_1s, 5 );
    BlinkLEDReverse( DispColdWaterStrong, Disp.BlinkTime_1s, 5 );
}

/* ���÷��� �¼� �� ���� */
static void ProcessDisplayConfHotAltitude(void)
{
    U8 mu8Altitude;


    /* 1. SETUP ALTITUDE */
    mu8Altitude = GetHotWaterAltidue();
    TurnOffAllLED();	

    switch( mu8Altitude )
    {
        case ALTITUDE_LEVEL_0 :  
            // �¼� �� ����
            BlinkLED( DispHotWaterWeak, Disp.BlinkTime_5s, 3 );
            break;

        case ALTITUDE_LEVEL_1 :  
            // �¼� �� ����
            BlinkLED( DispHotWaterStrong, Disp.BlinkTime_5s, 3 );
            break;

        case ALTITUDE_LEVEL_2 :  
            // �¼� ��, �� ����
            BlinkLED( DispHotWaterWeak, Disp.BlinkTime_5s, 3 );
            BlinkLED( DispHotWaterStrong, Disp.BlinkTime_5s, 3 );
            break;

        case ALTITUDE_LEVEL_3 :  
            // ��ü OFF
            TurnOffAllLED();
            break;

        default : 
            TurnOffAllLED();
            break;
    }
}




static void ProcessDisplayVersionMode(void)
{
    U8 mu8VersionBit_1;
    U8 mu8VersionBit_2;
    U8 mu8Remainder;        //������

    if( Disp.VersionOnTime == 0 )
    {
        Disp.Version = FALSE;
    }

    /* 1. TURN OFF ALL LED */
    TurnOffAllLED();	

    /* 2. VERSION */
    mu8Remainder = VERSION % 3;
    if ( mu8Remainder == 0 )
    {
        mu8Remainder = 0x03;
    }

    mu8VersionBit_1 = ( mu8Remainder & 0x01 );
    mu8VersionBit_2 = ( mu8Remainder & 0x02 );

    if ( mu8VersionBit_1 == 0x01 )
    {
        BlinkLED( DispColdWaterWeak, Disp.BlinkTime_1s, 5 );
    }
    
    if ( mu8VersionBit_2 == 0x02 )
    {
        BlinkLED( DispColdWaterStrong, Disp.BlinkTime_1s, 5 );
    }
    
}


static void DispMementoId(U8 mu8Id, SoundId_T mSoundId)
{
    Memento_T  mMemento;

    GetMementoError( mu8Id, &mMemento );
    if( mMemento.Error == ERR_NONE )
    {
        Disp.MementoStep = 0;

    }
    else
    {
        DisplayError( mMemento.Error, ON );
        Sound( mSoundId );
        Disp.MementoTimer = 50;

        Disp.MementoStep++;
    }
}

static void ProcessDisplayMementoMode(void)
{

    if( Disp.MementoTimer != 0 )
    {
        Disp.MementoTimer--;
        return ;
    }

    // 1. ALL ON LED - 1 SEC
    // 2. ALL OFF LED - 1 SEC
    // 3. DISPLAY ERROR 5SEC & SOUND 1~5
    // 3. DISPLAY ERROR 5SEC & SOUND 1
    switch( Disp.MementoStep )
    {
        case 0 : 
            TurnOnAllLED();	
            Disp.MementoTimer = 10;

            Disp.MementoStep++;
            break;

        case 1:
            TurnOffAllLED();
            Disp.MementoTimer = 10;

            Disp.MementoStep++;
            break;

        case 2:
            DispMementoId( 4, SOUND_MEMENTO_1 );
            break;

        case 3:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 4:
            DispMementoId( 3, SOUND_MEMENTO_2 );

            break;

        case 5:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 6:
            DispMementoId( 2, SOUND_MEMENTO_3 );
            break;

        case 7:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 8:
            DispMementoId( 1, SOUND_MEMENTO_4 );
            break;

        case 9:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 10:
            DispMementoId( 0, SOUND_MEMENTO_5 );
            break;

        case 11:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep = 0;
            break;

    }

    return ;
}


static void ProcessDisplayErrorMode(void)
{
    static ErrorId_T mu8PrevErr = 0;
    ErrorId_T mu8CurErr = 0;

    static U8 mu8Cmd = 0;
    static U8 mu8Counter = 10;


    /* Get error code */
    mu8CurErr = GetErrorId();

    /* Display error code */
    if( mu8CurErr == ERR_NONE )
    {
        mu8Cmd = 0;
        mu8Counter = 10;            /* 1sec */
    }
    else
    {
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }

        /* 0.5sec �ֱ� ���� */
        if( mu8Counter <= 5 )
        {
            mu8Cmd = 0;
        }
        else
        {
            mu8Cmd = 1;
        }

        DisplayError( mu8CurErr, mu8Cmd );
    }

    /* Sound new error code */
    if( mu8PrevErr != mu8CurErr )
    {
        mu8PrevErr = mu8CurErr;
    }
}


static void ProcessDisplayNormalMode(void)
{
    /* Water Configure - HOT / COLD */
    DisplayHotTempLevel();
    DisplayColdTempLevel();
}

static void ProcessDisplayTimeShort(void)
{
    static U8 mu8Step = 0;
    U8 mu8Ret;
    
    switch ( mu8Step )
    {
        // ��ü LED ��������
        case 0:
            mu8Ret = DisplayAnimation();
            if ( mu8Ret == TRUE )
            {
                mu8Step++;
            }
        break;

        //���� 2ȸ ����
        case 1:
            //ProcessDisplayVersion()���� ó��
            mu8Step++;
        break;

        //���� ǥ��
        case 2:
            DisplayHotTempLevel();
            DisplayWaterLevel();
        break;
    }
}

static void DisplayHotTempLevel(void)
{
    TEMP_T  mTempHotOn;             //Hot LED ON Target
    TEMP_T  mTempHotOff;            //Hot LED OFF Target
    TEMP_T  mTempHotCurrent;        //Hot Current Temp

    U8 mu8HotAltitude;
    mu8HotAltitude = GetHotWaterAltidue();
    
    mTempHotOn = GetTargetHotOnTemp( mu8HotAltitude );
    mTempHotOff = GetTargetHotOffTemp( mu8HotAltitude );
    mTempHotCurrent = GetTemp( TEMP_ID_HOT_WATER );

    /* HOT WATER TEMP LEVEL */
    if( GetHotWaterConfigMake() == ON ) 
    {
        DispHotWaterWeak( ON );

        if ( mTempHotCurrent > mTempHotOff )
        {
            DispHotWaterStrong( ON );
        }
        else if ( mTempHotCurrent < mTempHotOn )
        {
            DispHotWaterStrong( OFF );
        }
        else
        {
            //���� ���� ����
        }
    }
    else  
    {
        DispHotWaterWeak( OFF );
        DispHotWaterStrong( OFF );
    }
}

static void DisplayColdTempLevel(void)
{
    TEMP_T  mTempColdOn;            //Cold LED ON Target
    TEMP_T  mTempColdOff;           //Cold LED OFF Target
    TEMP_T  mTempColdCurrent;       //Cold Current Temp

    mTempColdOn = GetTargetColdOnTemp();
    mTempColdOff = GetTargetColdOffTemp();
    mTempColdCurrent = GetTemp( TEMP_ID_COLD_WATER );

    /* COLD WATER TEMP LEVEL */
    if( GetColdWaterConfigMake() == ON ) 
    {
        DispColdWaterWeak( ON );

        if ( mTempColdCurrent < mTempColdOff )
        {
            DispColdWaterStrong( ON );
        }
        else if ( mTempColdCurrent > mTempColdOn )
        {
            DispColdWaterStrong( OFF );
        }
        else
        {
            //���� ���� ����
        }
    }
    else 
    {
        DispColdWaterWeak( OFF );
        DispColdWaterStrong( OFF );
    }

}

static void DisplayWaterLevel(void)
{
    U8 mu8Level;
    
    mu8Level = GetRoomWaterLevel();

    if ( mu8Level == LEVEL_HIGH )
    {
        DispColdWaterWeak( ON );
        DispColdWaterStrong( ON );
    }
    else if ( mu8Level == LEVEL_MID )
    {
        DispColdWaterWeak( ON );
        DispColdWaterStrong( OFF );
    }
    else //if ( mu8Level == LEVEL_LOW )
    {
        DispColdWaterWeak( OFF );
        DispColdWaterStrong( OFF );
    }

}

static U8 DisplayAnimation ( void )
{
    static U16 mu16Count = 12;      /* 1.2sec */
    U8 mu8Ret = FALSE;

    if ( mu16Count > 9 )
    {
        DispHotWaterWeak( OFF );
        DispHotWaterStrong( OFF );
        DispColdWaterStrong ( OFF );
        DispColdWaterWeak( ON );
    }
    else if ( mu16Count > 6 )
    {
        DispHotWaterWeak( OFF );
        DispHotWaterStrong( OFF );
        DispColdWaterStrong( ON );
        DispColdWaterWeak( OFF );
    }
    else if ( mu16Count > 3 )
    {
        DispHotWaterWeak( ON );
        DispHotWaterStrong( OFF );
        DispColdWaterStrong( OFF );
        DispColdWaterWeak( OFF );
    }
    else if ( mu16Count > 0 )
    {
        DispHotWaterWeak( OFF );
        DispHotWaterStrong( ON );
        DispColdWaterStrong( OFF );
        DispColdWaterWeak( OFF );
    }
    else
    {
        SetVersionDisp ( 20 );
    }

    if ( mu16Count != 0 )
    {
        mu16Count--;
        mu8Ret = FALSE;
    }
    else
    {
        mu8Ret = TRUE;
    }

    return mu8Ret;
}


