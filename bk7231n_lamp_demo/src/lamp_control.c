/*
 * @Author: zgw
 * @email: liang.zhang@tuya.com
 * @LastEditors: zgw
 * @file name: sht21.h
 * @Description: SHT21 IIC drive src file
 * @Copyright: HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
 * @Company: http://www.tuya.com
 * @Date: 2020-12-16 18:51:29
 * @LastEditTime: 2021-03-26 16:31:08
 */

#include "lamp_control.h"
//#include "soc_adc.h"
#include "tuya_gpio.h"
#include "tuya_uart.h"
#include "BkDriverUart.h"
#include "lamp_pwm.h"
#include "sys_timer.h"
#include "app_key.h"
#include "sh1106.h"
#include "uni_time.h"
#include "soc_adc.h"
#include "tuya_hal_thread.h"
/***********************************************************
*************************types define***********************
***********************************************************/
typedef enum
{
    LOW = 0,
    HIGH,
}default_level;

USHORT_T user_pwm_duty = 300;
UCHAR_T alert_count = 0;
LAMP_CTRL_DATA_T lamp_ctrl_data = {0};
UCHAR_T light_mode_old = 0;
UCHAR_T Lamp_switch_old = 0;

STATIC TIMER_ID beep_timer;
STATIC TIMER_ID delay_off_timer;

UINT8_T key_buf = 0;
UINT8_T key_old = 0;
UINT8_T key_delay_cont = 0;

UINT8_T key_trg = 0x00;
UINT8_T key_cont = 0x00;

UINT8_T low_power_flag = 0;
/***********************************************************
*************************IO control device define***********
***********************************************************/
#define BEEP_PORT                    (14)
#define BEEP_LEVEL                   FALSE

#define SPI_IO                       (17)
/***********************************************************
*************************about OLED*************************
***********************************************************/
#define OLED_PIX_HEIGHT              (16)
CONST UCHAR_T diplay_buffer_time[] = {
/*--  文字:  0  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,

/*--  文字:  1  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,

/*--  文字:  2  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,

/*--  文字:  3  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00,

/*--  文字:  4  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24,

/*--  文字:  5  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00,

/*--  文字:  6  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00,

/*--  文字:  7  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00,

/*--  文字:  8  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,

/*--  文字:  9  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00,

/*--  文字:  :  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,

/*--  文字:  %  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xF0,0x08,0xF0,0x80,0x60,0x18,0x00,0x00,0x00,0x31,0x0C,0x03,0x1E,0x21,0x1E,0x00,

/*--  文字:  P  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,

/*--  文字:  O  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,

/*--  文字:  W  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00,

/*--  文字:  E  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,

/*--  文字:  R  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*--  文字:  T  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,

/*--  文字:  I  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,

/*--  文字:  M  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00,

/*--  文字:  E  --*/
/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
};

/***********************************************************
*************************about iic init*********************
***********************************************************/
#define IIC_SDA_PIN                         (22)
#define IIC_SCL_PIN                         (20)

STATIC sh1106_init_t sh1106_init_param = {IIC_SDA_PIN, IIC_SCL_PIN};



/***********************************************************
*************************about adc init*********************
***********************************************************/


#define TEMP_ADC_DATA_LEN           (4)

tuya_adc_dev_t tuya_adc;



/***********************************************************
*****************timer related functions********************
***********************************************************/
STATIC VOID __ctrl_gpio_init(CONST TY_GPIO_PORT_E port, CONST BOOL_T high)
{   
    //Set IO port as output mode
    tuya_gpio_inout_set(port, FALSE);
    //Set IO port level
    tuya_gpio_write(port, high);
}

STATIC VOID beep_timer_cb(UINT_T timerID, PVOID_T pTimerArg)
{
    __ctrl_gpio_init(BEEP_PORT, BEEP_LEVEL);
}

STATIC VOID delay_off_timer_cb(UINT_T timerID, PVOID_T pTimerArg)
{
    lamp_ctrl_data.Lamp_switch = FALSE;
    lamp_ctrl_data.Lamp_delay_off = FALSE;
    lamp_pwm_off();
}

STATIC VOID __ctrl_beep(USHORT_T time_ms)
{
    if(lamp_ctrl_data.Silent_mode) {
        return ;
    }
    __ctrl_gpio_init(BEEP_PORT, !BEEP_LEVEL);
    sys_start_timer(beep_timer,time_ms,TIMER_ONCE);    
}

