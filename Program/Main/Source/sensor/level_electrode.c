/**
 * File : level_electrode.c
 * 
 * Level Electrode ( ?�극�?)
**/
#include "hw.h"
#include "level_electrode.h"

#include "api_adc.h"

void EnalbeLevelElec(void)
{
    TURN_OFF_LEVEL_HOT_TANK_DIRECTION_1();
    TURN_OFF_LEVEL_HOT_TANK_DIRECTION_2();
}

void DisableLevelElec(void)
{
    TURN_ON_LEVEL_HOT_TANK_DIRECTION_1();
    TURN_ON_LEVEL_HOT_TANK_DIRECTION_2();
}

U8 IsTurnOnLevelElec(void)
{
    if( ( GET_STATUS_LEVEL_HOT_TANK_DIRECTION_1() == LOW )
         && ( GET_STATUS_LEVEL_HOT_TANK_DIRECTION_1() == LOW ) )
    {
        return TRUE;
    }

    return FALSE;
}

//ADC_ID_LEVEL_HOT_TANK_FB

static void StartMesureAdc( U8 xUid )
{
    TurnOnSensingAdc(xUid);
}

static void StopMesureAdc( U8 xUid )
{
    TurnOffSensingAdc(xUid);
}


#define POWER_SWAP_CYCLE      (100U)        // 100ms @1ms ( 90ms ?�안 adc 측정 )
#define LEVEL_DETECT_TIME     (10U)         // 1000ms @ 100ms

typedef void (*PFAction_T)(void);
typedef void (*PFGet_T)(void);
typedef struct _level_elec_pwr_
{
    U8            mEadcId;
    U16           mUpowerOnSwapCnt;     // ?�위 ?�서 ?�워 극성 변�?주기 카운??
    U16           mUpowerOffSwapCnt;     // ?�위 ?�서 ?�워 극성 변�?주기 카운??
    U16           mUpowerSwapCycle;     // ?�위 ?�서 ?�워 극성 변�?주기
} SLevelElecPwr_T;

static SLevelElecPwr_T gStArrLevelElecPwrList[] = 
{
    { ADC_ID_LEVEL_HOT_TANK_FB, 0, 0, POWER_SWAP_CYCLE },

};  

#define SZ_LEVEL_ELEC_PWR_LIST     (sizeof(gStArrLevelElecPwrList)/sizeof(SLevelElecPwr_T))

static void ScanLevelElec(void)
{
    U8 uIndex = 0U;
    SLevelElecPwr_T *gStPtrLevelElecPwrList;

    for( uIndex=0; uIndex<SZ_LEVEL_ELEC_PWR_LIST; uIndex++ )
    {
        gStPtrLevelElecPwrList = &gStArrLevelElecPwrList[uIndex];

        if( gStPtrLevelElecPwrList->mUpowerOffSwapCnt != 0 )
        {
            gStPtrLevelElecPwrList->mUpowerOffSwapCnt--;
            continue;
        }
        
        if( gStPtrLevelElecPwrList->mUpowerOnSwapCnt != 0 )
        {
            gStPtrLevelElecPwrList->mUpowerOnSwapCnt--;
            
            if( IsOnOffSensingAdc(gStPtrLevelElecPwrList->mEadcId) == OFF )
            {
                StartMesureAdc(gStPtrLevelElecPwrList->mEadcId);
            }
        }
        else
        {
            gStPtrLevelElecPwrList->mUpowerOnSwapCnt = gStPtrLevelElecPwrList->mUpowerSwapCycle;
            gStPtrLevelElecPwrList->mUpowerOffSwapCnt = gStPtrLevelElecPwrList->mUpowerSwapCycle;

            if( IsOnOffSensingAdc(gStPtrLevelElecPwrList->mEadcId) == ON )
            {
                StopMesureAdc(gStPtrLevelElecPwrList->mEadcId);
            }
        }
    }
}

// �̰��� : 491 , ������ : 577

