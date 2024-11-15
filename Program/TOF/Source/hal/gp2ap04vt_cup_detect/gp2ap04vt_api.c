/////////////////////////////////////////////////////////////////////////
//Copyright (C) 2023 SHARP All Rights Reserved.
//This program is for modeling purposes only, and is not guaranteed.
//This program is API of the GP2AP04VT sensor.
/////////////////////////////////////////////////////////////////////////

#include "./gp2ap04vt_api.h"
#include "./gp2ap04vt_firmware.h"

/**
   @brief initialize parameters 
   @param[in,out] data structure of sensor
*/
void gp2ap04vt_init_params(struct gp2ap04vt_sensor *pdev)
{
    struct gp2ap04vt_data *pdata = &pdev->data;

    if (pdata->mode == MODE_OFFSET_CALIBRATION) {
        pdata->sensor_status = NOT_CALIBRATED;
        pdata->offset_user = 0;
        pdata->offset_0p1mm = 0;
        pdata->xtalk_user = 0;
    }

    if (pdata->mode == MODE_XTALK_CALIBRATION) {
        pdata->xtalk_user = 0;
    }
}

/**
   @brief Write setting to register
   @param[in,out] data structure of sensor
*/
void gp2ap04vt_init_sensor(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    uint8_t wdata_multi[4] = {0x00, 0x00, 0x00, 0x00};

    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap04vt_init_params(pdev);

    //gp2ap04vt_software_reset(pdev);

    wdata = RAM; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    DELAY_FUNCTION(1);
    
    wdata = 0x44; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address

    // conv_time_set1_user
    wdata_multi[0] = SET_CONV1;
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // spad_sl_user
    wdata = 0x40; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = SET_SPAD_SL;
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // dis_spad_auto
    wdata = 0xAC; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = SET_DIS_SPAD_AUTO;
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_xtalk_amb
    wdata = 0xF4; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = (uint8_t)(SET_TH_XTALK_AMB & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_XTALK_AMB & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // maxcount_xtalk
    wdata_multi[0] = (uint8_t)(SET_MAXCOUNT_XTALK & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_MAXCOUNT_XTALK & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_hxtalk 
    wdata_multi[0] = (uint8_t)(SET_TH_HXTALK & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_HXTALK & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_low_sig
    wdata = 0x04; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x02; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = (uint8_t)(SET_TH_LOWSIG & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_LOWSIG & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_low_sn
    wdata = 0x0C; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x02; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = (uint8_t)(SET_TH_LOWSN & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_LOWSN & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_hsig
    wdata = 0x14; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x02; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
    wdata_multi[0] = (uint8_t)(SET_TH_HSIG & 0x0000FF);
    wdata_multi[1] = (uint8_t)((SET_TH_HSIG & 0x00FF00) >> 8);
    wdata_multi[2] = (uint8_t)((SET_TH_HSIG & 0xFF0000) >> 16);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_confi_lv
    wdata_multi[0] = (uint8_t)(SET_TH_CONFI_LV);
    wdata_multi[1] = 0; 
    wdata_multi[2] = 0; 
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_param_confi1
    wdata_multi[0] = (uint8_t)(SET_PARAM_CONFI1 & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_PARAM_CONFI1 & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_param_confi2
    wdata_multi[0] = (uint8_t)(SET_PARAM_CONFI2 & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_PARAM_CONFI2 & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_sq_3sigma
    wdata_multi[0] = (uint8_t)(SET_TH_SQ_3SIGMA & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_SQ_3SIGMA & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // en_rawrange 
    wdata_multi[0] = SET_EN_RAWRANGE; 
    wdata_multi[1] = 0; 
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_prox 
    wdata_multi[0] = (uint8_t)(SET_TH_PROX & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_PROX & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_range_l 
    wdata_multi[0] = (uint8_t)(SET_TH_RANGE_L & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_RANGE_L & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_range_h 
    wdata_multi[0] = (uint8_t)(SET_TH_RANGE_H & 0x00FF);
    wdata_multi[1] = (uint8_t)((SET_TH_RANGE_H & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // th_hys 
    wdata_multi[0] = SET_TH_HYS;
    wdata_multi[1] = 0;
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // offset calibration setting
    wdata = 0x1C; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x03; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address
  
    if (pdata->mode == MODE_OFFSET_CALIBRATION) {
        wdata_multi[0] = OFFSET_CALIBRAION_DISTANCE_MM;
    } else {
        wdata_multi[0] = 0x00;
    }
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    // xtalk calibration setting
    wdata = 0xA4; gp2ap_i2c_write(pdev, 0x20, &wdata, 1); // address
    wdata = 0x02; gp2ap_i2c_write(pdev, 0x21, &wdata, 1); // address

    if (pdata->mode == MODE_XTALK_CALIBRATION) {
        wdata_multi[0] = 0x01;
    } else {
        wdata_multi[0] = 0x00;
    }
    gp2ap_i2c_write(pdev, 0x22, wdata_multi, 4);

    wdata = 0x00; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    wdata = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    DELAY_FUNCTION(2); 

    // Set calibration data to sensor
    gp2ap04vt_set_offset_data(pdev);
    gp2ap04vt_set_xtalk_data(pdev);

    // Register
    wdata = SET_INTERRUPT;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x02, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x03, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x04, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x05, &wdata, 1);
    wdata = SET_INTERVAL & 0xFF;
    gp2ap_i2c_write(pdev, 0x06, &wdata, 1);
    wdata = (((uint16_t)SET_INTERVAL & 0xFF00) >> 8);
    gp2ap_i2c_write(pdev, 0x07, &wdata, 1);
    wdata = 0x10; gp2ap_i2c_write(pdev, 0x08, &wdata, 1);
    wdata = 0x68; gp2ap_i2c_write(pdev, 0x09, &wdata, 1);
    wdata = 0x0F; gp2ap_i2c_write(pdev, 0x0A, &wdata, 1);
    //wdata = 0xBE; gp2ap_i2c_write(pdev, 0x0B, &wdata, 1);
    wdata = 0xBF; gp2ap_i2c_write(pdev, 0x0B, &wdata, 1); // force xtalk update
    wdata = 0x04; gp2ap_i2c_write(pdev, 0x0C, &wdata, 1);
    wdata = 0x08; gp2ap_i2c_write(pdev, 0x0D, &wdata, 1);
    wdata = SET_VCSEL;
    gp2ap_i2c_write(pdev, 0x0E, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x0F, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x10, &wdata, 1);
    wdata = 0x70; gp2ap_i2c_write(pdev, 0x11, &wdata, 1);
    wdata = SET_VCSEL;
    gp2ap_i2c_write(pdev, 0x12, &wdata, 1);
    wdata = 0x05; gp2ap_i2c_write(pdev, 0x13, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x14, &wdata, 1);
    wdata = 0x02; gp2ap_i2c_write(pdev, 0x15, &wdata, 1);
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x16, &wdata, 1);
    wdata = 0x11; gp2ap_i2c_write(pdev, 0x17, &wdata, 1);
    wdata = 0xBF; gp2ap_i2c_write(pdev, 0x18, &wdata, 1);
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x19, &wdata, 1);
    wdata = 0x70; gp2ap_i2c_write(pdev, 0x1A, &wdata, 1);
    wdata = 0x01; gp2ap_i2c_write(pdev, 0x1B, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x1C, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x1D, &wdata, 1);
    wdata = 0x07; gp2ap_i2c_write(pdev, 0x1E, &wdata, 1);

#ifdef __KERNEL__ 
    wdata = 0x04; gp2ap_i2c_write(pdev, 0x46, &wdata, 1);  // linux device driver only
#endif

    gp2ap04vt_set_vs1(pdev);
}

void gp2ap04vt_set_vs1(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata;
    uint8_t wdata;
    uint8_t mode;
    uint8_t i;
    struct gp2ap04vt_data *pdata = &pdev->data;

    mode = pdata->mode;

    for (i = 0; i < 3; i++) {
        gp2ap04vt_enable_sensor(pdev, SENSOR_ON);

        DELAY_FUNCTION(10);

        gp2ap04vt_enable_sensor(pdev, SENSOR_OFF);

        DELAY_FUNCTION(2);

        gp2ap04vt_read_internal_calibration(pdev);

        if (pdata->vs1 != 0) {
            break;
        }
    }

    gp2ap_i2c_read(pdev, 0x0C, &rdata, sizeof(rdata));
    wdata = (0x01 | (0xFE & rdata));
    gp2ap_i2c_write(pdev, 0x0C, &wdata, 1);
    wdata = (uint8_t)pdata->vs1;
    gp2ap_i2c_write(pdev, 0x10, &wdata, 1);

    pdata->mode = mode;
}

/**
   @briefi Clear calibration data 
   @details The sensor's register value is initialized.

   @param[in,out] data data structure of sensor
*/
void gp2ap04vt_clear_calibration_data(struct gp2ap04vt_sensor *pdev)
{
    pdev->data.sensor_status = 0;
    pdev->data.offset_user = 0;
    pdev->data.offset_0p1mm = 0;
    pdev->data.xtalk_user = 0;
    gp2ap04vt_init_sensor(pdev);
}

/**
   @brief Software reset
   @details The sensor's register value is initialized.

   @param[in,out] data data structure of sensor
*/
void gp2ap04vt_software_reset(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;

    wdata = 0x01;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);
}

/**
   @brief on / off sensor 

   @param[in] enable
   @param[in,out] data data structre of sensor
*/
void gp2ap04vt_enable_sensor(struct gp2ap04vt_sensor *pdev, uint8_t enable)
{
    uint8_t wdata;
  
    if (enable) {
        wdata= (OP_RUN | OP_CONTINUOUS);
    } else {
        wdata= (OP_SHUTDOWN);
    }

    gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
}

void gp2ap04vt_start_ranging(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    struct gp2ap04vt_data *pdata = &pdev->data;

    pdata->mode = MODE_RANGING;

//    gp2ap04vt_init_params(pdev);
//    gp2ap04vt_init_sensor(pdev);

    // Clear flag
    wdata = 0x02;
    gp2ap_i2c_write(pdev, 0x28, &wdata, 1);

    gp2ap04vt_set_vs1(pdev);

    gp2ap04vt_enable_sensor(pdev, SENSOR_ON);
}

void gp2ap04vt_stop_ranging(struct gp2ap04vt_sensor *pdev)
{
    struct gp2ap04vt_data *pdata = &pdev->data;

    pdata->mode = MODE_SENSOR_OFF;

    gp2ap04vt_enable_sensor(pdev, SENSOR_OFF);

    gp2ap04vt_clear_flag(pdev);

    DELAY_FUNCTION(2);
}

/**
   @brief Read measurement range data

   @param[in,out] data structure of sensor
*/
void gp2ap04vt_read_range_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[2];
    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap_i2c_read(pdev, 0x29, rdata, sizeof(rdata));

    pdata->range = (int16_t)(((uint16_t)rdata[1] << 8) | rdata[0]); // unit: 0.1mm
}

/**
   @brief Check status data(0x28, 0x2B, 0x2C)

   @param[in,out] data data stucture of senosr
*/
void gp2ap04vt_read_status_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata0;
    uint8_t rdata1[2];
    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap_i2c_read(pdev, 0x28, &rdata0, sizeof(rdata0));
    gp2ap_i2c_read(pdev, 0x2B, rdata1, sizeof(rdata1));

    pdata->int_status = rdata0;
    pdata->range_status = (((uint16_t)rdata1[1] & 0xF0) << 8) | rdata1[0];
    pdata->confidence = rdata1[1] & 0x07;
}

void gp2ap04vt_read_temperature(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[2];
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x3A, rdata, sizeof(rdata));
  
    pdata->temperature = rdata[0];
    pdata->temp_detect = rdata[1];
}

void gp2ap04vt_read_sys_clock(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[4];
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x3C, rdata, sizeof(rdata));
  
    pdata->sys_clock = ((uint32_t)rdata[3] << 24) | ((uint32_t)rdata[2] << 16) | ((uint32_t)rdata[1] << 8) | rdata[0];
}

uint16_t gp2ap04vt_read_checksum(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[2];
  
    gp2ap_i2c_read(pdev, 0x24, rdata, sizeof(rdata));

    return ((uint16_t)rdata[1] << 8) | rdata[0];
}

uint8_t gp2ap04vt_get_fw_version(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    uint8_t rdata;
    uint8_t fw_version;

    wdata= (OP_RUN | OP_CONTINUOUS);
    gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    DELAY_FUNCTION(1);

    gp2ap_i2c_read(pdev, 0x40, &rdata, sizeof(rdata));
    fw_version = rdata;

    wdata= (OP_SHUTDOWN);
    gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    DELAY_FUNCTION(2);

    return fw_version;
}

uint8_t gp2ap04vt_get_device_id(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata;
    uint8_t device_id;
  
    gp2ap_i2c_read(pdev, 0x44, &rdata, sizeof(rdata));

    device_id = rdata;

    return device_id;
}

/**
   @brief Get the Measurement range data

   @param[in,out] data structure of senosr
*/
void gp2ap04vt_get_range_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[7];
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x28, rdata, sizeof(rdata));
  
    pdata->int_status = rdata[0];
    pdata->range = (int16_t)(((uint16_t)rdata[2] << 8) | rdata[1]); // unit: 0.1mm
    pdata->range_status = (((uint16_t)rdata[4] & 0xF8) << 8) | rdata[3];
    pdata->confidence = rdata[4] & 0x07;
    pdata->xtalk = ((uint16_t)rdata[6] << 8) | rdata[5];
}

void gp2ap04vt_get_range_data2(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[6];
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x29, rdata, sizeof(rdata));
  
    pdata->range = (int16_t)(((uint16_t)rdata[1] << 8) | rdata[0]); // unit: 0.1mm
    pdata->range_status = (((uint16_t)rdata[3] & 0xF0) << 8) | rdata[2];
    pdata->confidence = rdata[3] & 0x07;
    pdata->xtalk = ((uint16_t)rdata[5] << 8) | rdata[4];
}

/**
   @brief Get the Measurement range data

   @param[in,out] data structure of senosr
*/
void gp2ap04vt_get_signal_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[10];
    int32_t signal;
    int32_t signal_ref;
    int32_t amb;
    int32_t amb_ref;
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x2F, rdata, sizeof(rdata));
  
    signal = (int32_t)(((uint32_t)rdata[2] << 16) | ((uint32_t)rdata[1] << 8) | rdata[0]);
    if (signal & 0x800000) {
        signal = signal - 0x1000000; 
    }
    signal_ref = (int32_t)(((uint32_t)rdata[5] << 16) | ((uint32_t)rdata[4] << 8) | rdata[3]);
    if (signal_ref & 0x800000) {
        signal_ref = signal_ref - 0x1000000; 
    }
    amb = (int32_t)(((uint32_t)rdata[7] << 8) | rdata[6]);
    if (amb & 0x8000) {
        amb = amb - 0x10000; 
    }
    amb_ref = (int32_t)(((uint32_t)rdata[9] << 8) | rdata[8]);
    if (amb_ref & 0x8000) {
        amb_ref = amb_ref - 0x10000; 
    }
    pdata->signal = signal;
    pdata->signal_ref = signal_ref; 
    pdata->amb = amb; 
    pdata->amb_ref = amb_ref; 
}

