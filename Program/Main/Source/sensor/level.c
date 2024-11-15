#include "level.h"

#define  DETECT_TIME       30 /* 3sec @ 100ms */


typedef U8(*Action_T)(void);
typedef struct _level_list_
{
    U8 PreLevel;        // ?��??�?� ?�?� ?�전 값
    U8 CurLevel;        // ?��??�?� ?�?� ?�??값
    U8 SetDetectTime;   // 감지 지???�간 ?�정 값
    U8 DetectTime;      // 감지 지???�간
    U8 ErrCount[ ERR_TYPE_NUM ];  // ?�??발?� ?�?� 
    Action_T GetLevel;  // ?�크�? ?�?� ?�?� 반?� ?�수
} LevelList_T;

LevelList_T LevelList[ MAX_LEVEL_ID ] = 
{
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelRoomTank },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelColdTank },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelHotTank }
};


// ?�?� ?�서 감지 ?�간 초기화
void InitTankConfDetectTime(void)
{
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    DETECT_TIME );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     DETECT_TIME );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      DETECT_TIME );   // @100ms
}

// ?�서 감지 지???�간 ?�정
void SetTankConfDetectTime(U8 mu8Id, U8 mu16DetectTime)
{
    LevelList[ mu8Id ].SetDetectTime = mu16DetectTime; 
}

// ?��??�서 감지 ?��? 반?�
U8 GetTankLevel(U8 mu8Id)
{
    return LevelList[ mu8Id ].CurLevel; 
}

// ?��??�서 감지 ?��? 반?�
U8 IsDetectTankLevel(U8 mu8Id, U8 mu8Level)
{
   if( (LevelList[ mu8Id ].CurLevel & mu8Level) == mu8Level )
   {
       return TRUE;
   }

   return FALSE;
}

U8 IsTankLevelFull(U8 mu8Id)
{
    U8 mu8Ret = FALSE;
    U8 mu8Level;


    mu8Level = GetTankLevel( mu8Id );
    switch( mu8Level )
    {
        // FULL 
        case LEVEL_HIGH:       
        case LEVEL_ERR_LOW:    
        case LEVEL_ERR_HIGH:   
        case LEVEL_ERR_COMPLEX:
        case LEVEL_ERR_OVF_LOW:
        case LEVEL_OVF:        
            mu8Ret = TRUE;
            break;
            // NOT FULL TANK
        case LEVEL_LOW:        
        case LEVEL_MID:        
        default:
            mu8Ret = FALSE;
            break;
    }

    return mu8Ret;
}


U8 IsErrorTankLevel(U8 mu8Level)
{
    if( mu8Level == LEVEL_OVF
            || mu8Level == LEVEL_ERR_OVF_LOW
            || mu8Level == LEVEL_ERR_HIGH 
            || mu8Level == LEVEL_ERR_COMPLEX 
            || mu8Level == LEVEL_ERR_LOW  )
    {
        return TRUE;
    }

    return FALSE;
}


U8 GetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType)
{
    return LevelList[ mu8Id ].ErrCount[ mu8ErrType ];
}

void SetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType, U8 mu8Count)
{
    LevelList[ mu8Id ].ErrCount[ mu8ErrType ] = mu8Count;
}



static void CountErrorType(U8 mu8Id, U8 mu8Level )
{
    switch( mu8Level )
    {
        case LEVEL_ERR_LOW:      // ������ �̰���, ������ ����
        case LEVEL_ERR_OVF_LOW:  // ������ �̰���, ������ ����, �����÷ο� ����
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_ERR_COMPLEX:  // ������ �̰���, ������ �̰���, �����÷ο� ����
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_ERR_HIGH:  // ������ ����, ������ �̰���, �����÷ο� ���� 
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]     = 0;
             break;

        case LEVEL_OVF: // ������ ����, ������ ����, �����÷ο� ����
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_OVF ]++;

            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]      = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]  = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_HIGH:        // ������ ����, ������ ����
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]      = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]  = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_MID:         // ������ ����
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]  = 0;
            break;

        case LEVEL_LOW:         // ��� ���� �̰���
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]  = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_OVF ]  = 0;
            break;

        default:
            break;
    }
}

void ProcessTankLevel(void)
{
    U8 i;
    U8 mu8Level;

    for( i = 0; i < MAX_LEVEL_ID ; i++ )
    {
        /* Get new level status */
        mu8Level = LevelList[ i ].GetLevel();
        if( LevelList[ i ].PreLevel != mu8Level )
        {
            /* if different, update time, update level status */
            LevelList[ i ].PreLevel    = mu8Level;
            LevelList[ i ].DetectTime  = LevelList[i].SetDetectTime; 
            continue;
        }

        if( LevelList[ i ].DetectTime != 0 )
        {
            LevelList[ i ].DetectTime--;
            continue;
        }

        if( LevelList[ i ].CurLevel != mu8Level )
        {
            /* Count Error */
            CountErrorType( i, mu8Level );
        }

        /* Timeout, update new level status */
        LevelList[ i ].CurLevel = mu8Level;
    }
}
