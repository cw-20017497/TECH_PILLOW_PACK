#ifndef __EOL_H__
#define __EOL_H__

#include "prj_type.h"


#define  MODEL_CODE_CHP_260         0x01
#define  MODEL_CODE_CP_260          0x02
#define  MODEL_CODE_CHP_590         0x03
#define  MODEL_CODE_CHP_671         0x04

#define  MODEL_CODE                 MODEL_CODE_CHP_590

#define CONFIG_EOL_TEST_WATER       1   // Î¨ºÍ????¨Î? (?¨Ïö?1, ÎØ∏ÏÇ¨Ïö?0) 

#define  DEFAULT_TEST_TIME_VALUE       54000U   /* 90min, @100msec */
/* EOL STEP ID */

void InitEol(void);

#define EOL_TYPE_LOAD        0
#define EOL_TYPE_FRONT       1
void StartEol(U8 mu8Type);
U8 IsExpiredEolLimitTimer(void);
U8 GetEolStatus(void);
U8 GetEolType(void);

#define EOL_MODE_READY          0
#define EOL_MODE_INIT           1
#define EOL_MODE_CHECK_SENSOR   2
#define EOL_MODE_CHECK_LOAD     3
#define EOL_MODE_CHECK_VACCUM   4
#define EOL_MODE_CHECK_GAS      5
#define EOL_MODE_DONE           6
void SetEolMode(U8 mu8Mode);
U8 GetEolMode(void);

void SetEolTestTimer(U16 mu16Time);
U16 GetEolTestTimer(void);

void SetEolExpiredStepTimer(U8 mu8Val);
U16 GetEolExpiredStepTimer(void);

void ReloadEolCommErrTimer(void);
U16 GetEolCommErrTimer(void);

// EOL INPUT TEST MASK BIT
#define  MK_EOL_PHOTO_SENSOR_DP  0x80000000UL  // ?î?§Ìîå?à??Î™©Ï†Å?ºÎ? ?à?∏Ï†Å?ºÎ? ?¨Ïö?
#define  MK_EOL_KEY_SAVE         0x00000001UL
#define  MK_EOL_KEY_HOT          0x00000002UL
#define  MK_EOL_KEY_COLD         0x00000004UL
#define  MK_EOL_PHOTO_SENSOR     0x00000008UL
#define  MK_EOL_ALL              0x0000000FUL
void SetEolTestInputBit(U32 mu32MaskBit);
void ClearEolTestInputBit(U32 mu32MaskBit);
void SetEolTestInputVal(U32 mu32MaksBit);
U32 GetEolTestInputVal(void);

// Error
void SetEolErrorComm(U8 mu8Error);
U8 GetEolErrorComm(void);

// Check Status
#define  EOL_CHK_ID_EEPROM             0
#define  EOL_CHK_ID_NUM                1
U16 GetEolCheckStatus(U8 mu8Id );
void SetEolCheckStatus(U8 mu8Id, U16 mu16Val);

void UpdateEolTimer(void); // @1sec

#endif /* __EOL_H__ */
