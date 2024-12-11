#ifndef __HOT_WATER_H__
#define __HOT_WATER_H__

#include "prj_type.h"
#include "level.h"

/* ALTITUDE */
#define  ALTITUDE_LEVEL_0    0
#define  ALTITUDE_LEVEL_1    1
#define  ALTITUDE_LEVEL_2    2
#define  ALTITUDE_LEVEL_3    3
#define  ALTITUDE_LEVEL_NUM  4


/* UNUSED SAVING */
#define  UNUSED_SAVING_ON_10HOUR    36000U // @1sec 
#define  UNUSED_SAVING_ON_6HOUR     21600U // @1sec
#define  UNUSED_SAVING_ON_9HOUR     32400U // @1sec


typedef struct _hot_water_unused_
{
    /* Unused Power Saving */
    U8 Config;    // 미사용 절전 사용 설정
    U8 Run;          // 현재 동작 상태

    U16 OnTime;   // 미사용 절전 진입 시간 @min 
    U16 RunTime;  // 미사용 절전 동작 시간 @min

    U16 TempStatus;     // 외기 온도 상태 ( UNDER_TEMP, OVER_TEMP )
    U16 TempOverTime;   // Over 조건에 대한 지연 시간
    U16 TempUnderTime;  // Under 조건에 대한 지연 시간

} UnusedSaving_T;

typedef struct _hot_water_
{
    U8 InitWaitTime;    // 최초 대기 시간
    U8 InitFull;        // 탱크 최초 만수위 ( TRUE )
    U8 Level;           // 수위 레벨
    
    U8 ConfigMake;      // 사용 설정
    U8 Make;            // 온수 생성 여부
    U16 MakeDelay;      // 온수 생성 지연 시간

    /* TEMP */
    TEMP_T  TempTargetOn;     // 히터 ON 온도 기준치
    TEMP_T  TempTargetOff;    // 히터 OFF 온도 기준치  
    TEMP_T  TempCurrent;      // 히터 현재 온도

    /* Target Temperetrue as mode */
    U8 Altitude;              // 히터 고도 레벨 

    /* Unused Power Saving */
    UnusedSaving_T Saving;
} HotWater_T;

void  InitHotWater(void);
void  GetHotWaterData( HotWater_T *pData );

void  SetHotWaterInitFull(U8 mu8Val );
U8    GetHotWaterInitFull(void);

void  SetHotWaterUnusedOnTime(U16 mu16OnTime);
U16   GetHotWaterUnusedOnTime(void);

void  SetHotWaterLevel(U8 mu8Val );
U8    GetHotWaterLevel(void);

void  SetHotWaterConfigMake(U8 mu8Val );
U8    GetHotWaterConfigMake(void);

void  SetHotWaterMake(U8 mu8Val );
U8    GetHotWaterMake(void);

void  SetHotWaterAltidue(U8 mu8Val );
U8    GetHotWaterAltidue(void);

TEMP_T   GetTargetHotOnTemp(U8 mu8Altitude);
TEMP_T   GetTargetHotOffTemp(U8 mu8Altitude );


void  UpdateHotMakeDelay(void);
void  UpdateHotAirFlowTime(void);

void  ControlHotWaterLevel(void);
void  MakeHotWater(void);

/* UNUSED SAVING */
void SetUnusedSavingConfig(U8 mu8Val );
U8 GetUnusedSavingConfig(void);

void SetUnusedSaving(U8 mu8Val );
U8 GetUnusedSaving(void);

void ResetUnusedSavingTime(void);
void ProcessUnusedSaving(void);

void ControlAirValve(void);

/* AIR FLOW */
void StartHotWaterAirFlow(void);

#endif /* __HOT_WATER_H__ */
