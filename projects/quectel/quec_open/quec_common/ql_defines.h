/**
 * @file ql_platform.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _QL_PLATFORM_H
#define _QL_PLATFORM_H


//common detail errcode, 2 bytes
typedef enum
{
	QL_NO_ERROR = 0,
	QL_SUCCESS = 0,
	QL_GENERAL_ERROR  = 0xFFFF,
} ql_errcode_e;

//stand libs include  files
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

//platform libs include files
#include "bk_include.h"
#include "bk_assert.h"
#include "bk_generic.h"
#include "bk_err.h"
#include "bk_kernel_err.h"
#include "bk_compiler.h"
#include "os.h"
#include "mem.h"
#include "str.h"

/**BLE*/
#include<modules/ble.h>
#include<modules/ble_types.h>

/**FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "rtos_impl.h"

//gpio
#include <driver/gpio.h>
#include <gpio_driver.h>
#include <driver/hal/hal_gpio_types.h>

//i2c
#include "i2c.h"

//adc
#include "adc.h"
#include <hal/hal_adc_types.h>
#include <gpio_driver.h>

/**Flash*/
#include "flash.h"
#include "flash_hal.h"
#include "flash_partition.h"

//pwm
#include "pwm.h"
#include "pwm_types.h"

//SPI
#include <dma.h>
#include <spi.h>
#include <hal/hal_spi_types.h>
#include <driver/hal/hal_dma_types.h>

//timer
#include <timer.h>
#include <timer_types.h>

//uart
#include<uart.h>
#include<hal/hal_uart_types.h>

//wdg
#include "wdt.h"

//wlan
#include <modules/wifi.h>
#include <modules/wifi_types.h>
#include <components/netif_types.h>
#include <components/netif.h>
#include <components/event.h>
#include <modules/ota.h>
#include <lwip_intf_v2_1/lwip-2.1.2/port/net.h>
#include "wlan_ui_pub.h"
#include "wlan_defs_pub.h"
#include "bk_rw_msg.h"
#include "param_config.h"
#include "bk_wifi.h"
#include "bk_wifi_v1.h"
#include "bk_wifi_rw.h"
#include "bk_wifi/include/bk_private/bk_wifi_types.h"



//defines
#define QL_LOG_ON_ERR(_x) do {\
	bk_err_t _err = (_x);\
	if (_err != BK_OK) {\
		BK_LOGE("error", "%s %d: ret=-0x%x\n", __FUNCTION__, __LINE__, -_err);\
        return BK_FAIL;\
	}\
} while(0)

#define QL_LOG_ON_ERR_VOID(_x) do {\
	bk_err_t _err = (_x);\
	if (_err != BK_OK) {\
		BK_LOGE("error", "%s %d: ret=-0x%x\n", __FUNCTION__, __LINE__, -_err);\
        return ;\
	}\
} while(0)

#define QL_DEV_ENABLE          1    // enable  device
#define QL_DEV_DISABLE         0    // disable device

#endif
