/**
 * File : api_adc.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"
#include "api_adc.h"

#include "hal_adc.h"

#include "level_electrode.h"


/**
 * To Do
 * Cold Sensor / Hot Sensor 전원 ON/OFF PIN 매칭해야 함
**/


/* Sensor Power Off -> ON 후 전압 안정화 딜레이 */
#define POWER_TURN_ON_DELAY           (1000UL)      // 1000ms @1ms
#define POWER_TURN_ON_DELAY_10MS      (10UL)        // 10ms @1ms
#define POWER_TURN_ON_DELAY_1MS       (1UL)         // 1ms @1ms

/**
 * @ brief     Function For On Power Of Cold TempSensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void EnableTempCold(void)
{
  
}

/**
 * @ brief     Function For Off Power Of Cold TempSensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void DisableTempCold(void)
{
    //TURN_OFF_TEMP_COLD_ON_OFF();
}

/**
 * @ brief     Function For On Power Of Room TempSensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void EnableTempRoom(void)
{
    //TURN_ON_ADC_ROOM();
}

/**
 * @ brief     Function For Off Power Of Room TempSensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void DisableTempRoom(void)
{
    //TURN_OFF_ADC_ROOM();
}

/**
 * @ brief     Function For On Power Of Leak Sensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void EnableLeakSensor(void)
{
    //TURN_ON_ADC_ROOM();
}

/**
 * @ brief     Function For Off Power Of Leak Sensor
 * @ detail    none
 * @ param   none
 * @ return   none
**/
static void DisableLeakSensor(void)
{
    //TURN_OFF_ADC_ROOM();
}

typedef struct _adc_data_
{
    U16 mUsum;
    U8  mUsumCount;

    U16 mUaverage;

    U16 mUmin;
    U16 mUmax;
} SAdcData_T;

SAdcData_T  gStArrAdcData[ADC_ID_MAX];

typedef void (*PFControl_T)(void);
typedef struct _adc_config_
{
    U8          mUid;               // adc id
    U8          mUchannel;          // adc channel
    U8          mUsumCount;         // adc sum count

    U16         mUmin;              // filtering min value
    U16         mUmax;              // filtering max value

    U8          mUonOff;            // Adc Channel On / Off
    U16         mUturnOnDelay;      // Adc Channel Turn On Delay
    U16         mUturnOnDelaySet;      // Adc Channel Turn On Delay
    PFControl_T mPtrFunEnable;      // Adc Channel On Function
    PFControl_T mPtrFunDisable;     // Adc Channel Off Function
} SAdcConfig_T;

SAdcConfig_T gStArrAdcConfigList[] =
{
    /* Adc Id                     Adc Ch,  SumCount,  Initial Min  Inital Max,  OnOff,   Delay,                     Set_Delay                 Enable,              Disable */
    {ADC_ID_TEMP_HOT_WATER_FB,    0,       52U,       1024U,       0U,          ON,      0U,                        0U,                       NULL,                NULL     },
    {ADC_ID_TEMP_COLD_WATER_FB,   1,       52U,       1024U,       0U,          ON,      0U,                        0U,                       NULL,                NULL     },
    {ADC_ID_LEVEL_HOT_TANK_FB,    2,       52U,       1024U,       0U,          ON,      POWER_TURN_ON_DELAY_10MS,  POWER_TURN_ON_DELAY_10MS, EnalbeLevelElec,     DisableLevelElec    },
    {ADC_ID_PRESSURE_SW,          3,       52U,       1024U,       0U,          ON,      0U,                        0U,                       NULL,                NULL    },
};

#define SZ_ADC_CONF_LIST    (sizeof(gStArrAdcConfigList)/sizeof(SAdcConfig_T))

/**
 * @ brief     Function For Initialize Adc
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void InitAdc(void)
{
    U8 uIndex;
    U8 uChannel;

    // Setup inital value...
    for( uIndex = 0; uIndex < SZ_ADC_CONF_LIST ; uIndex++ )
    {
        if( gStArrAdcConfigList[ uIndex ].mPtrFunEnable != NULL )
        {
            gStArrAdcConfigList[ uIndex ].mPtrFunEnable();
        }

        uChannel = gStArrAdcConfigList[ uIndex ].mUchannel;
        gStArrAdcData[ uIndex ].mUaverage = HalSampleAdc( uChannel );
    }

    // setup front init value...
    //gStArrAdcData[ ADC_ID_TEMP_WATER_OUT_FB ].mUaverage = 1024;
}

/**
 * @ brief            Function For Set Adc Data
 * @ detail           none
 * @ param   xUid     Adc Id
 * @ param   xUadc    Adc Average Data
 * @ return           none
**/
void SetAdcData(U8 xUid, U16 xUadc )
{
    if( xUid < ADC_ID_MAX )
    {
        gStArrAdcData[ xUid ].mUaverage = xUadc;
    }
}

/**
 * @ brief            Function For Get Adc Data
 * @ detail           none
 * @ param   xUid     Adc Id
 * @ return           Adc Average Data
**/
U16 GetAdcData( U8 xUid )
{
    if( xUid >= ADC_ID_MAX )
    {
        return 0;
    }

    return gStArrAdcData[ xUid ].mUaverage;
}

