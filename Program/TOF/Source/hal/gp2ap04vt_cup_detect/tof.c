#pragma interrupt INTP3 r_intc3_interrupt

#include "hw.h"
#include "tof.h"
#include "gp2ap04vt_api.h"
#include "gp2ap04vt_firmware.h"
#include "r_cg_intc.h"



struct gp2ap04vt_sensor dev[ MAX_TOF_ID ];

#define ERROR_NO_DEVICE                 0x0001
#define ERROR_FIRMWARE_CHECKSUM         0x0002
#define ERROR_CALIBRATION_OFFSET        0x0004
#define ERROR_CALIBRATION_CROSSTALK     0x0008
U16 tof_error[ MAX_TOF_ID ] = {0,};

U8 tof_recv[ MAX_TOF_ID ] = {0,};
U32 tof_recv_isr_cnt[ MAX_TOF_ID ] = {0,};
U32 tof_recv_read_cnt[ MAX_TOF_ID ] = {0,};


static void gp2ap04vt_set_error( U8 id, U16 error )
{
    tof_error[ MAX_TOF_ID ]  |= error;
}


void InitToF(void)
{
    TURN_ON_TOF_1();

    dev[TOF_ID_1].slave_addr = 0x29;

    gp2ap04vt_write_firmware(&dev[TOF_ID_1]);


    dev[TOF_ID_1].data.sensor_status = NOT_CALIBRATED;
    dev[TOF_ID_1].data.offset_user = 0;
    dev[TOF_ID_1].data.xtalk_user = 0;

    dev[TOF_ID_1].data.mode = MODE_SENSOR_OFF;

    gp2ap04vt_init_sensor(&dev[TOF_ID_1]);

    //gp2ap04vt_set_floor_distance(&dev[TOF_ID_1]);
    //gp2ap04vt_set_floor_distance(&dev[TOF_ID_2]);
    //gp2ap04vt_set_floor_distance(&dev[TOF_ID_3]);
    //gp2ap04vt_set_floor_distance(&dev[TOF_ID_4]);
    //gp2ap04vt_set_floor_distance(&dev[TOF_ID_5]);

    // start pin interrupt
    R_INTC3_Start();
}



static void DoCalibration_ID(U8 id)
{
    struct gp2ap04vt_sensor *pdev = &dev[id];
    struct gp2ap04vt_data *pdata = &dev[id].data;

    if (pdata->mode == MODE_OFFSET_CALIBRATION) 
    {
        while (gp2ap04vt_read_flag(pdev) == 0) 
        { 
            // Wait until the calibration is finished
            Delay_MS(90); // ms
        }

        gp2ap04vt_get_offset_data(pdev);

        if (pdata->offset_user == 22222) 
        {
            gp2ap04vt_set_error( id, ERROR_CALIBRATION_OFFSET );
        }
    }

    if (pdata->mode == MODE_XTALK_CALIBRATION) 
    {
        while (gp2ap04vt_read_flag(pdev) == 0) 
        { 
            // Wait until the calibration is finished
            Delay_MS(99); // ms
        }

        gp2ap04vt_get_xtalk_data(pdev);

        if (pdata->xtalk_user >= 200) 
        {
            gp2ap04vt_set_error( id, ERROR_CALIBRATION_CROSSTALK );
        }
    }
}
 

void DoCalibration(void)
{
    DoCalibration_ID( TOF_ID_1 );
}


void StartRange(U8 id)
{
#if 0
    //gp2ap04vt_set_floor_distance( &dev[id] );
    gp2ap04vt_start_ranging( &dev[id] );
    while(gp2ap04vt_read_flag(&dev[id]) == 0) { 
        Delay_MS( 5 );
    }

    gp2ap04vt_stop_ranging(&dev[ id ]);
    gp2ap04vt_get_range_data(&dev[ id ]);
    gp2ap04vt_clear_flag(&dev[ id ]);
#endif


    gp2ap04vt_enable_sensor(&dev[ id ], SENSOR_OFF);

    gp2ap04vt_get_range_data(&dev[ id ]);
    gp2ap04vt_read_cup_data(&dev[ id ]);
    gp2ap04vt_check_cup_edge(&dev[ id ]);
    //output_data(pdata);
    gp2ap04vt_clear_flag(&dev[ id ]);
    gp2ap04vt_enable_sensor(&dev[ id ], SENSOR_ON);
}

