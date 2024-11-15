/**
 * File : hal_adc.c
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
**/
#include "hw.h"

#include "hal_adc.h"

/* ADC Register Macro */
#define ADC_SET_CHANNEL(x)             do{ ADS = (x); }while(0)    // Channel Select Register
#define ADC_START_CONVERSION_INT()     do{ ADIF = 0U; ADMK = 0U; ADCS = 1U; }while(0)    
#define ADC_STOP_CONVERSION_INT()      do{ ADCS = 0U; ADMK = 1U; ADIF = 0U; }while(0)
#define ADC_START_CONVERSION()         do{ ADCS = 1U; }while(0)    // AD Conversion Enable
#define ADC_STOP_CONVERSION()          do{ ADCS = 0U; }while(0)    // AD Conversion Disable
#define ADC_SAMPLING()                 while(!ADIF)
#define ADC_GET_VALUE()                (ADCR >> 6)

/**
 * @ brief               Function For Sampling Analoge Data
 * @ detail              none
 * @ param    xUchannel  Adc Channel
 * @ return              Digital Data
**/
U16 HalSampleAdc( U8 xUchannel )
{
    U16 uRawData = 0;

    /* Stop */
    ADIF = 0;
    ADC_STOP_CONVERSION();

    /* Set channel */
    ADC_SET_CHANNEL( xUchannel );

    /* Start adc */
    R_ADC_Set_OperationOn();
    ADC_START_CONVERSION();
    
    /* Drop garbage value */
    ADC_SAMPLING();
    uRawData = ADC_GET_VALUE();

    /* Get real value */
    ADC_SAMPLING();
    uRawData = ADC_GET_VALUE();

    /* Stop */
    ADC_STOP_CONVERSION();
    R_ADC_Set_OperationOff();

    return uRawData;
}
