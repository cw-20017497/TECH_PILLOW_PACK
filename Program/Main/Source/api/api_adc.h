/**
 * File : api_adc.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_ADC_H__
#define __API_ADC_H__

#include "prj_type.h"

typedef enum _adc_id_
{
    /* TEMPERATURE SENSOR FEEDBACK */
    ADC_ID_TEMP_HOT_WATER_FB,         // 0 : Hot Temp Sensor Feedback
    ADC_ID_TEMP_COLD_WATER_FB,        // 1 : Cold Temp Sensor Feedback

    /* WATER LEVEL FEEDBACK */
    ADC_ID_LEVEL_HOT_TANK_FB,         // 2 : Hot Tank Level Sensor Feedback
    ADC_ID_PRESSURE_SW,               // 3 : Pressure switch input

    ADC_ID_MAX,                       // 4 : ADC Max
} EAdcId_T;

void InitAdc(void);
void SetAdcData(U8 xUid, U16 xUadc );
U16 GetAdcData( U8 xUid );
void TurnOnSensingAdc(U8 xUid );
void TurnOffSensingAdc( U8 xUid );
I8 IsOnOffSensingAdc( U8 xUid );

void ProcessAdc(void);

#endif
