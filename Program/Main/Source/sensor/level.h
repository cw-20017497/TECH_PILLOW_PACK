#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "prj_type.h"
#include "hal_level.h"


/* LEVEL */
#define  LEVEL_LOW            HAL_LEVEL_LOW        
#define  LEVEL_MID            HAL_LEVEL_MID        
#define  LEVEL_ERR_LOW        HAL_LEVEL_ERR_LOW    
#define  LEVEL_HIGH           HAL_LEVEL_HIGH       
#define  LEVEL_ERR_COMPLEX    HAL_LEVEL_ERR_COMPLEX
#define  LEVEL_ERR_HIGH       HAL_LEVEL_ERR_HIGH   
#define  LEVEL_ERR_OVF_LOW    HAL_LEVEL_ERR_OVF_LOW
#define  LEVEL_OVF            HAL_LEVEL_OVF        

/*ERROR TYPE */
#define  ERR_TYPE_LOW       0
#define  ERR_TYPE_COMPLEX   1
#define  ERR_TYPE_HIGH      2
#define  ERR_TYPE_OVF       3
#define  ERR_TYPE_NUM       4

// ?�?� ?�서 감지 ?�간 초기화
void InitTankConfDetectTime(void);

// ?�서 감지 지???�간 ?�정
void SetTankConfDetectTime(U8 mu8Id, U8 mu16DetectTime);

U8 GetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType);
void SetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType, U8 mu8Count);

/* TANK LEVEL ID  */
#define  LEVEL_ID_ROOM        0
#define  LEVEL_ID_COLD        1
#define  LEVEL_ID_HOT         2
#define  MAX_LEVEL_ID         3
U8 GetTankLevel(U8 mu8Id);

// ?��??�서 감지 ?��? 반?�
// Return :
//  TRUE : 감지
//  FALSE : 미�?지
U8 IsDetectTankLevel(U8 mu8Id, U8 mu8Level);

// ?��?만?�?� ?��? ?�??
// Return :
//  TRUE : ?��?가?� 
//  FALSE : ?��?가?� ?�?�
U8 IsTankLevelFull(U8 mu8Id);

// ?��??�서 ?�???�?� 검??
// Return
//  TRUE : ?�??
//  FALSE ; ?�?�
U8 IsErrorTankLevel(U8 mu8Level);


void ProcessTankLevel(void);

#endif /* __LEVEL_H__ */
