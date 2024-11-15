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

// ?ò?Ñ ?ºÏÑú Í∞êÏßÄ ?úÍ∞Ñ Ï¥àÍ∏∞Ìôî
void InitTankConfDetectTime(void);

// ?ºÏÑú Í∞êÏßÄ ÏßÄ???úÍ∞Ñ ?§Ï†ï
void SetTankConfDetectTime(U8 mu8Id, U8 mu16DetectTime);

U8 GetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType);
void SetTankLevelErrorCount(U8 mu8Id, U8 mu8ErrType, U8 mu8Count);

/* TANK LEVEL ID  */
#define  LEVEL_ID_ROOM        0
#define  LEVEL_ID_COLD        1
#define  LEVEL_ID_HOT         2
#define  MAX_LEVEL_ID         3
U8 GetTankLevel(U8 mu8Id);

// ?±ÌÅ??ºÏÑú Í∞êÏßÄ ?¨Î? Î∞ò?ò
// Return :
//  TRUE : Í∞êÏßÄ
//  FALSE : ÎØ∏Í?ÏßÄ
U8 IsDetectTankLevel(U8 mu8Id, U8 mu8Level);

// ?±ÌÅ?Îßå?ò?Ñ ?¨Î? ?ï??
// Return :
//  TRUE : ?±ÌÅ?Í∞Ä?ù 
//  FALSE : ?±ÌÅ?Í∞Ä?ù ?Ñ?ò
U8 IsTankLevelFull(U8 mu8Id);

// ?±ÌÅ??ºÏÑú ?ê???Å?ú Í≤Ä??
// Return
//  TRUE : ?ê??
//  FALSE ; ?ï?Å
U8 IsErrorTankLevel(U8 mu8Level);


void ProcessTankLevel(void);

#endif /* __LEVEL_H__ */
