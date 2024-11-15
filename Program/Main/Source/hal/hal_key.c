#include "hw.h"
#include "hal_key.h"


#define  HAL_KEY_PUSHED     LOW
#define  HAL_LEVER_PUSHED   LOW


typedef struct _hal_key_
{
    U16 Key;
    U8  WaterOut;
    U8  IceOut;
} TKeyVal;

TKeyVal HalKey;


void HAL_InitKey(void)
{
    HalKey.Key      = 0;
    HalKey.WaterOut = 0;
    HalKey.IceOut   = 0;
}

void HAL_SetKeyVal(U16 mu16Val)
{
    HalKey.Key = mu16Val;
}

//U16 HAL_GetKeyVal(void)
//{
//    return HalKey.Key;
//}

U16 HAL_GetKeyVal(void)
{
    U16  mu16Val = 0;


    if( P_KEY_1 == HAL_KEY_PUSHED )
    {
        mu16Val |= HAL_KEY_1;
    }
    if( P_KEY_2 == HAL_KEY_PUSHED )
    {
        mu16Val |= HAL_KEY_2;
    }

    return mu16Val;
}


void HAL_SetLeverIce(U8 mu8Val)
{
    HalKey.IceOut = mu8Val;
}

U8 HAL_GetLeverIce(void)
{
    return HalKey.IceOut;
}


void HAL_SetLeverWater(U8 mu8Val)
{
    HalKey.WaterOut = mu8Val;
}


U8 HAL_GetLeverWater(void)
{
    return HalKey.WaterOut;
}
