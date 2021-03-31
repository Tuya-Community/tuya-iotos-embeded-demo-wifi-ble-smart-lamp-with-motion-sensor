# Tuya IoTOS Embeded Demo WiFi & BLE smart lamp with motion sensor

[English](./README.md) | [中文](./README_zh.md)

## 简介 


本 Demo 基于涂鸦智能云平台、涂鸦智能APP、IoTOS Embeded WiFi &Ble SDK，使用涂鸦WiFi/WiFi+BLE系列模组组建一个智能台灯demo，具有以下功能：
+ OLED屏显示时间和电量
+ 4个触摸按键，长按、短按、组合键三种按键触发方式
+ 支持设置蜂鸣器静音模式、档位调光或无极调光、设置坐姿提醒等，同时支持手机app远程控制
+ 支持自动开关灯及坐姿提醒告警


## 快速上手

### 编译与烧录
+ 下载[Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231n) 

+ 下载Demo至SDK目录的apps目录下 

  ```bash
  $ cd apps
  $ git clone https://github.com/Tuya-Community/tuya-iotos-embeded-demo-wifi-ble-smart-lamp-with-motion-sensor
  ```
  
+ 在SDK根目录下执行以下命令开始编译：

  ```bash
  sh build_app.sh apps/bk7231n_lamp_demo bk7231n_lamp_demo 1.0.0 
  ```

+ 固件烧录授权相关信息请参考：[Wi-Fi + BLE 系列模组烧录授权](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd) 



### 文件介绍
```
├── src	
|    ├── app_driver
|    |    ├── lamp_pwm.c           //台灯PWM驱动相关文件
|    |    ├── sh1106.c             //OLED屏驱动相关文件
|    |    └── app_key.c            //触摸按键相关代码文件
|    ├── app_soc                   //tuya SDK soc层接口相关文件
|    ├── tuya_device.c             //应用层入口文件
|    ├── app_lamp.c            //主要应用层
|    └── lamp_control.c             //按键相关逻辑
|
├── include				//头文件目录
|    ├── app_driver
|    |    ├── lamp_pwm.c       
|    |    ├── sh1106.c       
|    |    └── app_key.c            
|    ├── app_soc
|    ├── tuya_device.h
|    ├── app_lamp.h
|    └── lamp_control.h
|
└── output              //编译产物
```

<br>

### 应用入口
入口文件：tuya_device.c

重要函数：device_init()

+ 调用 tuya_iot_wf_soc_dev_init_param() 接口进行SDK初始化，配置了工作模式、配网模式，同时注册了各种回调函数并存入了固件key和PID。
+ 调用 tuya_iot_reg_get_wf_nw_stat_cb() 接口注册设备网络状态回调函数。
+ 调用应用层初始化函数 app_lamp_init()

<br>

### dp点相关

+ 下发dp点数据流：dev_obj_dp_cb() -> deal_dp_proc()
+ 上报dp点接口: dev_report_dp_json_async()

|函数名 | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt)|
|	---|---|
|    devid | 设备id（若为网关、MCU、SOC类设备则devid = NULL;若为子设备，则devid = sub-device_id)|
|    dp_data | dp结构体数组名|
|    cnt |dp结构体数组的元素个数|
|    Return    |  OPRT_OK: 成功  Other: 失败 |

### I/O 列表

|sh1106|Beep|KEY|radar|
| --- | --- | --- | --- |
|`SDA` P22|`IO` P14|`on/off` P26|`radar_TX` SOC_RX2|
|`SCL` P20||`set` P28|
|||`up` P7|
|||`down` P8|

<br>



## 相关文档

涂鸦Demo中心：https://developer.tuya.com/demo


<br>


## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com
- 帮助中心: https://support.tuya.com/help
- 技术支持工单中心: [https://service.console.tuya.com](https://service.console.tuya.com/) 


<br>