/**
   @brief Read measurement end flag 0x28h: FLAG

   @param[in] data structure of sensor
*/
uint8_t gp2ap04vt_read_flag(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata;
  
    gp2ap_i2c_read(pdev, 0x28, &rdata, sizeof(rdata));

    return (rdata & 0x10) >> 4;
}

/**
   @brief Clear measurement end flag 0x28h: FLAG

   @param[in] data structure of sensor
*/
void gp2ap04vt_clear_flag(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
  
    wdata = 0xE7;
    gp2ap_i2c_write(pdev, 0x28, &wdata, 1);
}

void gp2ap04vt_perform_offset_calibration(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    struct gp2ap04vt_data *pdata = &pdev->data;

    pdata->mode = MODE_OFFSET_CALIBRATION;

    gp2ap04vt_init_sensor(pdev);

    // Clear flag
    wdata = 0x02;
    gp2ap_i2c_write(pdev, 0x28, &wdata, 1);

    wdata = PIN_MEASUREMENT_END | INTTYPE_LEVEL;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);

    gp2ap04vt_enable_sensor(pdev, SENSOR_ON);
}

void gp2ap04vt_perform_xtalk_calibration(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    struct gp2ap04vt_data *pdata = &pdev->data;

    pdata->mode = MODE_XTALK_CALIBRATION;

    gp2ap04vt_init_sensor(pdev);

    // Clear flag
    wdata = 0x02;
    gp2ap_i2c_write(pdev, 0x28, &wdata, 1);

    wdata = PIN_MEASUREMENT_END | INTTYPE_LEVEL;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);

    gp2ap04vt_enable_sensor(pdev, SENSOR_ON);
}

