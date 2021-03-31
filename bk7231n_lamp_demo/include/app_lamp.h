/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: light_init.h
 * @Description: light init process include file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-21 11:30:03
 * @LastEditTime: 2021-03-15 17:15:18
 */

#ifndef __APP_LAMP_H__
#define __APP_LAMP_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/***********************************************************
*************************variable define********************
***********************************************************/
typedef enum{
    APP_LAMP_NORMAL,       //normal mode
    APP_LAMP_PRODTEST      //prodact test mode
}APP_LAMP_MODE;

#define DPID_DELAY_OFF               20
#define DPID_LIGHT_MODE              21
#define DPID_LIGHT_VALUE             22
#define DPID_SIT_REMIND              101
#define DPID_AUTO_LIGHT              102
#define DPID_LOW_POW_ALARM           103

/********************************************************************************
 * FUNCTION:       app_plant_init
 * DESCRIPTION:    application initialization
 * INPUT:          mode:application mode
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
OPERATE_RET app_lamp_init(IN APP_LAMP_MODE mode);

/********************************************************************************
 * FUNCTION:       deal_dp_proc
 * DESCRIPTION:    deal the data sented by app
 * INPUT:          root:app issued data structure
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         none
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID deal_dp_proc(IN CONST TY_OBJ_DP_S *root);

/*********************************************************************************
 * FUNCTION:       app_report_all_dp_status
 * DESCRIPTION:    report all dp date
 * INPUT:          none
 * OUTPUT:         none
 * RETURN:         none
 * OTHERS:         dp_cnt needs to be modified when adding or deleting the dp function
 * HISTORY:        2021-01-12
 *******************************************************************************/
VOID app_report_all_dp_status(VOID);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __LIHGT_INIT_H__ */