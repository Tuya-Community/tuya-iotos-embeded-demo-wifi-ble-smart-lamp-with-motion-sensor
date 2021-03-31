/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-01 10:44:58
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-15 18:56:24
 */
#ifndef __lamp_CONTROL_H__
#define __lamp_CONTROL_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

typedef enum {
    white,
    medium,
    warm
}LIGHT_MODE;


typedef struct {
    BOOL_T Lamp_delay_off;
    BOOL_T Lamp_switch;
    LIGHT_MODE Light_mode;
    BOOL_T Sit_remind;
    BOOL_T Auto_light;
    BOOL_T Low_pow_alarm;
    BOOL_T Silent_mode;
    BOOL_T Radar_sensor;
    USHORT_T Human_distance;
    UCHAR_T time_hour;
    UCHAR_T time_min;
    UCHAR_T Battery_remain;
}LAMP_CTRL_DATA_T;

/********************************************************************************
 * FUNCTION:       plant_device_init
 * DESCRIPTION:    device initialization
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID lamp_device_init(VOID);

/********************************************************************************
 * FUNCTION:       plant_get_adc_sensor_data
 * DESCRIPTION:    Get ADC sensor data
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID lamp_get_sensor_data(VOID);

/********************************************************************************
 * FUNCTION:       plant_ctrl_handle
 * DESCRIPTION:    sensor data deal handle
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID lamp_ctrl_handle(VOID);

/********************************************************************************
 * FUNCTION:       plant_ctrl_all_off
 * DESCRIPTION:    Close all control components
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID lamp_ctrl_all_off(VOID);

VOID lamp_key_poll(VOID);
#endif