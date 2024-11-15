#ifndef __HAL_LED_H__
#define __HAL_LED_H__

#define DISPLAY_DATA_RAM_LENGTH     16

#define HAL_LED_01      0x00000001UL        // COLD_STRONG
#define HAL_LED_02      0x00000002UL        // HOT_STRONG
#define HAL_LED_03      0x00000004UL        // COLD_WEAK
#define HAL_LED_04      0x00000008UL        // HOT_WEAK
#define HAL_LED_05      0x00000010UL
#define HAL_LED_06      0x00000020UL
#define HAL_LED_07      0x00000040UL
#define HAL_LED_08      0x00000080UL
#define HAL_LED_09      0x00000100UL
#define HAL_LED_10      0x00000200UL
#define HAL_LED_11      0x00000400UL
#define HAL_LED_12      0x00000800UL
#define HAL_LED_13      0x00001000UL
#define HAL_LED_14      0x00002000UL
#define HAL_LED_15      0x00004000UL
#define HAL_LED_16      0x00008000UL
#define HAL_LED_17      0x00010000UL
#define HAL_LED_18      0x00020000UL
#define HAL_LED_19      0x00040000UL
#define HAL_LED_20      0x00080000UL
#define HAL_LED_21      0x00100000UL
#define HAL_LED_22      0x00200000UL
#define HAL_LED_23      0x00400000UL
#define HAL_LED_24      0x00800000UL
#define HAL_LED_25      0x01000000UL
#define HAL_LED_26      0x02000000UL
#define HAL_LED_27      0x04000000UL
#define HAL_LED_28      0x08000000UL
#define HAL_LED_29      0x10000000UL
#define HAL_LED_30      0x20000000UL
#define HAL_LED_31      0x40000000UL
#define HAL_LED_32      0x80000000UL
#define HAL_LED_ALL     0xFFFFFFFFUL

void HAL_InitLed(void);

/*****************************************************************************************
 FUNCTION NAME : void TurnOffAllLED(void)

 DESCRIPTION
 *  -. 전체 LED OFF

 PARAMETERS: void

 RETURN VALUE: NULL

 NOTES:
 
*****************************************************************************************/
void TurnOffAllLED(void);

/*****************************************************************************************
 FUNCTION NAME : void TurnOnAllLED(void)

 DESCRIPTION
 *  -. 전체 LED OFF

 PARAMETERS: void

 RETURN VALUE: NULL

 NOTES:
 
*****************************************************************************************/
void TurnOnAllLED(void);

/*****************************************************************************************
 FUNCTION NAME : void CommonBitOnOff( U32 mu32Val, U8 mu8OnOff )

 DESCRIPTION
 *  -. Bit 단위 LED on/off
 
 PARAMETERS:
 *  -. val : 해당 bit
 *  -. OnOff: LED On/Off
 
 RETURN VALUE: void 

 NOTES:
 
*****************************************************************************************/
void CommonBitOnOff( U32 mu32Val, U8 mu8OnOff );

/*****************************************************************************************
 FUNCTION NAME : void HAL_SetByteDisplayBuffer( U8 mu8Index, U8 mu8Val )

 DESCRIPTION
 *  -. 상태 변경시 
 *  -. gu8WdataArray[]에 Bit 단위로 저장

 PARAMETERS: 
 *  -. Led : data를 상위 하위로 쪼개서 gu8WdataArray[]에 저장
 *  -. Val : On/Off 값

 RETURN VALUE: NULL

 NOTES:
 
*****************************************************************************************/
void HAL_SetBitDisplayBuffer( U32 mu32Led, U8 mu8OnOff );

/*****************************************************************************************
 FUNCTION NAME : void HAL_SetByteDisplayBuffer( U8 mu8Index, U8 mu8Val )

 DESCRIPTION
 *  -. 전체 LED Turn On/Off 할때 사용
 *  -. gu8WdataArray[]에 Byte 단위로 저장

 PARAMETERS: 
 *  -. Index : gu8WdataArray[] 번호
 *  -. Val : On/Off 값

 RETURN VALUE: NULL

 NOTES:
 
*****************************************************************************************/
void HAL_SetByteDisplayBuffer( U8 mu8Index, U8 mu8Val );




#endif /* __HAL_LED_H__ */
