#ifndef __COLD_WATER_H__
#define __COLD_WATER_H__

#include "prj_type.h"
#include "level.h"


/* COLD IN EXTRA FEED TIME WHEN HIGH LEVEL */
#define EXTRA_FEED_TIME    180U /* 18sec @100ms */

typedef struct _cold_water_
{
    U8 InitFull;
    U8 Level;
    
    /* MAKE */
    U8 ConfigMake;
    U8 Make;

    /* MODE */
    U8 Mode;

    /* TEMP */
    TEMP_T  TempTargetOn;
    TEMP_T  TempTargetOff;
    TEMP_T  TempCurrent;

    U16 FeedTime;   
    U16 ExtraFeedTime;  // ������ ���� ��, �߰� �Լ� �ð�

    U8 ExtraMake;       // A���Ƣ� ��A�ơ� flag
    U16 ExtraMakeTime;  // A���Ƣ� ��A�ơ� ��A�ơ�
    U16 ExtraMakeCheckTime; // ��A�ơ� 1���� Timer: A���Ƣ� ��A�ơ� ��A�ơ� E��A��
    
} ColdWater_T;

// �ʱ�ȭ
void  InitColdWater(void);
void  GetColdWaterData(ColdWater_T *pData);

// �ü� ���� ������
void  SetColdWaterInitFull(U8 mu8Val );
U8    GetColdWaterInitFull(void);

// �ü� ���� 
void  SetColdWaterLevel(U8 mu8Val );
U8    GetColdWaterLevel(void);

// �ü� ���� ����
void  SetColdWaterConfigMake(U8 mu8Val );
U8    GetColdWaterConfigMake(void);

// �ü� ���� ���
void  SetColdWaterMake(U8 mu8Val );
U8    GetColdWaterMake(void);

// �ü� �Լ� ���� �ð�
void  SetColdWaterFeedTime(U16 mu16Time );
U16   GetColdWaterFeedTime(void);

// �ü� ���� �� ���
#define  COLD_MODE_NONE          0x00
#define  COLD_MODE_INIT          0x01
#define  COLD_MODE_COOLING       0x02
#define  COLD_MODE_DONE          0x08
#define  COLD_MODE_ALL           0xFF
void SetColdWaterMakeMode(U8 mu8Mode );
void ClearColdWaterMakeMode(U8 mu8Mode );
U8 IsSetColdWaterMakeMode(U8 mu8Mode );
U8 GetColdWaterMakeMode(void );


// �ü� �Լ� ���� �ð�
void  SetColdWaterExtraMakeTime(U16 mu16Time );
U16   GetColdWaterExtraMakeTime(void);

// �ü� ��ũ ���� ����
void  ControlColdWaterLevel(void);

TEMP_T   GetTargetColdOnTemp(void);
TEMP_T   GetTargetColdOffTemp(void);

// ?��?? ?�??i����e���� ?��??
void  MakeColdWater(void);

#endif /* __COLD_WATER_H__ */
