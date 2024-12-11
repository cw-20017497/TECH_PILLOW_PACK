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
    U8 Config;    // �̻�� ���� ��� ����
    U8 Run;          // ���� ���� ����

    U16 OnTime;   // �̻�� ���� ���� �ð� @min 
    U16 RunTime;  // �̻�� ���� ���� �ð� @min

    U16 TempStatus;     // �ܱ� �µ� ���� ( UNDER_TEMP, OVER_TEMP )
    U16 TempOverTime;   // Over ���ǿ� ���� ���� �ð�
    U16 TempUnderTime;  // Under ���ǿ� ���� ���� �ð�

} UnusedSaving_T;

typedef struct _hot_water_
{
    U8 InitWaitTime;    // ���� ��� �ð�
    U8 InitFull;        // ��ũ ���� ������ ( TRUE )
    U8 Level;           // ���� ����
    
    U8 ConfigMake;      // ��� ����
    U8 Make;            // �¼� ���� ����
    U16 MakeDelay;      // �¼� ���� ���� �ð�

    /* TEMP */
    TEMP_T  TempTargetOn;     // ���� ON �µ� ����ġ
    TEMP_T  TempTargetOff;    // ���� OFF �µ� ����ġ  
    TEMP_T  TempCurrent;      // ���� ���� �µ�

    /* Target Temperetrue as mode */
    U8 Altitude;              // ���� �� ���� 

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
