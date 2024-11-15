#ifndef __TEMP_TABLE_H__
#define __TEMP_TABLE_H__

#include "prj_type.h"

#define CONFIG_TEMP_TABLE_HOT       1       // pull-down resisotr 65k
#define CONFIG_TEMP_TABLE_HOT_15K   0       // pull-down resistor 15k
#define CONFIG_TEMP_TABLE_AMBIENT   0
#define CONFIG_TEMP_TABLE_EVA       0
#define CONFIG_TEMP_TABLE_COLD      1
#define CONFIG_TEMP_TABLE_ROOM      0
#define CONFIG_TEMP_TABLE_TDS       0

#if (CONFIG_TEMP_TABLE_HOT) || (CONFIG_TEMP_TABLE_HOT_15K)
TEMP_T   ConvAdc2Temp_HotWater(U16 mu16Adc);    
#endif

#if CONFIG_TEMP_TABLE_AMBIENT
TEMP_T   ConvAdc2Temp_Ambient(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_EVA
TEMP_T   ConvAdc2Temp_Eva(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_COLD
TEMP_T   ConvAdc2Temp_ColdWater(U16 mu16Adc);
#endif

#if CONFIG_TEMP_TABLE_ROOM
TEMP_T   ConvAdc2Temp_RoomWater(U16 mu16Adc);
#endif


#if CONFIG_TEMP_TABLE_TDS
TEMP_T   ConvAdc2Temp_TDSIn(U16 mu16Adc);
TEMP_T   ConvAdc2Temp_TDSOut(U16 mu16Adc);
#endif

#endif /* __TEMP_TABLE_H__ */
