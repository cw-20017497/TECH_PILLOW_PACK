#include "hw.h"
#include "hal_level.h"
#include "level_electrode.h"

#if 0
U8 HAL_GetLevelRoomTank(void)
{
    U8 mu8Val = 0;


    if( GET_ROOM_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
    }
    
    if( GET_ROOM_TANK_HIGH()  == HIGH )
    {
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    //if( GET_ROOM_TANK_OVERFLOW() == HIGH )
    //{
    //    mu8Val |= LEVEL_DETECT_OVF;
    //}


    if( GET_ROOM_TANK_LOW() == HIGH 
            || GET_ROOM_TANK_HIGH() == HIGH )
    {
        mu8Val = LEVEL_DETECT_LOW;
    }
    else if( GET_ROOM_TANK_LOW() == LOW 
            || GET_ROOM_TANK_HIGH() == HIGH )
    {
        mu8Val = 0;
    }
    else if( GET_ROOM_TANK_LOW() == HIGH 
            || GET_ROOM_TANK_HIGH() == LOW )
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH;
    }
    else if( GET_ROOM_TANK_LOW() == LOW 
            || GET_ROOM_TANK_HIGH() == LOW )
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH | LEVEL_DETECT_OVF;   // error..
    }

    return mu8Val;
}
#endif

// tof
extern I16 tof_sensor[7];
#define TOF_RANGE_ERROR     22222
#define TOF_RANGE_OVF       100     // 10.0mm
#define TOF_RANGE_HIGH      300     // 30.0mm
#define TOF_RANGE_MID       760     // 76.0mm
//#define TOF_RANGE_LOW       765 
U8 dbg_level = 0;

#define TOF_STATUS_INIT     0       // Àü¿ø ¸®¼Â.. ÃÖÃÊ Åë½Å Àü
#define TOF_STATUS_RUN      1       // Åë½Å ¼ö½Å
#define TOF_STATUS_STOP     2       // Åë½Å ¹Ì¼ö½Å
extern U8 tof_status;
U8 HAL_GetLevelRoomTank(void)
{
    static U8 mu8Val = 0;

    if( tof_status == TOF_STATUS_INIT )
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH;
    }
    else if( tof_sensor[0] == TOF_RANGE_ERROR )
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH | LEVEL_DETECT_OVF;
    }
    else if( tof_sensor[0] <= TOF_RANGE_OVF )  
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH | LEVEL_DETECT_OVF;
    }
    else if( tof_sensor[0] <= TOF_RANGE_HIGH )  
    {
        mu8Val = LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH;
    }
    else if( tof_sensor[0] <= TOF_RANGE_MID )    
    {
        mu8Val = LEVEL_DETECT_LOW;
    }
    else //if( tof_sensor[0] <= TOF_RANGE_LOW )
    {
        mu8Val = 0;
    }

    dbg_level = mu8Val;
    
    return mu8Val;
}

U8 HAL_GetLevelHotTank(void)
{
    U8 mu8Val = 0;
#if 0
    if( GET_ROOM_TANK_HIGH() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    if( GET_ROOM_TANK_LOW() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
    }
#endif

    if( GetLevelElecStatus(LEVEL_ELEC_ID_HOT_TANK) == LEVEL_STATUS_DETECT )
    {
        mu8Val |= LEVEL_DETECT_LOW;
        mu8Val |= LEVEL_DETECT_HIGH;
    }

    return mu8Val;

}

// ?•?˜ ?±í??˜?„ ê°ì§€?˜ë©?ë§Œ?˜?„
U8 HAL_GetLevelColdTank(void)
{
    U8 mu8Val = 0;


    if( GET_COLD_TANK_HIGH() == HIGH )
    {
        mu8Val |= LEVEL_DETECT_LOW;
        mu8Val |= LEVEL_DETECT_HIGH;
    }
    
    return mu8Val;
}

