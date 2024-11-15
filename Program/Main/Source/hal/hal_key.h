#ifndef __HAL_KEY_H__
#define __HAL_KEY_H__

#define  HAL_KEY_NONE   0x0000
#define  HAL_KEY_1      0x0001          //Cold
#define  HAL_KEY_2      0x0002          //Hot
//#define  HAL_KEY_3      0x0004
//#define  HAL_KEY_4      0x0008
//#define  HAL_KEY_5      0x0010
//#define  HAL_KEY_6      0x0020
//#define  HAL_KEY_7      0x0040
//#define  HAL_KEY_8      0x0080
//#define  HAL_KEY_9      0x0100
//#define  HAL_KEY_10     0x0200      // ICE OUT 
//#define  HAL_KEY_11     0x0400      // WATER OUT 
#define  HAL_KEY_ALL    0x0007


void HAL_InitKey(void);

void HAL_SetKeyVal(U16 mu16Val);
U16 HAL_GetKeyVal(void);

void HAL_SetLeverIce(U8 mu8Val);
U8 HAL_GetLeverIce(void);

void HAL_SetLeverWater(U8 mu8Val);
U8 HAL_GetLeverWater(void);

#endif /* __HAL_KEY_H__ */
