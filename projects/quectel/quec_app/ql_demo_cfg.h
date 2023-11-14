/**
 * @file ql_demo_cfg.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _QL_DEMO_CFG_H
#define _QL_DEMO_CFG_H

#define CFG_ENABLE_QUECTEL_DEMO_CLIS                1

#define CFG_ENABLE_QUECTEL_DEMO                     0
#define CFG_ENABLE_QUECTEL_ADC                      0
#define CFG_ENABLE_QUECTEL_BLE_PERIPHERA            0
#define CFG_ENABLE_QUECTEL_BLE_CENTRAL              0
#define CFG_ENABLE_QUECTEL_FLASH                    0
#define CFG_ENABLE_QUECTEL_GPIO                     0
#define CFG_ENABLE_QUECTEL_I2C                      0
#define CFG_ENABLE_QUECTEL_PWM                      0
#define CFG_ENABLE_QUECTEL_SPI                      0
#define CFG_ENABLE_QUECTEL_TIMER                    0
#define CFG_ENABLE_QUECTEL_UART                     0
#define CFG_ENABLE_QUECTEL_WDG                      0
#define CFG_ENABLE_QUECTEL_WLAN                     0
#define CFG_ENABLE_QUECTEL_W5500                    0
#define CFG_ENABLE_QUECTEL_OSI                      0

#define CFG_ENABLE_QUECTEL_LOWPOWER                 0
#define CFG_ENABLE_QUECTEL_BLE_CONFIG_NETWORK       0
#define CFG_ENABLE_QUECTEL_WLAN_CONFIG_NETWORK      0
#define CFG_ENABLE_QUECTEL_WEBSOCKET                0
#define CFG_ENABLE_QUECTEL_LOWPWR                   0
#define CFG_ENABLE_QUECTEL_TCP_CLIENT_DEMO          0

#if (CFG_ENABLE_QUECTEL_BLE_PERIPHERA == 1 && CFG_ENABLE_QUECTEL_BLE_CENTRAL == 1)
#error "no support"
#endif

#endif
