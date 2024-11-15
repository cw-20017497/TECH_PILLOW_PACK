#include "hw.h"
#include "hal_valve.h"


void HAL_OpenValve(Valve_T valve )
{
    if( HAL_VALVE_NOS == valve )
    {
       OPEN_VALVE_NOS(); 
    }
    else if( HAL_VALVE_FEED == valve )
    {
       OPEN_VALVE_FEED(); 
    }
    else if ( HAL_VALVE_AIR == valve )
    {
        OPEN_VALVE_AIR();
    }
    //else if ( HAL_VALVE_FLUSHING == valve )
    //{
    //    OPEN_VALVE_FLUSHING();
    //}
}



void HAL_CloseValve(Valve_T valve)
{
    if( HAL_VALVE_NOS == valve )
    {
       CLOSE_VALVE_NOS(); 
    }
    else if( HAL_VALVE_FEED == valve )
    {
       CLOSE_VALVE_FEED(); 
    }
    else if ( HAL_VALVE_AIR == valve )
    {
        CLOSE_VALVE_AIR();
    }
    //else if ( HAL_VALVE_FLUSHING == valve )
    //{
    //    CLOSE_VALVE_FLUSHING();
    //}
}

