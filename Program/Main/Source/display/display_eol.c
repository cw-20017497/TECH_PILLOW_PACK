#include "hw.h"
#include "display_eol.h"
#include "display.h"


static void EolCheckInput(void)
{
#if 0
    U32 mu32Val;

    // Turn off all leds
    TurnOffAllLED();

    mu32Val = GetEolTestInputVal();

    if( (mu32Val & MK_EOL_PHOTO_SENSOR ) != 0 )
    {
        DispIceFull( ON );
    }

    if( (mu32Val & MK_EOL_KEY_HOT ) != 0 )
    {
        DispHotWater( ON );
        DispHotTemp_1( ON );
        DispHotTemp_2( ON );
        DispHotTemp_3( ON );
    }

    if( (mu32Val & MK_EOL_KEY_ROOM ) != 0 )
    {
        DispRoomWater( ON );
    }

    if( (mu32Val & MK_EOL_KEY_COLD ) != 0 )
    {
        DispColdWater( ON );
        DispColdTemp_1( ON );
        DispColdTemp_2( ON );
        DispColdTemp_3( ON );
    }

    if( (mu32Val & MK_EOL_KEY_CONTY ) != 0 )
    {
        DispConty( ON );
        DispConty1L( ON );
        DispConty1D5L( ON );
    }

    if( (mu32Val & MK_EOL_KEY_ICE_MAKE ) != 0 )
    {
        DispIce( ON );
        DispIceMaker( ON );
    }
    else
    {
        DispIceMaker( OFF );
    }

    if( (mu32Val & MK_EOL_KEY_ICE_TURBO ) != 0 )
    {
        DispIceTurbo( ON );
    }

    if( (mu32Val & MK_EOL_KEY_ICE_LOCK ) != 0 )
    {
        DispLockIce( ON );
    }

    if( (mu32Val & MK_EOL_KEY_STER ) != 0 )
    {
        DispSter( ON );
        DispDrain( ON );
        DispSterProgressBar( 100 );
    }
    else
    {
        DispSterProgressBar( 0 );
    }

    if( (mu32Val & MK_EOL_KEY_HOT_LOCK ) != 0 )
    {
        DispLockHot( ON );
    }

    if( (mu32Val & MK_EOL_KEY_SAVE ) != 0 )
    {
        DispPowerSaving( ON );
    }

    if( (mu32Val & MK_EOL_LEVER_WATER ) != 0 )
    {
        DispWaterOut( ON );
    }
    else
    {
        DispWaterOut( OFF );
    }

    if( (mu32Val & MK_EOL_LEVER_ICE ) != 0 )
    {
        DispIceOut( ON );
    }
    else
    {
        DispIceOut( OFF );
    }

    if( (mu32Val & MK_EOL_TANK_OPEN ) != 0 )
    {
        DispService( ON );
    }
#endif
}

// FRONT 검사 DISPLAY
// 1. 키 버튼 입력,
// 2. 조도 센서 감지
// 3. 서비스 점검 
static void DisplayEolFrontTest(void)
{
#if 0  // 통신 기능 삭제
    // 만약 버튼 EOL TEST FLAG가 SET되어 있으면,
    // EOL 통신 상태만 표시 하기위해 모든 LED를 다시 OFF한다.
    if( GetEolTestInputVal() == MK_EOL_ALL )
    {
        TurnOffAllLED();

        DispIceOut( OFF );
        DispWaterOut( OFF );
        DispIceMaker( OFF );
        DispSterProgressBar( 0 );
    }
    else
    {
        EolCheckInput();
    }

    // Comm error
    if( GetEolErrorComm() == TRUE )
    {
        DispIceFull( ON );
    }
    else
    {
        static U8 mu8Toggle = 0;

        if(Disp.BlinkTime_5s == 0 )
        {
            mu8Toggle = !mu8Toggle;
        }

        if( mu8Toggle == 0 )
        {
            DispIceFull( ON );
        }
        else
        {
            DispIceFull( OFF );
        }
    }
#else
    EolCheckInput();

    // NFC Error
    // 일반 모델 + 일반 FRONT = OK
    // 일반 모델 + 살균 FRONT = ERROR
    // 살균 모델 + 일반 FROTN = ERROR
    // 살균 모델 + 살균 FRONT = OK
    
#if 0
    if( GetEolErrorNFC() == FALSE )
    {
        static U8 mu8Toggle = 0;
        static U8 mu8BlinkTime = 11;

        if(mu8BlinkTime == 0 )
        {
            mu8BlinkTime = 11;
            mu8Toggle = !mu8Toggle;
        }
        mu8BlinkTime--;

        if( GetEolErrorNFC() == FALSE )
        {
            if( mu8Toggle == 0 )
            {
                DispSter( ON );
                DispDrain( ON );
            }
            else
            {
                DispSter( OFF );
                DispDrain( OFF );
            }
        }
    }
#endif
#endif
}

static void DisplayEolLoadTest(void)
{
#if 0

    // 만약 버튼 EOL TEST FLAG가 SET되어 있으면,
    // EOL 통신 상태만 표시 하기위해 모든 LED를 다시 OFF한다.
    if( GetEolTestInputVal() == MK_EOL_ALL )
    {
        TurnOffAllLED();

        DispIceOut( OFF );
        DispWaterOut( OFF );
        DispIceMaker( OFF );
        DispSterProgressBar( 0 );
    }
    else
    {
        if( GetWaterOutSelect() == SEL_WATER_HOT )
        {
            SetEolTestInputBit( MK_EOL_KEY_HOT );
            ClearEolTestInputBit( MK_EOL_KEY_ROOM );
            ClearEolTestInputBit( MK_EOL_KEY_COLD );
        }
        else if( GetWaterOutSelect() == SEL_WATER_ROOM )
        {
            ClearEolTestInputBit( MK_EOL_KEY_HOT );
            SetEolTestInputBit( MK_EOL_KEY_ROOM );
            ClearEolTestInputBit( MK_EOL_KEY_COLD );
        }
        else 
        {
            ClearEolTestInputBit( MK_EOL_KEY_HOT );
            ClearEolTestInputBit( MK_EOL_KEY_ROOM );
            SetEolTestInputBit( MK_EOL_KEY_COLD );
        }
        EolCheckInput();
    }
#endif
}


void ProcessDisplayEol(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        // FRONT 검사 모드
        DisplayEolFrontTest();
    }
    else
    {
        // LOAD 검사 모드
        DisplayEolLoadTest();
    }
}
