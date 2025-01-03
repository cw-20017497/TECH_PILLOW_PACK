/**
  @file gp2ap04vt_api.h
*/

#ifndef __GP2AP04VT_API_H__
#define __GP2AP04VT_API_H__

#ifdef __KERNEL__ 
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/of_gpio.h>
#include <linux/miscdevice.h>
#endif

#ifndef __KERNEL__ 
#include "prj_type.h"
#endif

#define API_REVISION        "gp2ap04vt_api_v100"

//#define STM32
#define RL78

#ifdef ARDUINO_UNO
#define DELAY_FUNCTION(value) delay(value)
#endif

#ifdef STM32 
extern void HAL_Delay(uint32_t);
#define DELAY_FUNCTION(period) HAL_Delay(period)
#endif

#ifdef RL78 
#include "util.h"
extern void Delay_MS(U16 ms);
#define DELAY_FUNCTION(period) Delay_MS(period)
#endif

#ifdef __KERNEL__
#define DELAY_FUNCTION(period) mdelay(period)
#endif

#define DEVICE_ID                         0x3F

#define MODE_SENSOR_OFF                      0 
#define MODE_RANGING                         1 
#define MODE_OFFSET_CALIBRATION              3 
#define MODE_XTALK_CALIBRATION               4 

#define NO_ACCESS                         0x00
#define ROM                               0x01
#define RAM                               0x02
#define HIST_RET                          0x04

#define INIT_NO_ERROR                     0x00
#define INIT_ERROR                        0x01

#define NOT_CALIBRATED                    0x00
#define USER_OFFSET_CALIBRATION_DONE      0x01
#define USER_XTALK_CALIBRATION_DONE       0x02
#define USER_CALIBRATION_DONE             0x03
#define USER_OFFSET_CALIBRATION_FAILED    0xFF 
#define USER_XTALK_CALIBRATION_FAILED     0xFE 

#define OP_SHUTDOWN                       0x00
#define OP_RUN                            0x80
#define OP_ONCE                           0x00
#define OP_CONTINUOUS                     0x40

#define SENSOR_ON                            1
#define SENSOR_OFF                           0

#define DYNAMIC_ADDRESS_SETTING_MODE         1
#define NORMAL_MODE                          0



#define INT_DISENABLE                     0x00
#define INT_ENABLE                        0x20
#define PIN_PROX                          0x00
#define PIN_MEASUREMENT_END               0x04
#define PIN_ERROR                         0x08
#define PIN_WINDOW                        0x0C
#define PIN_NO_ERROR_MEASUREMENT_END      0x10
#define INTTYPE_LEVEL                     0x00
#define INTTYPE_PULSE                     0x02
#define SET_CONV1_100HZ                     13
#define SET_CONV1_60HZ                      26
#define SET_CONV1_60HZ_LP                   13 
#define SET_CONV1_30HZ                      57
#define SET_CONV1_30HZ_LP                   13 
#define SET_CONV2_100HZ                      0
#define SET_CONV2_60HZ                       0
#define SET_CONV2_60HZ_LP                    0 
#define SET_CONV2_30HZ                       0
#define SET_CONV2_30HZ_LP                    0 
#define SET_INTERVAL_100HZ                   0
#define SET_INTERVAL_60HZ                    6
#define SET_INTERVAL_60HZ_LP                19 
#define SET_INTERVAL_30HZ                   14
#define SET_INTERVAL_30HZ_LP                58 
#define VCSEL_30MA                        0x0A

// *************************** USER SETTING ***************************
#define SET_VCSEL              VCSEL_30MA 
//#define SET_CONV1              SET_CONV1_100HZ
#define SET_CONV1              67 
#define SET_CONV2              SET_CONV2_100HZ
//#define SET_INTERVAL           SET_INTERVAL_100HZ
#define SET_INTERVAL           3 
#define SET_INTERRUPT          PIN_MEASUREMENT_END | INT_ENABLE | INTTYPE_LEVEL

#define SET_TH_PROX                         30 // mm
#define SET_TH_RANGE_L                      20 // mm
#define SET_TH_RANGE_H                     100 // mm
#define SET_TH_HYS                           5 // mm

#define OFFSET_CALIBRAION_DISTANCE_MM       20 // mm
// *************************** USER SETTING ***************************

#define SET_SPAD_SL                       0x07
#define SET_DIS_SPAD_AUTO                 0x01
#define SET_TH_XTALK_AMB                   500
#define SET_MAXCOUNT_XTALK                 300
#define SET_TH_HXTALK                      250
#define SET_TH_LOWSIG                     2000
#define SET_TH_LOWSN                        40
#define SET_TH_HAMB                       4095
#define SET_TH_HSIG         (uint32_t)0xFFFFFF
#define SET_TH_CONFI_LV                      0
#define SET_PARAM_CONFI1                   400
#define SET_PARAM_CONFI2                  1280
#define SET_TH_SQ_3SIGMA                    25
#define SET_EN_RAWRANGE                      0

