#include "hw.h"
#include "process_fct.h"
#include "fct.h"

#include "hal_adc.h"

#include "key.h"
#include "eeprom.h"
#include "util.h"
#include "comm.h"
#include "timer.h"
#include "display.h"
#include "temp.h"

#include "valve.h"
#include "relay.h"


static U8 IsExpiredTestTime(void)
{
    if( GetFctTestTimer() != 0 )
    {
        return FALSE;   // going on
    }

    return TRUE;  // timeout..
}


static void InitLoad(void)
{
#if 0
    U8 mu8Ret;
    
    // Off UART-1
    /R_UART1_Stop();

    // Turn off Valves
    CloseValve( VALVE_ALL );
    OpenValve( VALVE_NOS );

    // Turn off Relay
    TurnOffRelayAll();

    // TEST, EEPROM
    mu8Ret = TestEeprom();
    if( mu8Ret == TRUE )
    {
        SetFctTestInputBit( MK_FCT_EEPROM );
    }
    else
    {
        ClearFctTestInputBit( MK_FCT_EEPROM );
    }
#endif
}



void ProcessFctLoadTest(void)
{
#if 0
    // TURN OFF ALL LEDS
    TurnOffAllLED();

    // DISPLAY VERSION ON LCD 
    //if( DispVersionFct() == FALSE )
    //{
    //    return ;
    //}

    // EEPROM 
    if( IsSetFctTestInputVal( MK_FCT_EEPROM ) == TRUE )
    {
        //DispWaterOut( ON );
    }
    else
    {
        //DispWaterOut( OFF );
    }

    // PHOTO SENSOR(POWER SAVING)
    if( HAL_GetAdcValue( ANI_SENSOR_PHOTO ) < 650 )
    {
       // DispIceOut( ON );
    }
    else
    {
       // DispIceOut( OFF );
    }


    // KEY - COLD
    if( IsSetFctTestInputVal( MK_FCT_KEY_COLD ) == TRUE )
    {
        //OpenValve( VALVE_COLD_OUT );
        //HAL_TurnOnOffLED( COLD_WATER, ON );
    }
    else
    {
        //CloseValve( VALVE_COLD_OUT );
        //HAL_TurnOnOffLED( COLD_WATER, OFF );
    }

    // ROOM HIGH, LOW 
    if( GET_ROOM_TANK_HIGH() == HIGH )
    {
        //TurnOnRelay( RELAY_ICE_TRAY_CW );
        //TurnOffRelay( RELAY_ICE_TRAY_CCW );
    }
    else if( GET_ROOM_TANK_LOW() == HIGH )
    {
        //TurnOffRelay( RELAY_ICE_TRAY_CW );
        //TurnOnRelay( RELAY_ICE_TRAY_CCW );
    }
    else
    {
        //TurnOffRelay( RELAY_ICE_TRAY_CW );
        //TurnOffRelay( RELAY_ICE_TRAY_CCW );
    }

    // ROOM OVF, COLD HIGH
    if( GET_ROOM_TANK_OVERFLOW() == HIGH )
    {
     //   TurnOnRelay( RELAY_FEEDER_CW );
     //   TurnOffRelay( RELAY_FEEDER_CCW );
    }
    else
    {
        //TurnOffRelay( RELAY_FEEDER_CW );
        //TurnOffRelay( RELAY_FEEDER_CCW );
    }

    // COLD TEMPERTURE
    if( GetTemp( TEMP_ID_COLD_WATER ) >= 8.0f 
            && GetTemp( TEMP_ID_COLD_WATER ) <= 12.0f )
    {
    //    OpenValve( VALVE_DRAIN );
    }
    else
    {
    //    CloseValve( VALVE_DRAIN );
    }
#endif
}


void ProcessFct(void)
{
    static U8 mu8Init = 0;

    // check expired test time out
    if( IsExpiredTestTime() == TRUE )
    {
        Reset();
        return ;
    }

    if( mu8Init == 0 )
    {
        mu8Init = 1;
        InitLoad();
    }
}

