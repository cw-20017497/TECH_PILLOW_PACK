#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"


/* Watchdog timer reset */
#define	RESET_WDT()				   R_WDT_Restart()

/* KEY */
#define  P_KEY_1                   P7.3         // COLD
#define  P_KEY_2                   P7.2         // HOT

/* Matrix */
#define SCAN_2              P7.0
#define SCAN_1              P7.1

#define	INIT_LED_SCAN()	\
	do{	\
		SCAN_1 = 1;	\
		SCAN_2 = 1;	\
	}while(0)
	
#define SELECT_2            P1.7
#define SELECT_1            P5.1

#define	INIT_LED_SELECT()	\
	do{	\
		SELECT_1 = 0;	\
		SELECT_2 = 0;	\
	}while(0)



/* COMP */
#define  P_COMP_ONOFF               P1.2
#define  TURN_ON_COMP()             do{ P_COMP_ONOFF = 1; }while(0)
#define  TURN_OFF_COMP()            do{ P_COMP_ONOFF = 0; }while(0)
#define  GET_STATUS_COMP()          ( P_COMP_ONOFF )

/* HEATER FOR HOT WATER */
#define  P_HEATER                   P1.3
#define  TURN_ON_HEATER()           do{ P_HEATER = 1; }while(0)
#define  TURN_OFF_HEATER()          do{ P_HEATER = 0; }while(0)
#define  GET_STATUS_HEATER()        ( P_HEATER )


/* VALVE - NOS */
#define  P_VALVE_NOS                   P14.6
#define  OPEN_VALVE_NOS()              do{ P_VALVE_NOS = 0; }while(0)
#define  CLOSE_VALVE_NOS()             do{ P_VALVE_NOS = 1; }while(0)
#define  GET_STATUS_VALVE_NOS()        ( P_VALVE_NOS )

/* VALE - FEED */
#define  P_VALVE_FEED                  P14.7
#define  OPEN_VALVE_FEED()             do{ P_VALVE_FEED = 1; }while(0)
#define  CLOSE_VALVE_FEED()            do{ P_VALVE_FEED = 0; }while(0)
#define  GET_STATUS_VALVE_FEED()       ( P_VALVE_FEED )

/* VALE - AIR */
#define  P_VALVE_AIR                  P1.1
#define  OPEN_VALVE_AIR()             do{ P_VALVE_AIR = 1; }while(0)
#define  CLOSE_VALVE_AIR()            do{ P_VALVE_AIR = 0; }while(0)
#define  GET_STATUS_VALVE_AIR()       ( P_VALVE_AIR )


/* VALE - UNUSED */
#define  P_VALVE_UNUSED                  P1.0
#define  OPEN_VALVE_UNUSED()             do{ P_VALVE_UNUSED = 1; }while(0)
#define  CLOSE_VALVE_UNUSED()            do{ P_VALVE_UNUSED = 0; }while(0)
#define  GET_STATUS_VALVE_UNUSED()       ( P_VALVE_UNUSED )


/* VALE - FLUSHING */
//#define  P_VALVE_FLUSHING              P14.1
//#define  OPEN_VALVE_FLUSHING()         do{ P_VALVE_FLUSHING = 1; }while(0)
//#define  CLOSE_VALVE_FLUSHING()        do{ P_VALVE_FLUSHING = 0; }while(0)
//#define  GET_STATUS_VALVE_FLUSHING()   ( P_VALVE_FLUSHING )

/**
 * Load : Level
**/
/* Hot Tank Water Level */
#define P_LEVEL_HOT_TANK_DIRECTION_1             P1.5
#define TURN_ON_LEVEL_HOT_TANK_DIRECTION_1()     do{ P_LEVEL_HOT_TANK_DIRECTION_1 = 1; }while(0)
#define TURN_OFF_LEVEL_HOT_TANK_DIRECTION_1()    do{ P_LEVEL_HOT_TANK_DIRECTION_1 = 0; }while(0)
#define GET_STATUS_LEVEL_HOT_TANK_DIRECTION_1()  ( P_LEVEL_HOT_TANK_DIRECTION_1 )

#define P_LEVEL_HOT_TANK_DIRECTION_2             P1.4
#define TURN_ON_LEVEL_HOT_TANK_DIRECTION_2()     do{ P_LEVEL_HOT_TANK_DIRECTION_2 = 1; }while(0)
#define TURN_OFF_LEVEL_HOT_TANK_DIRECTION_2()    do{ P_LEVEL_HOT_TANK_DIRECTION_2 = 0; }while(0)
#define GET_STATUS_LEVEL_HOT_TANK_DIRECTION_2()  ( P_LEVEL_HOT_TANK_DIRECTION_2 )

/* ROOM - LOW */
#define  P_ROOM_TANK_LOW                P7.4
#define  GET_ROOM_TANK_LOW()           ( P_ROOM_TANK_LOW )
#define  GET_REVERSE_ROOM_TANK_LOW()   ( !P_ROOM_TANK_LOW )

/* ROOM - HIGH */
#define  P_ROOM_TANK_HIGH               P7.5
#define  GET_ROOM_TANK_HIGH()           ( P_ROOM_TANK_HIGH )
#define  GET_REVERSE_ROOM_TANK_HIGH()   ( !P_ROOM_TANK_HIGH )


/* COLD - HIGH */
#define  P_COLD_TANK_HIGH               P3.1
#define  GET_COLD_TANK_HIGH()           ( P_COLD_TANK_HIGH )

/* ROOM - OVER FLOW */
//#define  P_ROOM_TANK_OVERFLOW               P2.6
//#define  GET_ROOM_TANK_OVERFLOW()           ( P_ROOM_TANK_OVERFLOW )
//#define  GET_REVERSE_ROOM_TANK_OVERFLOW()   ( !P_ROOM_TANK_OVERFLOW )

/* COLD TEMP ADC ON/OFF - COLD TEMPERTURE  */
//#define  P_ADC_COLD_ONOFF           P6.3
//#define  TURN_ON_ADC_COLD()         do{ P_ADC_COLD_ONOFF = 1; }while(0)
//#define  TURN_OFF_ADC_COLD()        do{ P_ADC_COLD_ONOFF = 0; }while(0)

/* PUMP - ON OFF */
//#define  P_PUMP_ONOFF                  P7.5
//#define  TURN_ON_PUMP()             do{ P_PUMP_ONOFF = 1; }while(0)
//#define  TURN_OFF_PUMP()            do{ P_PUMP_ONOFF = 0; }while(0)
//#define  GET_PUMP_ONOFF()           (P_PUMP_ONOFF)


/* Condensor DC FAN MOTOR */
//#define  P_FAN_MOTOR                 P14.0
//#define  TURN_ON_FAN_MOTOR()         do{ P_FAN_MOTOR = 1; }while(0)
//#define  TURN_OFF_FAN_MOTOR()        do{ P_FAN_MOTOR = 0; }while(0)
//#define  GET_STATUS_FAN_MOTOR_ONOFF()   (P_FAN_MOTOR)


/* PUMP - CONNETED CHECK */
//#define  P_PUMP_CHECK                P7.6
//#define  GET_STATUS_PUMP_CHECK()     (P_PUMP_CHECK)

#endif /* __HW__H__ */

