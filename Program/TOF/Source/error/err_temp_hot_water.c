#include "err_temp_hot_water.h"
#include "err_temp.h"
#include "hal_adc.h"
#include "hot_water.h"

static U8 mu8CountList[4] = 
{ 
    0,
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempHotWater(U8 mu8Error)
{
    U16 mu16Adc;

    if( GetHotWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    mu16Adc = HAL_GetAdcValue( ANI_TEMP_HOT_WATER );
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}

U8 ReleaseErrTempHotWater(U8 mu8Error)
{
    // 온수 생성이 OFF이면, 에러는 정상
    if( GetHotWaterConfigMake() == FALSE )
    {
        return FALSE;   // no error
    }

    return CheckErrTempHotWater(mu8Error);
}
