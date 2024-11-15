#include "api_adc.h"
#include "temp.h"
#include "temp_table.h"

typedef TEMP_T(*Action_T)(U16 mu16Adc);
typedef struct _temp_list_
{
    U8 Id;                  // Analoge Channel Id 
    Action_T ConvAdc2Temp;  // Convert raw adc to temperture

    U8 OverrideFlag;        // Override Flag
    TEMP_T OverrideTemp;    // Overrided Temperture Value

    U32 TimeOut;            // @100msec
} TempList_T;

TempList_T TempList[] = 
{
    { ADC_ID_TEMP_COLD_WATER_FB,  ConvAdc2Temp_ColdWater,  FALSE,  24.0f,     0 },
    { ADC_ID_TEMP_HOT_WATER_FB,   ConvAdc2Temp_HotWater,   FALSE,  24.0f,     0 }
};
#define	SZ_LIST		( sizeof( TempList ) / sizeof( TempList_T ))

static U8 ConvAdc2Temp( U8 mu8Id, U16 mu16AdcRaw, TEMP_T *pTemp )
{
    if( pTemp == NULL )
    {
        return FALSE;
    }

    if( TempList[ mu8Id ].ConvAdc2Temp != NULL )
    {
        *pTemp = TempList[ mu8Id ].ConvAdc2Temp( mu16AdcRaw );

        return TRUE;
    }

    return FALSE;
}

TEMP_T GetTemp(U8 mu8Id )
{
    U16  mu16Adc = 0U;
    TEMP_T  mTemp = 0.0f;


    /* Out of range */
    if( mu8Id >= TEMP_ID_MAX )
    {
        return 0.0f;
    }

    /* Get Adc value */
    mu16Adc = GetAdcData( TempList[ mu8Id ].Id );

    /* Conv Adc to Temperture */
    ConvAdc2Temp( mu8Id, mu16Adc, &mTemp );

    /* Override Temp */
    if( TempList[ mu8Id ].OverrideFlag == TRUE )
    {
        return TempList[ mu8Id ].OverrideTemp;
    }

    return mTemp;
}

U16 GetTempSensorTime(U8 mu8Id)
{
    return (U16)( TempList[ mu8Id ].TimeOut / 10UL );
}

// 현재 시간 보다 큰 경우에는 업데이트 해준다.
void TurnOnTempSensor(U8 mu8Id, U16 mu16TimeOut)
{
    U32 mu32Time;


    mu32Time = (U32)mu16TimeOut * 10UL;
    if( mu32Time > TempList[ mu8Id ].TimeOut )
    {
        TurnOnSensingAdc( TempList[ mu8Id ].Id );
        TempList[ mu8Id ].TimeOut = mu32Time;
    }
}

void TurnOffTempSensor(U8 mu8Id)
{
    TurnOffSensingAdc( TempList[ mu8Id ].Id );
    TempList[ mu8Id ].TimeOut = 0;
}


// Override
void SetOverrideTemp(U8 mu8Id, U8 mOverride, TEMP_T mTemp )
{
    TempList[ mu8Id ].OverrideFlag = mOverride; 
    TempList[ mu8Id ].OverrideTemp = mTemp; 

}

TEMP_T GetOverrideTemp(U8 mu8Id, U8 mOverride )
{
    return TempList[ mu8Id ].OverrideTemp = mOverride; 
}

TEMP_T  gTempCold = 24.0f;
TEMP_T  gTempHot = 24.0f;
void ProcessTemp(void)
{
    U8 i;

    for( i = 0; i < SZ_LIST; i++ )
    {
        if( TempList[ i ].TimeOut != 0UL )
        {
            TempList[ i ].TimeOut--;
            if( TempList[ i ].TimeOut == 0UL )
            {
                TurnOffTempSensor( i );
            }
        }
    }
    
    // FOR DEBUGGING...
    gTempCold = GetTemp( TEMP_ID_COLD_WATER );
    gTempHot = GetTemp( TEMP_ID_HOT_WATER );
}