void gp2ap04vt_get_offset_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    uint8_t rdata[4];
    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap04vt_stop_ranging(pdev);
    gp2ap04vt_clear_flag(pdev);
    wdata = SET_INTERRUPT;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);

    pdata->sensor_status = 0x01;

    wdata = RAM; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    // address
    wdata = 0x40; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);
    DELAY_FUNCTION(1);

    gp2ap_i2c_read(pdev, 0x23, rdata, sizeof(rdata));

    wdata = 0x00; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    wdata = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    DELAY_FUNCTION(2);

    pdata->offset_user = (int32_t)(((uint32_t)rdata[3] << 24) | ((uint32_t)rdata[2] << 16) | ((uint32_t)rdata[1] << 8) | rdata[0]);
    pdata->offset_0p1mm = pdata->offset_user * 15 / 10;
}

void gp2ap04vt_get_xtalk_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata;
    uint8_t rdata[4];
    struct gp2ap04vt_data *pdata = &pdev->data;

    gp2ap04vt_stop_ranging(pdev);
    gp2ap04vt_clear_flag(pdev);
    wdata = SET_INTERRUPT;
    gp2ap_i2c_write(pdev, 0x01, &wdata, 1);

    pdata->sensor_status = 0x03;

    wdata = RAM; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    // address
    wdata = 0x38; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);
    DELAY_FUNCTION(1);

    gp2ap_i2c_read(pdev, 0x23, rdata, sizeof(rdata));

    wdata = 0x00; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    wdata = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    DELAY_FUNCTION(2);

    pdata->xtalk_user = ((uint32_t)rdata[3] << 24) | ((uint32_t)rdata[2] << 16) | ((uint32_t)rdata[1] << 8) | rdata[0];
}