U8 CheckStartRange(U8 id )
{
    if(gp2ap04vt_read_flag(&dev[id]) == 0) { // Wait until the calibration is finished
        //HAL_Delay(5); // ms
        return FALSE;
    }

    return TRUE;
}

void StopRange(U8 id)
{
    gp2ap04vt_stop_ranging( &dev[id] );
}

I16 GetRange(U8 id)
{
    return dev[ id ].data.range;
}

U8 xtalk_calibration(struct gp2ap04vt_sensor *pdev) 
{
  struct gp2ap04vt_data *pdata = &pdev->data;

  gp2ap04vt_perform_xtalk_calibration(pdev);

  while (gp2ap04vt_read_flag(pdev) == 0) { // Wait until the calibration is finished
    Delay_MS(33); // ms
  }

  gp2ap04vt_get_xtalk_data(pdev);

  if (pdata->xtalk_user < 200) {
      return TRUE;
    //printf("CROSSTALK  CALIBRATION PASSED!!!\r\n");
    // NOTE THAT: Store sensor_statsu to primary NVM.
    // NOTE THAT: Store xtalk_user to primary NVM.
    // _eraseFlash();
    // writeFlash(ADDRESS_STATUS, (uint8_t*)&pdata->sensor_status, sizeof(pdata->sensor_status));
    // writeFlash(ADDRESS_OFFSET, (uint8_t*)&pdata->offset_user, sizeof(pdata->offset_user));
    // writeFlash(ADDRESS_XTALK, (uint8_t*)&pdata->xtalk_user, sizeof(pdata->xtalk_user));
  } else {
//    printf("CROSSTALK CALIBRATION FAILED!!!\r\n");
      return FALSE;
  }
#if 0
  printf("sensor_status: 0x%X\r\n", pdata->sensor_status);
  printf("xtalk_user: %ld\r\n", pdata->xtalk_user);
#endif
}

U8 offset_calibration(struct gp2ap04vt_sensor *pdev) 
{
    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap04vt_perform_offset_calibration(pdev);

    while (gp2ap04vt_read_flag(pdev) == 0) { // Wait until the calibration is finished
        Delay_MS(33); // ms
    }

    gp2ap04vt_get_offset_data(pdev);

    if (pdata->offset_user != 22222) {
        return TRUE;
        //printf("OFFSET CALIBRATION PASSED!!!\r\n");
    } else {
        return FALSE;
        //printf("OFFSET CALIBRATION FAILED!!!\r\n");
    }
#if 0

    printf("Offset calibration distance[mm]: %d \r\n", OFFSET_CALIBRAION_DISTANCE_MM);
    printf("sensor_status: 0x%X \r\n", pdata->sensor_status);
    printf("offset[0.1mm]: %ld\r\n", pdata->offset_0p1mm);
    printf("offset_user[bin]: %ld\r\n", pdata->offset_user);
#endif
}

U8 dbg_offset = 0;
U8 dbg_xtalk = 0;
U8 dbg_floor = 0;
U8 offset_status[ MAX_TOF_ID ];
U8 xtalk_status[ MAX_TOF_ID ];

U8 GetStatusOffsetCalibration(void)
{
    if( offset_status[ 0 ] == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

U8 GetStatusXtalkCalibration(void)
{
    if( xtalk_status[ 0 ] == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

void ProcessToF(void)
{
    U8 i;

    for( i = 0; i < MAX_TOF_ID ; i++ )
    {
        if( tof_recv[i] == TRUE )
        {
            tof_recv[i] = FALSE;
            tof_recv_read_cnt[i]++;
        }


        if( dbg_offset )
        {
            dbg_offset = 0;

            offset_status[ 0 ] = offset_calibration( &dev[ TOF_ID_1 ]);
        }

        if( dbg_xtalk )
        {
            dbg_xtalk = 0;

            xtalk_status[ 0 ] = xtalk_calibration( &dev[ TOF_ID_1 ]);

            //gp2ap04vt_set_floor_distance( &dev[ TOF_ID_1 ]);
        }

#if 0
        if( dbg_floor )
        {
            dbg_floor = 0;

            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_1 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_2 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_3 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_4 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_5 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_6 ]);
            gp2ap04vt_set_floor_distance( &dev[ TOF_ID_7 ]);
        }
#endif
    }
}

// SENSOR 1 
__interrupt static void r_intc3_interrupt(void)
{
    tof_recv[ TOF_ID_1 ] = TRUE;
    tof_recv_isr_cnt[ TOF_ID_1 ]++;
}