/**
 * @ brief                 Function For Find Adc Id
 * @ detail                none
 * @ param   xUid          Adc Id
 * @ param   xUptrIndex    List Index Address
 * @ return                Adc Average Data
**/
static U8 GetIndexConfList(U8 xUid, U8 *xUptrIndex)
{
    U8 uIndex;

    for( uIndex = 0; uIndex < SZ_ADC_CONF_LIST ; uIndex++ )
    {
        if( gStArrAdcConfigList[ uIndex ].mUid == xUid )
        {
            *xUptrIndex = uIndex;
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @ brief                 Function For On Adc Channel
 * @ detail                none
 * @ param   xUid          Adc Id
 * @ return                none
**/
void TurnOnSensingAdc(U8 xUid )
{
    U8 uIndex = 0U;

    if( GetIndexConfList( xUid, &uIndex) == TRUE )
    {
        gStArrAdcConfigList[ uIndex ].mUonOff = ON;
    }

}

/**
 * @ brief                 Function For Off Adc Channel
 * @ detail                none
 * @ param   xUid          Adc Id
 * @ return                none
**/
void TurnOffSensingAdc( U8 xUid )
{
    U8 uIndex = 0U;

    if( GetIndexConfList( xUid, &uIndex) == TRUE )
    {
        gStArrAdcConfigList[ uIndex ].mUonOff = OFF;
    }
}

I8 IsOnOffSensingAdc( U8 xUid )
{
    U8 uIndex = 0U;
    I8 onOff = -1;

    if( GetIndexConfList( xUid, &uIndex) == TRUE )
    {
        onOff = gStArrAdcConfigList[ uIndex ].mUonOff;        
    }

    return onOff;
}

/**
 * @ brief                 Function For Initialize Adc Data
 * @ detail                none
 * @ param   xUid          Adc Id
 * @ return                none
**/
static void InitAdcData( U8 xUid )
{
    U8 uIndex;


    if( GetIndexConfList( xUid, &uIndex) == TRUE )
    {
        gStArrAdcData[ xUid ].mUsum       = 0;
        gStArrAdcData[ xUid ].mUsumCount  = 0;
        gStArrAdcData[ xUid ].mUmin       = gStArrAdcConfigList[ uIndex ].mUmin;
        gStArrAdcData[ xUid ].mUmax       = gStArrAdcConfigList[ uIndex ].mUmax;
    }
}

/**
 * @ brief       Function For Sum Adc Data
 * @ detail      none
 * @ param       none
 * @ return      none
**/
static void SumAdc( void )
{
    U8  uIndex;
    U8  uId;
    U8  uChannel;
    U8  uSumCount;
    U16 uData;

    for( uIndex = 0; uIndex < SZ_ADC_CONF_LIST ; uIndex++ )
    {
        uId = gStArrAdcConfigList[ uIndex ].mUid;
#if !CONFIG_TEST_TEMP_READ
        if( gStArrAdcConfigList[ uIndex ].mUonOff == OFF )
        {
            // 초기화
            if( gStArrAdcConfigList[ uIndex ].mPtrFunDisable != NULL )
            {
                // PORT를 DISABLE하고 나서는 지연 시간을 초기화 한다.
                gStArrAdcConfigList[ uIndex ].mPtrFunDisable();
                gStArrAdcConfigList[ uIndex ].mUturnOnDelay = gStArrAdcConfigList[ uIndex ].mUturnOnDelaySet;
            }

            InitAdcData( uId );
        }
        else
#endif
        {       
            // 센싱 
            if( gStArrAdcConfigList[ uIndex ].mPtrFunEnable != NULL )
            {
                gStArrAdcConfigList[ uIndex ].mPtrFunEnable();

#if !CONFIG_TEST_TEMP_READ
                // PORT를 ENABLE하고, 센서 값이 안정화 될 때가지 지연시간을 둔다.
                if( gStArrAdcConfigList[ uIndex ].mUturnOnDelay != 0 )
                {
                    gStArrAdcConfigList[ uIndex ].mUturnOnDelay--;
                    continue ;
                }
#endif
            }

            uSumCount = gStArrAdcData[ uId ].mUsumCount;
            if( uSumCount < gStArrAdcConfigList [ uIndex ].mUsumCount )
            {
                uChannel  = gStArrAdcConfigList[ uIndex ].mUchannel;
                uData = HalSampleAdc( uChannel ); 

                gStArrAdcData[ uId ].mUsum += uData;
                gStArrAdcData[ uId ].mUsumCount++;

                // Sampling min/max value
                if( uData < gStArrAdcData[ uId ].mUmin )
                {
                    gStArrAdcData[ uId ].mUmin = uData;
                }

                if( uData > gStArrAdcData[ uId ].mUmax )
                {
                    gStArrAdcData[ uId ].mUmax = uData;
                }
            }
        }
    }
}

/**
 * @ brief       Function For Average Adc Sum Data
 * @ detail      none
 * @ param       none
 * @ return      none
**/
static void AverageAdc( void )
{
    U8          uIndex;
    U8          uId;
    U8          uSumCount;
    SAdcData_T  *stPtrAdcData = NULL;


    for( uIndex = 0; uIndex < SZ_ADC_CONF_LIST ; uIndex++ )
    {
        uId           = gStArrAdcConfigList[ uIndex ].mUid;
        stPtrAdcData  = &gStArrAdcData[ uId ];

        uSumCount     = stPtrAdcData->mUsumCount;
        if(  gStArrAdcConfigList[ uIndex ].mUsumCount == uSumCount )
        {
            stPtrAdcData->mUsum -=  (stPtrAdcData->mUmin + stPtrAdcData->mUmax);
            stPtrAdcData->mUaverage =  stPtrAdcData->mUsum / (uSumCount - 2);

            InitAdcData( uId );
        }
    }
}

static void ControlAdc(void)
{
    SumAdc();
    AverageAdc();
}

void ProcessAdc(void)
{
    ControlAdc();
}