void gp2ap04vt_set_offset_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata[4] = {0, 0, 0, 0};
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    wdata[0] = RAM; gp2ap_i2c_write(pdev, 0x1F, wdata, 1);
    wdata[0] = 0x80; gp2ap_i2c_write(pdev, 0x00, wdata, 1);
    wdata[0] = 0x40; gp2ap_i2c_write(pdev, 0x20, wdata, 1);
    wdata[0] = 0x00; gp2ap_i2c_write(pdev, 0x21, wdata, 1);
    DELAY_FUNCTION(1);

    wdata[0] = (uint8_t)( pdata->offset_user & 0x000000FF);
    wdata[1] = (uint8_t)((pdata->offset_user & 0x0000FF00) >> 8);
    wdata[2] = (uint8_t)((pdata->offset_user & 0x00FF0000) >> 16);
    wdata[3] = (uint8_t)((pdata->offset_user & 0xFF000000) >> 24);

    gp2ap_i2c_write(pdev, 0x22, wdata, 4);

    wdata[0] = 0x00; gp2ap_i2c_write(pdev, 0x00, wdata, 1);
    wdata[0] = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, wdata, 1);
    DELAY_FUNCTION(2);
}

void gp2ap04vt_set_xtalk_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t wdata[4] = {0, 0, 0, 0};
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    wdata[0] = RAM; gp2ap_i2c_write(pdev, 0x1F, wdata, 1);
    wdata[0] = 0x80; gp2ap_i2c_write(pdev, 0x00, wdata, 1);
    wdata[0] = 0x38; gp2ap_i2c_write(pdev, 0x20, wdata, 1);
    wdata[0] = 0x00; gp2ap_i2c_write(pdev, 0x21, wdata, 1);
    DELAY_FUNCTION(1);
  
    wdata[0] = (uint8_t)( pdata->xtalk_user & 0x000000FF);
    wdata[1] = (uint8_t)((pdata->xtalk_user & 0x0000FF00) >> 8);
    wdata[2] = (uint8_t)((pdata->xtalk_user & 0x00FF0000) >> 16);
    wdata[3] = (uint8_t)((pdata->xtalk_user & 0xFF000000) >> 24);

    gp2ap_i2c_write(pdev, 0x22, wdata, 4);

    wdata[0] = 0x00; gp2ap_i2c_write(pdev, 0x00, wdata, 1);
    wdata[0] = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, wdata, 1);
    DELAY_FUNCTION(2);
}

