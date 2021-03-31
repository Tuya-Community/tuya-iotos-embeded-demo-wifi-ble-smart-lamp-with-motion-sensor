# Tuya IoTOS Embeded Demo WiFi & BLE smart lamp with motion sensor

[English](./README.md) | [中文](./README_zh.md)

## Introduction 


This Demo is based on Tuya Smart Cloud Platform, Tuya Smart APP, IoTOS Embeded WiFi &Ble SDK, using Tuya WiFi/WiFi+BLE series modules to build a smart desk lamp demo with the following functions.
+ OLED screen to display time and power
+ 4 touch keys, long press, short press and key combination three key triggering methods
+ Support setting buzzer mute mode, gear dimming or infinitely dimming, setting sitting posture reminder, etc., while supporting mobile app remote control
+ Support automatic light switch and sitting posture reminder alarm


## Quick start

### Compile and burn
+ Download [Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231n) 

+ Download the demo to the apps directory of the SDK 

  ```bash
  $ cd apps
  $ git clone https://github.com/Tuya-Community/tuya-iotos-embeded-demo-wifi-ble-smart-lamp-with-motion-sensor
  ```
  
+ Execute the following command in the SDK root directory to start compiling.

  ```bash
  sh build_app.sh apps/bk7231n_lamp_demo bk7231n_lamp_demo 1.0.0 
  ```

+ Firmware burn-in license information please refer to: [Wi-Fi + BLE series module burn-in license](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd) 



### File description

```
├── src	
|    ├── app_driver
|    |    ├── lamp_pwm.c           //Desk lamp PWM driver related files
|    |    ├── sh1106.c             //OLED screen driver related files
|    |    └── app_key.c            //touch key related code file
|    ├── app_soc                   //tuya SDK soc layer interface related files
|    ├── tuya_device.c             //Application layer entry file
|    ├── app_lamp.c            //Main application layer
|    └── lamp_control.c             //Key-related logic
|
├── include				//Header file directory
|    ├── app_driver
|    |    ├── lamp_pwm.c       
|    |    ├── sh1106.c       
|    |    └── app_key.c            
|    ├── app_soc
|    ├── tuya_device.h
|    ├── app_lamp.h
|    └── lamp_control.h
|
└── output              //Compilation products
```

<br>

### Application entry
Entry file: tuya_device.c

Important functions: device_init()

+ Call tuya_iot_wf_soc_dev_init_param() interface to initialize the SDK, configure the operating mode, the mating mode, and register various callback functions and store the firmware key and PID.
+ Calling the tuya_iot_reg_get_wf_nw_stat_cb() interface to register the device network status callback functions.
+ Call the application layer initialization function app_lamp_init()

<br>

### dp point related

+ Send down dp point data stream: dev_obj_dp_cb() -> deal_dp_proc()
+ Report dp point interface: dev_report_dp_json_async()

| function name | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt)|
| ---|--|
| devid | device id (if it is a gateway, MCU, SOC class device then devid = NULL; if it is a sub-device, then devid = sub-device_id)|
| dp_data | dp structure array name|
| cnt | number of elements of the dp structure array|
| return | OPRT_OK: Success Other: Failure |

### I/O List


|sh1106|Beep|KEY|radar|
| --- | --- | --- | --- |
|`SDA` P22|`IO` P14|`on/off` P26|`radar_TX` SOC_RX2|
|`SCL` P20||`set` P28|
|||`up` P7|
|||`down` P8|

<br>



## Related Documents

Tuya Demo Center: https://developer.tuya.com/demo


<br>


## Technical Support

You can get support for Tuya by using the following methods:

- Developer Center: https://developer.tuya.com
- Help Center: https://support.tuya.com/help
- Technical Support Work Order Center: [https://service.console.tuya.com](https://service.console.tuya.com/)  


<br>