STATIC VOID __delay_off(USHORT_T time_ms)
{
    if(!lamp_ctrl_data.Lamp_switch) {
        lamp_ctrl_data.Lamp_delay_off = FALSE;
        return ;
    }
    if(IsThisSysTimerRun(delay_off_timer) == TRUE) {
        return ;
    }
    sys_start_timer(delay_off_timer,time_ms,TIMER_ONCE);    
}


/***********************************************************
******************screen related functions******************
***********************************************************/
STATIC VOID lamp_display_time(VOID)
{   
    if(lamp_ctrl_data.Lamp_switch != TRUE) {
        return ;
    }

    UCHAR_T i = 0;
    POSIX_TM_S cur_time; 
    PR_DEBUG("enter ctrl handle");
    if( uni_local_time_get(&cur_time) != OPRT_OK ) {
        PR_NOTICE("cant get local time");
        //return;
    }
    PR_DEBUG("display time!!!!!!!");

    lamp_ctrl_data.time_hour = cur_time.tm_hour;
    lamp_ctrl_data.time_min = cur_time.tm_min;
    
    for(i = 4; i < 8; i++) {
        tuya_sh1106_gram_point_set_test(0,i,&diplay_buffer_time[(i+14)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(1,i,&diplay_buffer_time[(i+14)*OLED_PIX_HEIGHT+8]);  
    }

    if(lamp_ctrl_data.time_hour < 10) {
        tuya_sh1106_gram_point_set_test(0,9,&diplay_buffer_time[0]);
        tuya_sh1106_gram_point_set_test(1,9,&diplay_buffer_time[8]);
    }else {
        tuya_sh1106_gram_point_set_test(0,9,&diplay_buffer_time[(lamp_ctrl_data.time_hour/10)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(1,9,&diplay_buffer_time[(lamp_ctrl_data.time_hour/10)*OLED_PIX_HEIGHT+8]);
    }
    tuya_sh1106_gram_point_set_test(0,10,&diplay_buffer_time[(lamp_ctrl_data.time_hour%10)*OLED_PIX_HEIGHT]);
    tuya_sh1106_gram_point_set_test(1,10,&diplay_buffer_time[(lamp_ctrl_data.time_hour%10)*OLED_PIX_HEIGHT+8]);

    //flicker effect of ':'
    tuya_sh1106_gram_point_set_test(0,11,&diplay_buffer_time[10*OLED_PIX_HEIGHT]);
    tuya_sh1106_gram_point_set_test(1,11,&diplay_buffer_time[10*OLED_PIX_HEIGHT+8]);

    if(lamp_ctrl_data.time_min < 10) {
        tuya_sh1106_gram_point_set_test(0,12,&diplay_buffer_time[0]);
        tuya_sh1106_gram_point_set_test(1,12,&diplay_buffer_time[8]);
    }else {
        tuya_sh1106_gram_point_set_test(0,12,&diplay_buffer_time[(lamp_ctrl_data.time_min/10)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(1,12,&diplay_buffer_time[(lamp_ctrl_data.time_min/10)*OLED_PIX_HEIGHT+8]);
    }
    
    tuya_sh1106_gram_point_set_test(0,13,&diplay_buffer_time[(lamp_ctrl_data.time_min%10)*OLED_PIX_HEIGHT]);
    tuya_sh1106_gram_point_set_test(1,13,&diplay_buffer_time[(lamp_ctrl_data.time_min%10)*OLED_PIX_HEIGHT+8]);
        
    tuya_sh1106_display();
}


STATIC VOID lamp_display_power(VOID)
{   
    if(lamp_ctrl_data.Lamp_switch != TRUE) {
        return ;
    }

    UCHAR_T i = 0;
    for(i = 4; i < 9; i++) {
        tuya_sh1106_gram_point_set_test(2,i,&diplay_buffer_time[(i+8)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(3,i,&diplay_buffer_time[(i+8)*OLED_PIX_HEIGHT+8]);
    }

    //flicker effect of ':'
    tuya_sh1106_gram_point_set_test(2,9,&diplay_buffer_time[17*OLED_PIX_HEIGHT]);
    tuya_sh1106_gram_point_set_test(3,9,&diplay_buffer_time[17*OLED_PIX_HEIGHT+8]);

    if(lamp_ctrl_data.Battery_remain == 100) {
        tuya_sh1106_gram_point_set_test(2,10,&diplay_buffer_time[OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(3,10,&diplay_buffer_time[OLED_PIX_HEIGHT+8]);

        tuya_sh1106_gram_point_set_test(2,11,&diplay_buffer_time[0]);
        tuya_sh1106_gram_point_set_test(3,11,&diplay_buffer_time[8]);
        tuya_sh1106_gram_point_set_test(2,12,&diplay_buffer_time[0]);
        tuya_sh1106_gram_point_set_test(3,12,&diplay_buffer_time[8]);
    }else {
        tuya_sh1106_gram_point_set_test(2,10,&diplay_buffer_time[17*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(3,10,&diplay_buffer_time[17*OLED_PIX_HEIGHT+8]);

        tuya_sh1106_gram_point_set_test(2,11,&diplay_buffer_time[(lamp_ctrl_data.Battery_remain/10)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(3,11,&diplay_buffer_time[(lamp_ctrl_data.Battery_remain/10)*OLED_PIX_HEIGHT+8]);
        tuya_sh1106_gram_point_set_test(2,12,&diplay_buffer_time[(lamp_ctrl_data.Battery_remain%10)*OLED_PIX_HEIGHT]);
        tuya_sh1106_gram_point_set_test(3,12,&diplay_buffer_time[(lamp_ctrl_data.Battery_remain%10)*OLED_PIX_HEIGHT+8]);
    }

    tuya_sh1106_gram_point_set_test(2,13,&diplay_buffer_time[11*OLED_PIX_HEIGHT]);
    tuya_sh1106_gram_point_set_test(3,13,&diplay_buffer_time[11*OLED_PIX_HEIGHT+8]);

}

/***********************************************************
**************touch button related functions****************
***********************************************************/
STATIC VOID lamp_key_event(UINT8_T key_event)
{
    if(key_event == KEY_CODE_SWITCH) {
        PR_NOTICE("--------------POWER ON!!!!!!!-------------");
        if(lamp_ctrl_data.Lamp_switch == FALSE) {
            lamp_ctrl_data.Lamp_switch = TRUE;
            lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty); 
        }else{
            lamp_ctrl_data.Lamp_switch = FALSE;
            lamp_pwm_off();    
        }
    }else if(key_event == KEY_CODE_SET_LIGHT_COLOR) {
        lamp_ctrl_data.Light_mode++;
        if(lamp_ctrl_data.Light_mode > 2){
            lamp_ctrl_data.Light_mode = 0;
        }
        lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
        PR_NOTICE("-----------change light mode to %d-------------",lamp_ctrl_data.Light_mode);
    }
    else if(key_event == KEY_CODE_UP) {
        if(user_pwm_duty != 600) {
            if(user_pwm_duty > 400){
                user_pwm_duty = 600;
            }else{
                user_pwm_duty += 200;
            }
            lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
            PR_NOTICE("-----------PWM_VALUE UP ONCE-------------");
        }
    }
    else if(key_event == KEY_CODE_DOWN) {
        if(user_pwm_duty != 0) {
            if(user_pwm_duty < 200){
                user_pwm_duty = 0;
            }else{
                user_pwm_duty -= 200;
            }
            lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
            PR_NOTICE("-----------PWM_VALUE DOWN ONCE-------------");
        }
    }
    else if(key_event == KEY_CODE_SET_BEEP) {
        lamp_ctrl_data.Silent_mode = !lamp_ctrl_data.Silent_mode;
        PR_NOTICE("-----------SET BEEP-------------");
    }
        __ctrl_beep(100);
}

VOID lamp_key_poll(VOID)
{
    app_key_scan(&key_trg,&key_cont);

    switch (key_cont)
    {
    case KEY_CODE_RELEASE:
        if(key_buf != 0) {
            lamp_key_event(key_buf);
        }
        key_buf = 0;
        key_old = KEY_CODE_RELEASE;
        break;
    case KEY_CODE_SWITCH:
        vTaskDelay(10);
        app_key_scan(&key_trg,&key_cont);
        if(key_cont == KEY_CODE_SWITCH) {
            key_buf = KEY_CODE_SWITCH;
        }
        key_old = KEY_CODE_SWITCH;
        break;
    case KEY_CODE_SET_LIGHT_COLOR:
        if(lamp_ctrl_data.Lamp_switch == FALSE) {
            key_buf = 0;
            return ;
        }
        vTaskDelay(10);
        app_key_scan(&key_trg,&key_cont);
        if(key_cont == KEY_CODE_SET_LIGHT_COLOR) {
            key_buf = KEY_CODE_SET_LIGHT_COLOR;
        }
        key_old = KEY_CODE_SET_LIGHT_COLOR;
        break;
    case KEY_CODE_UP:
        if(lamp_ctrl_data.Lamp_switch == FALSE) {
            key_buf = 0;
            return ;
        }
        if(key_old == KEY_CODE_UP) {
            key_delay_cont++;
        }else{
            key_delay_cont = 0;
        }

        if(key_delay_cont >= 2) {
            key_buf = KEY_CODE_UP;
        }

        if(key_delay_cont >= 40) {
            key_buf = 0;
            if(user_pwm_duty <= 590) {
                user_pwm_duty += 10;
                lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
            }
        }
        
        key_old = KEY_CODE_UP;
        break;
    case KEY_CODE_DOWN:
        if(lamp_ctrl_data.Lamp_switch == FALSE) {
            key_buf = 0;
            return ;
        }
        if(key_old == KEY_CODE_DOWN) {
            key_delay_cont++;
        }else{
            key_delay_cont = 0;
        }

        if(key_delay_cont >= 2) {
            key_buf = KEY_CODE_DOWN;
        }

        if(key_delay_cont >= 40) {
            key_buf = 0;
            if(user_pwm_duty>=10) {
                user_pwm_duty -= 10;
                lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
            } 
        }
        
        key_old = KEY_CODE_DOWN;        
        break;
    case KEY_CODE_SET_BEEP:
        vTaskDelay(10);
        app_key_scan(&key_trg,&key_cont);
        if(key_cont == KEY_CODE_SET_BEEP) {
            key_buf = KEY_CODE_SET_BEEP;
        }
        break;
    case KEY_CODE_DELAY_OFF:
        
        break;           
    default:
        break;
    }
 
}



/***********************************************************
******************radar related functions*******************
***********************************************************/
STATIC CHAR_T get_radar_data(UCHAR_T *data, UCHAR_T size)
{
    UCHAR_T buffer[100];
    UCHAR_T i,j;
    memset(buffer, 0, sizeof(buffer));
    bk_uart_recv(TY_UART1, buffer, 99, 50);
    for(i = 0; i < 100; i++) {
        if((buffer[i] == 'S')&&((i + size) < 100)) {
            for(j = 0; j < size; j++) {
                if(buffer[i+j] != 'L') {
                    data[j] = buffer[i+j];
                }else{
                    return 0;
                }
            }
            //未找到换行符
            return -1;
        }
    }
    //未找到'S'
    return -1;
}

VOID lamp_get_sensor_data(VOID)
{   

    UCHAR_T data[50];
    memset(data, 0, sizeof(data));

    CHAR_T opt;
    opt = get_radar_data(data,50);
    if(opt == 0){
        UCHAR_T i;
        if((data[0] == 'S')&&(data[6] == ':')) {
            if(data[8] == '[') {
                lamp_ctrl_data.Radar_sensor = TRUE;
            }else {
                lamp_ctrl_data.Radar_sensor = FALSE;
                lamp_ctrl_data.Human_distance = 0;
                PR_NOTICE("--------NO MAN AROUND-----------");
            }  
        }
        if(lamp_ctrl_data.Radar_sensor == FALSE) {
            return ;
        }
        for(i=0;i<50;i++) {
            if(data[i]=='R') {
                if((data[i+8] >= '0')&&(data[i+8] <= '9')) {
                    lamp_ctrl_data.Human_distance = ((data[i+7] - '0') * 10) + (data[i+8] - '0');
                }else {
                    lamp_ctrl_data.Human_distance = (data[i+7] - '0');
                }
                PR_NOTICE("--------Human_distance = %d-----------",lamp_ctrl_data.Human_distance);
                return ;        
            }    
        }
    }
}



/***********************************************************
************************other functions*********************
***********************************************************/
VOID lamp_device_init(VOID)
{
    INT_T op_ret = 0;
    
    op_ret = sys_add_timer(beep_timer_cb, NULL, &beep_timer);
    if(op_ret != OPRT_OK) {
        PR_ERR("add light timer failed");
    }


    op_ret = sys_add_timer(delay_off_timer_cb, NULL, &delay_off_timer);
    if(op_ret != OPRT_OK) {
        PR_ERR("add light timer failed");
    }
    
    __ctrl_gpio_init(SPI_IO,LOW);
    tuya_hal_system_sleep(1000);
    tuya_sh1106_init(&sh1106_init_param);
    tuya_hal_system_sleep(100);
    tuya_sh1106_clear();
    tuya_hal_system_sleep(100);

    lamp_pwm_init();
    
    //gpio init
    __ctrl_gpio_init(BEEP_PORT, BEEP_LEVEL);

    app_key_init();

    tuya_adc.priv.pData = Malloc(TEMP_ADC_DATA_LEN * sizeof(USHORT_T));
    memset(tuya_adc.priv.pData, 0, TEMP_ADC_DATA_LEN*sizeof(USHORT_T));
    tuya_adc.priv.data_buff_size = TEMP_ADC_DATA_LEN;
}


VOID STATIC lamp_light_control(VOID)
{   
    if((light_mode_old != lamp_ctrl_data.Light_mode)&&(lamp_ctrl_data.Lamp_switch == TRUE)) {
        lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
    }
    light_mode_old = lamp_ctrl_data.Light_mode;
    

    if(lamp_ctrl_data.Auto_light != TRUE) {
        return ;
    }

    if(lamp_ctrl_data.Radar_sensor == FALSE) {
        lamp_ctrl_data.Lamp_switch = FALSE;
        lamp_pwm_off();
    }else {
        if(lamp_ctrl_data.Lamp_switch == FALSE) {
            lamp_ctrl_data.Lamp_switch = TRUE;
            lamp_pwm_set(lamp_ctrl_data.Light_mode,user_pwm_duty);
        }
    }
}


VOID STATIC lamp_sit_remind(VOID)
{
    if(lamp_ctrl_data.Sit_remind != TRUE) {
        alert_count = 0;
        return ;
    }

    if((lamp_ctrl_data.Human_distance <= 5)&&(lamp_ctrl_data.Radar_sensor == TRUE)) {
        PR_NOTICE("------------enter sit remind-------------");
        alert_count++;
        if(alert_count >= 3) {
            __ctrl_beep(300);
        }
    }else{
        alert_count = 0; 
    }
}
 
VOID lamp_battery_detect(VOID)
{   
    USHORT_T adc_value = 0;
    float adc_voltage = 0.0;
    tuya_hal_adc_init(&tuya_adc);
    tuya_hal_adc_value_get(TEMP_ADC_DATA_LEN, &adc_value);
    PR_NOTICE("------------------adc_value = %d----------------",adc_value);
    adc_voltage = 2.4*((float)adc_value/2048);

    PR_NOTICE("------------------adc_voltage = %f----------------",adc_voltage);
    tuya_hal_adc_finalize(&tuya_adc);
    
    if((adc_voltage < 1.6)&&(low_power_flag == 0)) {
        low_power_flag = 1;
        tuya_gpio_write(SPI_IO, HIGH);
    }
    if((adc_voltage > 1.8)&&(low_power_flag == 1)) {
        low_power_flag = 0;
        tuya_gpio_write(SPI_IO, LOW);
        tuya_hal_system_sleep(1000);
        tuya_sh1106_init(&sh1106_init_param);
        tuya_hal_system_sleep(100);
        tuya_sh1106_clear();
        tuya_hal_system_sleep(100);
    }

    if(adc_voltage > 1.95) {
        lamp_ctrl_data.Battery_remain = 100;
        lamp_ctrl_data.Low_pow_alarm = 0;
        return ;
    }
    if(adc_voltage > 1.92) {
        lamp_ctrl_data.Battery_remain = 80;
        lamp_ctrl_data.Low_pow_alarm = 0;
        return ;
    }
    if(adc_voltage > 1.89) {
        lamp_ctrl_data.Battery_remain = 60;
        lamp_ctrl_data.Low_pow_alarm = 0;
        return ;
    }
    if(adc_voltage > 1.86) {
        lamp_ctrl_data.Battery_remain = 40;
        lamp_ctrl_data.Low_pow_alarm = 0;
        return ;
    }
    if(adc_voltage > 1.8) {
        lamp_ctrl_data.Battery_remain = 20;
        if(lamp_ctrl_data.Low_pow_alarm) {
            __ctrl_beep(300);
        }
        return ;
    }
}

VOID lamp_ctrl_handle(VOID)
{   
    lamp_light_control();

    lamp_sit_remind();

    if((lamp_ctrl_data.Lamp_switch != Lamp_switch_old)&&(lamp_ctrl_data.Lamp_switch == TRUE)) {
        tuya_sh1106_on();
        tuya_hal_system_sleep(150);
    }else if(lamp_ctrl_data.Lamp_switch == FALSE) {
        tuya_sh1106_off();
    }

    Lamp_switch_old = lamp_ctrl_data.Lamp_switch;

    lamp_display_time();

    lamp_display_power();

    if(lamp_ctrl_data.Lamp_delay_off) {
        PR_NOTICE("------------enter DELAY OFF-------------");
        __delay_off(30000);
    }
    
    lamp_battery_detect();
}


VOID lamp_ctrl_all_off(VOID)
{   

}

 