void gp2ap04vt_read_internal_calibration(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata;
    struct gp2ap04vt_data *pdata = &pdev->data;
  
    gp2ap_i2c_read(pdev, 0x42, &rdata, sizeof(rdata));
  
    pdata->vs1   = rdata;
}

/**
   @brief Turn on the power in sequence

   @param[in] data structure of sensor
   @param[in] dynamic address 
*/
void gp2ap04vt_set_multiple_sensors(struct gp2ap04vt_sensor *pdev, uint8_t dynamic_addr)
{
    uint8_t wdata;

    pdev->slave_addr = 0x00;
    wdata = 0x40; gp2ap_i2c_write(pdev, 0x04, &wdata, 1);
    wdata = dynamic_addr; gp2ap_i2c_write(pdev, 0x05, &wdata, 1);
    pdev->slave_addr = dynamic_addr;
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x04, &wdata, 1);

    gp2ap04vt_write_firmware(pdev);

    pdev->data.sensor_status = NOT_CALIBRATED;
    pdev->data.offset_user = 0;
    pdev->data.xtalk_user = 0;

    pdev->data.mode = MODE_SENSOR_OFF;
    gp2ap04vt_init_sensor(pdev);

}

/**
   @brief Set communication enable mode 

   @param[in] data structure of sensor
   @param[in] mode(communication enable setting mode or normal mode) 
*/
void gp2ap04vt_set_multiple_sensors_ce(struct gp2ap04vt_sensor *pdev, uint8_t mode)
{
    uint8_t wdata;
    uint8_t dynamic_addr;

    dynamic_addr = pdev->slave_addr;
    pdev->slave_addr = 0x29;
    if (mode) {
        wdata = 0x41; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_W_EN: 0b1, GPIO0_SL:0b01
    } else {
        wdata = 0x00; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_W_EN: 0b0, GPIO0_SL:0b00
    }
    pdev->slave_addr = dynamic_addr;
}

