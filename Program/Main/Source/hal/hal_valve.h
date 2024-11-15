#ifndef __HAL_VALVE_H__
#define __HAL_VALVE_H__

#include "prj_type.h"

typedef enum
{
    HAL_VALVE_NOS,            
    HAL_VALVE_FEED,
    HAL_VALVE_AIR,
    //HAL_VALVE_FLUSHING,

    MAX_HAL_VALVE_NUM
} Valve_T;

void HAL_OpenValve( Valve_T valve);
void HAL_CloseValve( Valve_T valve);


#endif /* __HAL_VALVE_H__ */
