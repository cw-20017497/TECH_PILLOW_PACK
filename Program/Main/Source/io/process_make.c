#include "process_make.h"
#include "temp.h"

#include "comp.h"
#include "cold_water.h"

#include "heater.h"
#include "hot_water.h"

void InitProcessMake(void)
{
    InitHeater();
    
    InitComp();
}

/* 온수 생성 제어 */
static void ProcessMakeHotWater(void)
{
    U8 mu8OnOff = OFF;

    /* MAKE ON/OFF */
    if( GetHotWaterMake() == TRUE )
    {
        mu8OnOff = ON;
    }
    ControlHeater( mu8OnOff );
}

/* 냉각 운전 상세 모드 결정 */
static void MakeColdWaterMode(void)
{
    static U8 mu8PrevMake = FALSE;
    U8 mu8Make;
    U8 mu8Mode;


    mu8Make = GetColdWaterMake();         // 냉각 운전 상태
    mu8Mode = GetColdWaterMakeMode();     // 냉각 운전 상세 모드

    // 냉각 운전 상태가 변할 때에 운전할지 정지할지 결정 한다.
    // 만약, 운전 조건인데 운전 모드가 없으면 강제 운전 시킨다 
    if( mu8PrevMake != mu8Make )
    {
        // 정지 -> 운전 
        if( mu8Make == TRUE )
        {
            if( mu8Mode == COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
            }
        }
        // 운전 -> 정지
        else 
        {
            if( mu8Mode != COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_DONE );
            }
        }

        mu8PrevMake = mu8Make;
    }
    else if( mu8Make == TRUE )
    {
        // 냉각 모드에서 냉각중이 아니라면, 냉각을 시작한다.
        if( mu8Mode == COLD_MODE_NONE )
        {
            SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
        }
    }
}

// 냉각 제어 ( 초기화, 냉각, 완료 )
static void ProcessMakeColdWater(void)
{
    /* 냉수 초기화 
     *  -. 냉수 순환 펌프 중지
     *  -. 냉매 밸브 전환...
     *
     */
    if( IsSetColdWaterMakeMode( COLD_MODE_INIT ) == TRUE )
    {
        // 냉각 초기화
        if( IsExpiredCompProtectTime() == TRUE )
        {
            ClearColdWaterMakeMode( COLD_MODE_INIT );
        }
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_DONE ) == TRUE )
    {
        // 냉각 완료
        // 압축기 OFF
        TurnOffComp(COMP_TYPE_COLD);

        ClearColdWaterMakeMode( COLD_MODE_COOLING );
        ClearColdWaterMakeMode( COLD_MODE_DONE );
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_COOLING ) == TRUE )
    {
        // 압축기 ON
        TurnOnComp(COMP_TYPE_COLD);
    }
    else 
    {
        TurnOffComp(COMP_TYPE_COLD);
    }
}

void ProcessMake(void)
{
    /* Make Mode COLD */
    MakeColdWaterMode();      // 냉각 Make Mode 결정

    /* Process Cold Water */
    ProcessMakeColdWater();   // 냉각 MakeMode Handler


    /* Process Compressor */
    ProcessComp();            // 압축기 제어

   /* Process Make Hot Water */
    ProcessMakeHotWater();    // 온수 히터 제어
}