/**
   @brief Set daisy chain mode 

   @param[in] data structure of sensor
   @param[in] mode(daisy chain setting mode or normal mode) 
*/
void gp2ap04vt_set_multiple_sensors_dc(struct gp2ap04vt_sensor *pdev, uint8_t mode)
{
    uint8_t wdata;
    uint8_t dynamic_addr;

    dynamic_addr = pdev->slave_addr;
    pdev->slave_addr = 0x29;
    if (mode) {
        wdata = 0x82; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_W_EN: 0b1, GPIO0_SL:0b01
    } else {
        wdata = 0x00; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_W_EN: 0b0, GPIO0_SL:0b00
    }
    pdev->slave_addr = dynamic_addr;
}

/**
   @brief Set GPIO1 terminal output in daisy chain mode

   @param[in] data structure of sensor
   @param[in] high or low 
*/
void gp2ap04vt_set_gpio1(struct gp2ap04vt_sensor *pdev, uint8_t output)
{
    uint8_t wdata;
    if (output) {
        wdata = 0x92; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_DAISY: 0b1, GPIO1: 0b1, GPIO0_SL: 0b10
    } else {
        wdata = 0x00; gp2ap_i2c_write(pdev, 0x04, &wdata, 1); // MSA_DAISY: 0b0, GPIO1: 0b0, GPIO0_SL:0b00
    }
}

