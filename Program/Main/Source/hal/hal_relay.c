#include "hw.h"
#include "hal_relay.h"



void HAL_TurnOnRelay( Relay_T	relay )
{
    if( HAL_RELAY_COMP == relay )
    {
        TURN_ON_COMP();
    }
    else if( HAL_RELAY_HEATER == relay )
    {
        TURN_ON_HEATER();
    }
//    else if( HAL_RELAY_ICE_HEATER == relay )
//    {
//        TURN_ON_ICE_HEATER();
//    }
//    else if( HAL_RELAY_FEEDER_CW == relay )
//    {
//        TURN_ON_FEEDER_CW();
//    }
//    else if( HAL_RELAY_FEEDER_CCW == relay )
//    {
//        TURN_ON_FEEDER_CCW();
//    }
//    else if( HAL_RELAY_ICE_TRAY_CW == relay )
//    {
//        TURN_ON_ICE_TRAY_CW();
//    }
//    else if( HAL_RELAY_ICE_TRAY_CCW == relay )
//    {
//        TURN_ON_ICE_TRAY_CCW();
//    }
}

void HAL_TurnOffRelay( Relay_T	relay)
{
    if( HAL_RELAY_COMP == relay )
    {
        TURN_OFF_COMP();
    }
    else if( HAL_RELAY_HEATER == relay )
    {
        TURN_OFF_HEATER();
    }
//    else if( HAL_RELAY_ICE_HEATER == relay )
//    {
//        TURN_OFF_ICE_HEATER();
//    }
//    else if( HAL_RELAY_FEEDER_CW == relay )
//    {
//        TURN_OFF_FEEDER_CW();
//    }
//    else if( HAL_RELAY_FEEDER_CCW == relay )
//    {
//        TURN_OFF_FEEDER_CCW();
//    }
//    else if( HAL_RELAY_ICE_TRAY_CW == relay )
//    {
//        TURN_OFF_ICE_TRAY_CW();
//    }
//    else if( HAL_RELAY_ICE_TRAY_CCW == relay )
//    {
//        TURN_OFF_ICE_TRAY_CCW();
//    }
}