#define BIN_NUMBER                         128

/**
 * sensor data
 */
struct gp2ap04vt_data {
    uint8_t  mode;
    uint8_t  sensor_status;

    // Range results
    uint8_t  int_status;
    int16_t  range; // 0.1mm
    uint16_t range_status;
    uint8_t  confidence;
    uint16_t xtalk; // Auto crosstalk update
    int32_t  range_pre; // 0.1mm

    // Signal
    int32_t  signal;
    int32_t  amb;
    int32_t  signal_ref;
    int32_t  amb_ref;
    uint8_t  temperature;
    uint8_t  temp_detect;
    uint32_t sys_clock;
    uint8_t  spad_sl;

    int32_t  ratio;
    uint8_t  cup_detection;

    // Signal win
    uint8_t  maxbin;
    uint8_t  maxbin_ref;
    int16_t  hist_ret_a;
    int16_t  hist_ret_maxbin;
    uint32_t vspad;
    uint32_t vs1;
    uint32_t ibias;

    // User calibration
    int32_t  offset_0p1mm;
    int32_t  offset_user; // 0.01bin
    uint32_t xtalk_user;

    // floor calibration 
};

struct gp2ap04vt_sensor {
    uint8_t slave_addr;

#ifdef __KERNEL__
    struct mutex mutex;
    struct i2c_client *client;
    struct miscdevice gp2ap04vt_misc;
    struct input_dev *range_input_dev;
    struct delayed_work work_queue_timer;
    struct work_struct work_queue_ext_int;
    uint8_t range_enabled;
    int range_delay;
    int range_irq;
    
    int gp2ap04vt_opened;

    enum of_gpio_flags irq_gpio_flags;

#endif

    struct gp2ap04vt_data data;
};

#ifdef __KERNEL__ 
extern int gp2ap_i2c_write(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t);
extern int gp2ap_i2c_read(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t);
#endif

#ifndef __KERNEL__ 
void gp2ap_i2c_write(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t);
void gp2ap_i2c_read(struct gp2ap04vt_sensor *, uint8_t, uint8_t *, uint8_t);
#endif

void gp2ap04vt_init_paramas(struct gp2ap04vt_sensor *);
void gp2ap04vt_init_sensor(struct gp2ap04vt_sensor *);
void gp2ap04vt_clear_calibration_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_set_vs1(struct gp2ap04vt_sensor *);
void gp2ap04vt_software_reset(struct gp2ap04vt_sensor *);
void gp2ap04vt_enable_sensor(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_start_ranging(struct gp2ap04vt_sensor *);
void gp2ap04vt_stop_ranging(struct gp2ap04vt_sensor *);
void gp2ap04vt_read_range_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_read_status_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_read_temperature(struct gp2ap04vt_sensor *);
void gp2ap04vt_read_sys_clock(struct gp2ap04vt_sensor *);
uint16_t gp2ap04vt_read_checksum(struct gp2ap04vt_sensor *);
uint8_t gp2ap04vt_get_device_id(struct gp2ap04vt_sensor *);
void gp2ap04vt_get_range_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_get_signal_data(struct gp2ap04vt_sensor *);
uint8_t gp2ap04vt_read_flag(struct gp2ap04vt_sensor *);
void gp2ap04vt_clear_flag(struct gp2ap04vt_sensor *);
void gp2ap04vt_perform_offset_calibration(struct gp2ap04vt_sensor *);
void gp2ap04vt_perform_xtalk_calibration(struct gp2ap04vt_sensor *);
void gp2ap04vt_get_offset_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_get_xtalk_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_offset_calibration(struct gp2ap04vt_sensor *);
void gp2ap04vt_xtalk_calibration(struct gp2ap04vt_sensor *);
void gp2ap04vt_calculate_offset(struct gp2ap04vt_data *);
void gp2ap04vt_calculate_xtalk(struct gp2ap04vt_data *);
void gp2ap04vt_set_offset_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_set_xtalk_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_read_internal_calibration(struct gp2ap04vt_sensor *);
void gp2ap04vt_set_multiple_sensors(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_set_multiple_sensors_ce(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_set_multiple_sensors_dc(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_set_gpio1(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_set_dynamic_address(struct gp2ap04vt_sensor *, uint8_t);
void gp2ap04vt_read_cup_data(struct gp2ap04vt_sensor *);
void gp2ap04vt_check_cup_edge(struct gp2ap04vt_sensor *);

#endif //__GP2AP04VT_API_H__
