/**
 * @file ql_demo.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_common/ql_defines.h>
#include "ql_demo.h"


#if CFG_ENABLE_QUECTEL_ADC
extern void ql_adc_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
extern void ql_ble_demo_periphera_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_BLE_CENTRAL
extern void ql_ble_demo_central_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_FLASH
extern void ql_flash_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_GPIO
extern void ql_gpio_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_I2C
extern void ql_i2c_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_PWM
extern void ql_pwm_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_SPI
extern void ql_spi_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_TIMER
extern void ql_timer_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_UART
extern void ql_uart_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_WDG
extern void ql_wdg_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_WLAN
extern void ql_wlan_test_demo_thread_create(void);
#endif
#if CFG_ENABLE_QUECTEL_W5500
extern void w5500_ethernet_test(void);
#endif
#if CFG_ENABLE_QUECTEL_WEBSOCKET
extern int tcp_server_init(void);
#endif

#if CFG_ENABLE_QUECTEL_LOWPWR
extern  void ql_lowpwr_test_demo_thread_create(void);
#endif

#if CFG_ENABLE_QUECTEL_TCP_CLIENT_DEMO
extern int ql_tcp_client_init(void);
#endif

static void ql_init_demo_start(void)
{
#if CFG_ENABLE_QUECTEL_LOWPWR
  ql_lowpwr_test_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_ADC
    ql_adc_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
    ql_ble_demo_periphera_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_BLE_CENTRAL
    ql_ble_demo_central_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_FLASH
    ql_flash_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_GPIO
    ql_gpio_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_I2C
    ql_i2c_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_PWM
    ql_pwm_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_SPI
    ql_spi_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_TIMER
    ql_timer_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_UART
    ql_uart_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_WDG
    ql_wdg_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_WLAN
    ql_wlan_test_demo_thread_create();
#endif

#if CFG_ENABLE_QUECTEL_W5500
   w5500_ethernet_test();
#endif

#if CFG_ENABLE_QUECTEL_WEBSOCKET
    tcp_server_init();
#endif

#if CFG_ENABLE_QUECTEL_TCP_CLIENT_DEMO
   ql_tcp_client_init();
#endif
}
void ql_demo_main(void)
{
#if CFG_ENABLE_QUECTEL_DEMO
    os_printf("\r\n ************************* QuecOpen API Demo Test Start ************************ \r\n");
    ql_init_demo_start();
#endif
}


