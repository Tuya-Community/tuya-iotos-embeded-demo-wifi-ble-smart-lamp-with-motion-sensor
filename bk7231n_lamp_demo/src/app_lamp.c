/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: app_plant.c
 * @Description: light init process include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-21 11:30:03
 * @LastEditTime: 2021-03-15 11:45:59
 */

#include "app_lamp.h"
#include "lamp_control.h"
#include "uni_time_queue.h"
#include "sys_timer.h"
#include "tuya_iot_wifi_api.h"
#include "FreeRTOS.h"
#include "tuya_hal_thread.h"
#include "uni_thread.h"
#include "queue.h"

#define TASKDELAY_SEC         1000



extern LAMP_CTRL_DATA_T lamp_ctrl_data;

STATIC VOID sensor_data_get_thread(PVOID_T pArg);
STATIC VOID sensor_data_deal_thread(PVOID_T pArg);
STATIC VOID key_scan_thread(PVOID_T pArg);
STATIC VOID sensor_data_report_thread(PVOID_T pArg);

#define BEEP_PORT                    (14)
#define BEEP_LEVEL                   HIGH

OPERATE_RET app_lamp_init(IN APP_LAMP_MODE mode)
{
    OPERATE_RET op_ret = OPRT_OK;

    if(APP_LAMP_NORMAL == mode) {
        
        lamp_device_init();

        //create ADC sensor data collection thread
        tuya_hal_thread_create(NULL, "thread_data_get", 512*4, TRD_PRIO_4, sensor_data_get_thread, NULL);

        tuya_hal_thread_create(NULL, "thread_data_deal", 512*4, TRD_PRIO_4, sensor_data_deal_thread, NULL);

        tuya_hal_thread_create(NULL, "key_scan_thread", 512*4, TRD_PRIO_4, key_scan_thread, NULL);

        tuya_hal_thread_create(NULL, "thread_data_report", 512*4, TRD_PRIO_4, sensor_data_report_thread, NULL);
    }else {
        //not factory test mode
    }

    return op_ret;
}

STATIC VOID sensor_data_get_thread(PVOID_T pArg)
{   
    while(1) {
        PR_DEBUG("sensor_data_get_thread");
        lamp_get_sensor_data();
        tuya_hal_system_sleep(TASKDELAY_SEC/2);
        
    }
}

STATIC VOID key_scan_thread(PVOID_T pArg)
{   


    while(1) {
        lamp_key_poll();
       
        tuya_hal_system_sleep(25);       
    }

}

STATIC VOID sensor_data_deal_thread(PVOID_T pArg)
{   
    while(1) {
        lamp_ctrl_handle();
        tuya_hal_system_sleep(TASKDELAY_SEC);
        
    }
}

STATIC VOID sensor_data_report_thread(PVOID_T pArg)
{   
    while(1) {

        tuya_hal_system_sleep(TASKDELAY_SEC*10);

        app_report_all_dp_status();
    }

}


VOID app_report_all_dp_status(VOID)
{
    OPERATE_RET op_ret = OPRT_OK;

    INT_T dp_cnt = 0;
    dp_cnt = 5;

    TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
    if(NULL == dp_arr) {
        PR_ERR("malloc failed");
        return;
    }

    memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));

    dp_arr[0].dpid = DPID_DELAY_OFF;
    dp_arr[0].type = PROP_BOOL;
    dp_arr[0].time_stamp = 0;
    dp_arr[0].value.dp_value = lamp_ctrl_data.Lamp_delay_off;

    dp_arr[1].dpid = DPID_LIGHT_MODE;
    dp_arr[1].type = PROP_ENUM;
    dp_arr[1].time_stamp = 0;
    dp_arr[1].value.dp_value = lamp_ctrl_data.Light_mode;

    dp_arr[2].dpid = DPID_SIT_REMIND;
    dp_arr[2].type = PROP_BOOL;
    dp_arr[2].time_stamp = 0;
    dp_arr[2].value.dp_value = lamp_ctrl_data.Sit_remind;

    dp_arr[3].dpid = DPID_AUTO_LIGHT;
    dp_arr[3].type = PROP_BOOL;
    dp_arr[3].time_stamp = 0;
    dp_arr[3].value.dp_value = lamp_ctrl_data.Auto_light;

    dp_arr[4].dpid = DPID_LOW_POW_ALARM;
    dp_arr[4].type = PROP_BOOL;
    dp_arr[4].time_stamp = 0;
    dp_arr[4].value.dp_value = lamp_ctrl_data.Low_pow_alarm;

    op_ret = dev_report_dp_json_async(NULL,dp_arr,dp_cnt);
    Free(dp_arr);
    if(OPRT_OK != op_ret) {
        PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
    }

    PR_DEBUG("dp_query report_all_dp_data");
    return;
}


VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;

    dpid = root->dpid;
    PR_DEBUG("dpid:%d",dpid);
    
    switch (dpid) {
    
    case DPID_DELAY_OFF:
        PR_DEBUG("set led switch:%d",root->value.dp_bool);
        lamp_ctrl_data.Lamp_delay_off = root->value.dp_bool;
        break;
        
    case DPID_LIGHT_MODE:
        PR_DEBUG("set light mode:%d",root->value.dp_enum);
        lamp_ctrl_data.Light_mode = root->value.dp_enum;
        break;
    
    case DPID_SIT_REMIND:
        PR_DEBUG("set sit remind switch:%d",root->value.dp_bool);
        lamp_ctrl_data.Sit_remind = root->value.dp_bool;
        break;

    case DPID_AUTO_LIGHT:
        PR_DEBUG("set auto switch:%d",root->value.dp_bool);
        lamp_ctrl_data.Auto_light = root->value.dp_bool;
        break;
    
    case DPID_LOW_POW_ALARM:
        PR_DEBUG("set low power alarm switch:%d",root->value.dp_bool);
        lamp_ctrl_data.Low_pow_alarm = root->value.dp_bool;
        break;

    default:
        break;
    }
    
    app_report_all_dp_status();

    return;

}