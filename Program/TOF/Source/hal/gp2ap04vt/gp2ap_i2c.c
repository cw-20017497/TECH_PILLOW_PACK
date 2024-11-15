/**
  @file gp2ap_i2c.c
  @brief  i2c of the GP2AP sensor
*/

#include "hal_i2c.h"

#include <gp2ap_i2c.h>
#include <stdlib.h>
#include <string.h>
//#include "stm32f4xx_hal.h"
#include "gp2ap04vt_api.h"
#include "hal_eeprom.h"
#include "r_cg_serial.h"


#define WRITE_DELAY 1500 /* 1.5ms */  /* TOF Write Cycle = MAX 1ms */
#define READ_DELAY  1000 /* 1.0ms */  /* TOF Write Cycle = MAX 1ms */


typedef struct _tof_
{
    /* READ/WRITE STATUS */
    U8 WriteProc; 
    U8 ReadProc;

    U32 ReadFail;
    U32 ReadOk;

    U32 WriteFail;
    U32 WriteOk;
} TOF_T;

TOF_T tof;
/* Functions -------------------------------------------------------------------*/

void HAL_TOF_Init(void)
{
    memset( &tof, 0, sizeof( TOF_T ));
}

void	HAL_TOF_SetWriteProc( U8 _proc )
{
    tof.WriteProc = _proc;
}

U8	HAL_TOF_GetWriteProc( void )
{
    return tof.WriteProc;
}

static BOOL_T	HAL_TOF_WriteProcComplete( void )
{
    U16	wait = 10000;

    do
    {
        if( wait > 0 )
            wait--;
        else
            return FALSE;
    }
    while( !( HAL_TOF_GetWriteProc() == TOF_PROC_DONE ) );

    return TRUE;
}

void	HAL_TOF_SetReadProc( U8 _proc )
{
    tof.ReadProc = _proc;
}

U8	HAL_TOF_GetReadProc( void )
{
    return tof.ReadProc;
}

static BOOL_T	HAL_TOF_ReadProcComplete( void )
{
    U16	wait = 10000;

    do
    {
        if( wait > 0 )
            wait--;
        else
            return FALSE;
    }
    while( !( HAL_TOF_GetReadProc() == TOF_PROC_DONE ) );

    return TRUE;
}

static void CountReadError(void)
{
    tof.ReadFail++;
}

static void CountWriteError(void)
{
    tof.WriteFail++;
}

static void i2ca0_StopCondition(void)
{
    R_IICA0_StopCondition();
}

/**
 * @brief Sequential I2C write 
 * @param[in] struct gp2ap04vt_sensor 
 * @param[in] word address 
 * @param[in] write data 
 * @param[in] length 
 */
void gp2ap_i2c_write(struct gp2ap04vt_sensor *pdev, uint8_t word_address, uint8_t *wdata, uint8_t len) 
{
    uint16_t ret = 0;
    uint8_t buf[20];    // 16 + 4


    buf[0] = word_address;
    memcpy(buf + 1, wdata, len);

    HAL_TOF_SetWriteProc( TOF_PROC_START );
    ret = i2ca0_master_send(pdev->slave_addr << 1, buf, len + 1, 200);
    if (ret != 0) {
        CountWriteError();
        i2ca0_StopCondition();
        return;
    }

    if( !HAL_TOF_WriteProcComplete() )
    {
        CountWriteError();
        i2ca0_StopCondition();
        return ;
    }

    i2ca0_StopCondition();
    //Delay_MS( WRITE_DELAY );
    Delay_US( WRITE_DELAY );   // 1.5ms
    tof.WriteOk++;

    return;
}

/**
 * @brief Sequential I2C read 
 * @param[in] struct gp2ap04vt_sensor 
 * @param[in] word address 
 * @param[in] read data 
 * @param[in] length 
 */
void gp2ap_i2c_read(struct gp2ap04vt_sensor *pdev, uint8_t word_address, uint8_t *rdata, uint8_t len) 
{
    uint16_t ret = 0;


    HAL_TOF_SetWriteProc( TOF_PROC_START );
    ret = i2ca0_master_send((pdev->slave_addr << 1), &word_address, 1, 200);
    if (MD_OK != 0) 
    {
        CountReadError();
        i2ca0_StopCondition();
        *rdata = 0xFF;
        return ;
    }

    if( !HAL_TOF_WriteProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return ;
    }

    HAL_TOF_SetReadProc( TOF_PROC_START );

    ret = i2ca0_master_recv((pdev->slave_addr << 1), rdata, len, 200);
    if (ret != 0) 
    {
        *rdata = 0xFF;
        CountReadError();
        i2ca0_StopCondition();
        return ;
    }

    if( !HAL_TOF_ReadProcComplete() )
    {
        CountReadError();
        i2ca0_StopCondition();
        return ;
    }

    i2ca0_StopCondition();
    Delay_US( READ_DELAY );
    tof.ReadOk++;
    return ;
}

