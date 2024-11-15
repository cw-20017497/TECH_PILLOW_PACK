/*
 * gp2ap_i2c.h
 */

#ifndef GP2AP_I2C_H_
#define GP2AP_I2C_H_
//#include <stdio.h>
//#include <stdint.h>
//#include "stm32f4xx_hal.h"
#include "prj_type.h"
#include "gp2ap04vt_api.h"

//#define TIMEI2C1   200//10
#define TIMEI2C1   200
#define TIMEI2C2   1
#define CLOCKDEFI2C 400
#define CLOCKMAXI2C 1000
#define CLOCKMINI2C 10

#define TOF_PROC_START		0
#define TOF_PROC_DONE		1

/* Exported Functions ----------------------------------------------------------*/

void HAL_TOF_Init(void);

/**
  * @brief	: EEP processing status setting.
  * @param	: U8 EEP processing start or done.
  * @retval : None.
  */
void	HAL_TOF_SetWriteProc( U8 _proc );

/**
  * @brief	: EEP processing status read.
  * @param	: None.
  * @retval : U8 EEP processing start or done.
  */
U8	HAL_TOF_GetWriteProc( void );

/**
  * @brief	: EEP processing status setting.
  * @param	: U8 EEP processing start or done.
  * @retval : None.
  */
void	HAL_TOF_SetReadProc( U8 _proc );

/**
  * @brief	: EEP processing status read.
  * @param	: None.
  * @retval : U8 EEP processing start or done.
  */

U8	HAL_TOF_GetReadProc( void );

//void ack_chk(char ack);
void gp2ap_i2c_read(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t); 
void gp2ap_i2c_read_sram(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t); 
void gp2ap_i2c_write(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t);
void setI2CClock(uint32_t);



#endif /* GP2AP02VT_I2C_H_ */
