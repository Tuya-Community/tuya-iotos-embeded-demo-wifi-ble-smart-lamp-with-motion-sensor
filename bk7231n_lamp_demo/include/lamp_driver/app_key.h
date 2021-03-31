/*
 * @file name: app_key.h
 * @Descripttion: 
 * @Author: zgw
 * @email: wuls@tuya.com
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2021-03-03 10:43:50
 * @LastEditors: zgw
 * @LastEditTime: 2021-03-03 16:17:35
 */
#ifndef __APP_KEY_H__
#define __APP_KEY_H__

#include "uni_log.h"
#include "tuya_cloud_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define KEY_CODE_RELEASE                   0x00                     
#define KEY_CODE_SWITCH                    0x08
#define KEY_CODE_SET_LIGHT_COLOR           0x04
#define KEY_CODE_UP                        0x02
#define KEY_CODE_DOWN                      0x01
#define KEY_CODE_SET_BEEP                  0x06
#define KEY_CODE_DELAY_OFF                 0x10

void app_key_init(void);
void app_key_scan(unsigned char *trg,unsigned char *cont);








#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif  /* __APP_KEY_H__ */