/**
   @brief Set dyanmic address at 0x05 

   @param[in] data structure of sensor
   @param[in] dynamic address 
*/
void gp2ap04vt_set_dynamic_address(struct gp2ap04vt_sensor *pdev, uint8_t dynamic_addr)
{
    uint8_t wdata;

    wdata = dynamic_addr; gp2ap_i2c_write(pdev, 0x05, &wdata, 1); // MULTI SLAVE ADDRESS(MSA)
    pdev->slave_addr = dynamic_addr;
}

void gp2ap04vt_read_cup_data(struct gp2ap04vt_sensor *pdev)
{
    uint8_t rdata[120];
    uint8_t wdata;

    struct gp2ap04vt_data *pdata = &pdev->data;
  
    DELAY_FUNCTION(1);
    wdata = RAM; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    DELAY_FUNCTION(1);

    // address
    wdata = 0x08; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);

    gp2ap_i2c_read(pdev, 0x23, rdata, 1);
    pdata->maxbin        = rdata[0];

    // address
    wdata = 0x1C; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);

    gp2ap_i2c_read(pdev, 0x23, rdata, 1);
    pdata->maxbin_ref        = rdata[0];

    // address
    wdata = 0x14; gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x03; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);

    gp2ap_i2c_read(pdev, 0x23, rdata, 4); //

    pdata->range_pre = (int32_t)(((uint32_t)rdata[3] << 24) | ((uint32_t)rdata[2] << 16) | ((uint32_t)rdata[1] << 8) | rdata[0]);

    wdata = 0x00; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    wdata = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    DELAY_FUNCTION(2);

    wdata = HIST_RET; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    wdata = 0x80; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    DELAY_FUNCTION(1);

    // address
    wdata = (pdata->maxbin_ref - 3) * 2; 
    gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);

    gp2ap_i2c_read(pdev, 0x23, rdata, 2);
    pdata->hist_ret_a = ((uint16_t)rdata[1] << 8) | rdata[0];

    // address
    wdata = pdata->maxbin * 2; 
    gp2ap_i2c_write(pdev, 0x20, &wdata, 1);
    wdata = 0x00; gp2ap_i2c_write(pdev, 0x21, &wdata, 1);

    gp2ap_i2c_read(pdev, 0x23, rdata, 2);
    pdata->hist_ret_maxbin = ((uint16_t)rdata[1] << 8) | rdata[0];

    wdata = 0x00; gp2ap_i2c_write(pdev, 0x00, &wdata, 1);
    wdata = NO_ACCESS; gp2ap_i2c_write(pdev, 0x1F, &wdata, 1);
    DELAY_FUNCTION(2);
}

#define NO_CUP 0
#define CUP_RANGE 1
#define CUP 2
#define DIRT_ERROR 3 

void gp2ap04vt_check_cup_edge(struct gp2ap04vt_sensor *pdev)
{
    int32_t ratio;
    uint8_t result;
    struct gp2ap04vt_data *pdata = &pdev->data;

    if (pdata->hist_ret_maxbin != 0) {
        ratio = ((int32_t)pdata->hist_ret_a * 1000) / (int32_t)pdata->hist_ret_maxbin;
    } else {
        ratio = 0;
    }

    if (pdata->range == 22222) {
        result = NO_CUP;
    } else if (ratio <= 150) {
        result = NO_CUP;
    } else if (pdata->xtalk > SET_MAXCOUNT_XTALK) {
        result = DIRT_ERROR;
    } else if ((pdata->range_pre > 0) && (pdata->range_pre < 500)) { // range_pre unit: 0.1mm, 50[mm] = 500[0.1mm]
        result = CUP_RANGE;
    } else {
        result = CUP;
    }

    pdata->ratio = ratio;
    pdata->cup_detection = result;

}
