/*
 * @file name: 
 * @Descripttion: 
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-03 17:19:36
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-04 19:19:17
 */

#ifndef __LAMP_PWM_H__
#define __LAMP_PWM_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"



/**
 * @brief pwm init data structure
 * pwm send data structure
 */
typedef struct 
{
    USHORT_T usFreq;            ///< PWM Frequency
    USHORT_T usDuty;            ///< PWM Init duty
    UCHAR_T ucList[2];          ///< GPIO List 
    UCHAR_T ucChannelNum;       ///< GPIO List length
    BOOL_T bPolarity;           ///< PWM output polarity
    BOOL_T bCCTFlag;           ///< CCT drive mode flag
}USER_PWM_INIT_T;

typedef struct 
{
    USHORT_T duty_cold;         ///< R value,rang from 0 to 1000
    USHORT_T duty_warm;        ///< W value,rang from 0 to 1000
}USER_PWM_DUTY_T;


OPERATE_RET lamp_pwm_init(VOID);

OPERATE_RET lamp_pwm_set(UCHAR_T color, USHORT_T duty);

OPERATE_RET lamp_pwm_off(VOID);

#endif