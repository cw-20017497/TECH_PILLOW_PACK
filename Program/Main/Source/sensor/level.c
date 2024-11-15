#include "level.h"

#define  DETECT_TIME       30 /* 3sec @ 100ms */


typedef U8(*Action_T)(void);
typedef struct _level_list_
{
    U8 PreLevel;        // ?±í??˜?„ ??œ ?´ì „ ê°’
    U8 CurLevel;        // ?±í??˜?„ ??œ ?„??ê°’
    U8 SetDetectTime;   // ê°ì§€ ì§€???œê°„ ?¤ì • ê°’
    U8 DetectTime;      // ê°ì§€ ì§€???œê°„
    U8 ErrCount[ ERR_TYPE_NUM ];  // ???ë°œ? ?Ÿ?˜ 
    Action_T GetLevel;  // ?±í¬ë? ?˜?„ ??œ ë°˜?˜ ?¨ìˆ˜
} LevelList_T;

LevelList_T LevelList[ MAX_LEVEL_ID ] = 
{
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelRoomTank },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelColdTank },
    { 0, 0, DETECT_TIME, DETECT_TIME, { 0, 0, 0, 0 }, HAL_GetLevelHotTank }
};


// ?˜?„ ?¼ì„œ ê°ì§€ ?œê°„ ì´ˆê¸°í™”
void InitTankConfDetectTime(void)
{
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    DETECT_TIME );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     DETECT_TIME );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      DETECT_TIME );   // @100ms
}

// ?¼ì„œ ê°ì§€ ì§€???œê°„ ?¤ì •
void SetTankConfDetectTime(U8 mu8Id, U8 mu16DetectTime)
{
    LevelList[ mu8Id ].SetDetectTime = mu16DetectTime; 
}

// ?±í??¼ì„œ ê°ì§€ ?¬ë? ë°˜?˜
U8 GetTankLevel(U8 mu8Id)
{
    return LevelList[ mu8Id ].CurLevel; 
}

// ?±í??¼ì„œ ê°ì§€ ?¬ë? ë°˜?˜
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
        case LEVEL_ERR_LOW:      // Àú¼öÀ§ ¹Ì°¨Áö, ¸¸¼öÀ§ °¨Áö
        case LEVEL_ERR_OVF_LOW:  // Àú¼öÀ§ ¹Ì°¨Áö, ¸¸¼öÀ§ °¨Áö, ¿À¹öÇÃ·Î¿ì °¨Áö
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_ERR_COMPLEX:  // Àú¼öÀ§ ¹Ì°¨Áö, ¸¸¼öÀ§ ¹Ì°¨Áö, ¿À¹öÇÃ·Î¿ì °¨Áö
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_ERR_HIGH:  // Àú¼öÀ§ °¨Áö, ¸¸¼öÀ§ ¹Ì°¨Áö, ¿À¹öÇÃ·Î¿ì °¨Áö 
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ]++;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]     = 0;
             break;

        case LEVEL_OVF: // Àú¼öÀ§ °¨Áö, ¸¸¼öÀ§ °¨Áö, ¿À¹öÇÃ·Î¿ì °¨Áö
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_OVF ]++;

            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]      = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]  = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_HIGH:        // Àú¼öÀ§ °¨Áö, ¸¸¼öÀ§ °¨Áö
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]      = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_COMPLEX ]  = 0;
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_HIGH ] = 0;
            break;

        case LEVEL_MID:         // Àú¼öÀ§ °¨Áö
            LevelList[ mu8Id ].ErrCount[ ERR_TYPE_LOW ]  = 0;
            break;

        case LEVEL_LOW:         // ¸ğµç ¼öÀ§ ¹Ì°¨Áö
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