/* �¼� ��ũ ���� �̰��� �� ( 840~850 ), ������ ( 540~550 ) */
#define THRESHOLD_HISTERESIS      (20U)          // 0.10v
#define THRESHOLD_HOT_TANK        (700U)         // 3.42v (700) -> 3.66v (750) -> 3.90(800) -> 3.17(650)
//#define THRESHOLD_HOT_TANK        (530U)         // 3.42v (700) -> 3.66v (750) -> 3.90(800) -> 3.17(650)
typedef struct _level_electrode_
{
    EAdcId_T       mEadcId;
    ELevelStatus   mEPreStatus;         // ?�위 ?�서 ?�전 ?�태
    ELevelStatus   mEstatus;            // ?�위 ?�서 ?�재 ?�태
    U16            mUadcData;           // ?�위 ?�서 adc Data
    U16            mUDetectTimeCnt;     // ?�위 ?�서 ?�태 최종 ?�데?�트 ?�간 Count
    U16            mUSetDetectTime;     // ?�위 ?�서 ?�태 최종 ?�데?�트 ?�간 Config
    U16            mUthreshold;         // 감�? Data Threshold
} SLevelElectrode_T;

SLevelElectrode_T gStArrLevelElecList[] =
{
    /* AdcId                      PrevStatus,  Status,  AdcData,  DetectTimeCnt,      DetectTimeSet,       Threshold         */

    { ADC_ID_LEVEL_HOT_TANK_FB,   0,            0,        0,       LEVEL_DETECT_TIME,  LEVEL_DETECT_TIME,   THRESHOLD_HOT_TANK },       // HOT TANK

};

#define SZ_LEVEL_ELEC_LIST     (sizeof(gStArrLevelElecList)/sizeof(SLevelElectrode_T))

void SetLevelElecStatus( ELevelElecId xUid, ELevelStatus xEstatus )
{
    gStArrLevelElecList[xUid].mEstatus = xEstatus;
}

ELevelStatus GetLevelElecStatus( ELevelElecId xUid )
{
    return gStArrLevelElecList[xUid].mEstatus;
}

static ELevelStatus Get_Adc2Status( ELevelElecId xUid, U16 xUthreshold )
{
    /* Hysteresis */
    if( gStArrLevelElecList[xUid].mUadcData >= xUthreshold )
    {
        if( gStArrLevelElecList[xUid].mUadcData >= ( gStArrLevelElecList[xUid].mUthreshold + THRESHOLD_HISTERESIS ) ) 
        {
            return LEVEL_STATUS_UNDETECT;
        }
    }
    else
    {
        if( gStArrLevelElecList[xUid].mUadcData < ( gStArrLevelElecList[xUid].mUthreshold - THRESHOLD_HISTERESIS ) ) 
        {
            return LEVEL_STATUS_DETECT;
        }
    }    

    return LEVEL_STATUS_NONE;
}

static void ControlLevelElec(void)
{
    U8                  uIndex = 0U;
    ELevelStatus        eStatus = LEVEL_STATUS_NONE;
    SLevelElectrode_T   *gStPtrLevelElecList;
    
    for( uIndex =0; uIndex<SZ_LEVEL_ELEC_LIST; uIndex++ )
    {
        gStPtrLevelElecList = &gStArrLevelElecList[uIndex];
        
        gStPtrLevelElecList->mUadcData = GetAdcData(gStPtrLevelElecList->mEadcId);     
        eStatus = Get_Adc2Status( uIndex, gStPtrLevelElecList->mUthreshold);

        if( gStPtrLevelElecList->mEPreStatus != eStatus)
        {
            gStPtrLevelElecList->mEPreStatus = eStatus;
            gStPtrLevelElecList->mUDetectTimeCnt = gStPtrLevelElecList->mUSetDetectTime;
        }

        if( gStPtrLevelElecList->mUDetectTimeCnt != 0 )
        {
            gStPtrLevelElecList->mUDetectTimeCnt--;
            continue;
            
        }

        SetLevelElecStatus(uIndex, eStatus);

    }

}

void ProcessLevelElec(void)
{
    ControlLevelElec();
}

void ProcessScanLevelElec(void)
{
    ScanLevelElec();
}

