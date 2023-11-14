set(SRC_FILES   ./ql_demo.c
                ./ql_demo_cli.c
                ./gpio_demo/ql_gpio_demo.c
                ./i2c_demo/ql_i2c_demo.c
                ./adc_demo/ql_adc_demo.c
                ./ble_demo/ql_ble_demo.c
                ./flash_demo/ql_flash_demo.c
                ./pwm_demo/ql_pwm_demo.c
                ./spi_demo/ql_spi_demo.c
                ./timer_demo/ql_timer_demo.c
                ./uart_demo/ql_uart_demo.c
                ./wdg_demo/ql_wdg_demo.c
                ./wlan_demo/ql_wlan_demo.c
                ./w5500_demo/wizchip_test.c
                ./websocket_demo/ql_websocket_demo.c
                ./websocket_demo/mongoose.c
                ./tls_demo/ql_tls_demo.c
                ./lowpwr_demo/demo_tcp_alive.c
                ./tcp_client/tcp_client_demo.c)

set(INCS ./
    ../quec_open
    ../quec_components/5500
    ../quec_components/5500/w5500
    $ENV{ARMINO_PATH}/include/common
    $ENV{ARMINO_PATH}/include/driver
    $ENV{ARMINO_PATH}/include/driver/hal
    $ENV{ARMINO_PATH}/include/modules
    $ENV{ARMINO_PATH}/include/os
    $ENV{ARMINO_PATH}/include/soc
    $ENV{ARMINO_PATH}/components/
    $ENV{ARMINO_PATH}/components/bk_cli
    $ENV{ARMINO_PATH}/components/bk_cli/include
    $ENV{ARMINO_PATH}/components/bk_cli/include/bk_private
    $ENV{ARMINO_PATH}/components/bk_rtos/include
    $ENV{ARMINO_PATH}/components/os_source/freertos_v10/include
    $ENV{ARMINO_PATH}/components/os_source/freertos_v10/portable/GCC/RISC-V
    $ENV{ARMINO_PATH}/components/bk_wifi/include/
    $ENV{ARMINO_PATH}/components/bk_wifi/include/bk_private
    $ENV{ARMINO_PATH}/components/bk_wifi/include/bk_private/legacy
    $ENV{ARMINO_PATH}/components/bk_rtos/freertos/v10
    $ENV{ARMINO_PATH}/components/lwip_intf_v2_1/lwip-2.1.2/port
    $ENV{ARMINO_PATH}/components/lwip_intf_v2_1/lwip-2.1.2/src/include
    $ENV{ARMINO_PATH}/components/lwip_intf_v2_1/lwip-2.1.2/src/include/lwip
    $ENV{ARMINO_PATH}/middleware/dirver
    $ENV{ARMINO_PATH}/middleware/dirver/bk7256
    $ENV{ARMINO_PATH}/components/mbedtls
    $ENV{ARMINO_PATH}/components/mbedtls/mbedtls-port/inc
    $ENV{ARMINO_PATH}/components/mbedtls/mbedtls/include
    $ENV{ARMINO_PATH}/components/mbedtls/mbedtls_ui)

armino_component_register(SRCS ${SRC_FILES}
                    INCLUDE_DIRS ${INCS})