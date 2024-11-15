#ifndef __HW__H__
#define __HW__H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
//#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"
#include "r_cg_macrodriver.h"


/* HW
 *
 * TIMER0 
 *  -. BASE TIMER 1ms
 *
 * PIN INTERRUPT
 *
 */


/* Watchdog timer reset */
#define	RESET_WDT()		    R_WDT_Restart()

#define TOF_GPIO_OUT_1      P7.0
#define TURN_ON_TOF_1()     do{ TOF_GPIO_OUT_1 = 1; }while(0)
#define TURN_OFF_TOF_1()     do{ TOF_GPIO_OUT_1 = 0; }while(0)

#endif /* __HW__H__ */

