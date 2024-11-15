#ifndef __FCT_H__
#define __FCT_H__

#include "prj_type.h"

void InitFct(void);

void StartFct(void);
void StopFct(void);
U8 IsExpiredFctLimitTimer(void);
U8 GetFctStatus(void);

void SetFctTestTimer(U16 mu16Time);
U16 GetFctTestTimer(void);


// FCT INPUT TEST MASK BIT
#define MK_FCT_KEY_BACK         0x00000001UL
#define MK_FCT_KEY_SETTING      0x00000002UL
#define MK_FCT_KEY_LEFT         0x00000004UL
#define MK_FCT_KEY_RIGHT        0x00000008UL

#define MK_FCT_KEY_ROOM         0x00000010UL
#define MK_FCT_KEY_COLD         0x00000020UL
#define MK_FCT_KEY_CONTY        0x00000040UL
#define MK_FCT_KEY_SODA         0x00000080UL

#define MK_FCT_KEY_USER         0x00000100UL
#define MK_FCT_LEVER_WATER      0x00000200UL
#define MK_FCT_LEVER_ICE        0x00000400UL
#define MK_FCT_TANK_OPEN        0x00000800UL

#define MK_FCT_EEPROM           0x00001000UL
#define MK_FCT_LCD_MUTE         0x00002000UL             
#define MK_FCT_LCD_COLD         0x00004000UL             
#define MK_FCT_LCD_SAVING       0x00008000UL             

#define MK_FCT_LCD_ERROR        0x00010000UL             
#define MK_FCT_LCD_ICE          0x00020000UL             
#define MK_FCT_LCD_LOW          0x00040000UL

#define MK_FCT_ALL              0x0007FFFFUL
void SetFctTestInputBit(U32 mu32MaskBit);
void ClearFctTestInputBit(U32 mu32MaskBit);
void SetFctTestInputVal(U32 mu32MaksBit);
U8 IsSetFctTestInputVal( U32 mu32MaskBit );
U32 GetFctTestInputVal(void);


void UpdateFctTimer(void); // @1sec

#endif /* __FCT_H__ */
