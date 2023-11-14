set(SRC_FILES   ./src/quec_apimf.c
                ./src/quec_main.c
                ./src/apimf_quectel_ble.c
                ./src/apimf_quectel_wifi.c
                ./src/apimf_quectel_gpio.c
                ./src/apimf_quectel_i2c.c
                ./src/apimf_quectel_pwm.c
                ./src/apimf_quectel_spi.c
                ./src/apimf_quectel_wdg.c
                ./src/apimf_quectel_adc.c
                ./src/apimf_quectel_flash.c
                ./src/apimf_quectel_uart.c
                ./src/apimf_quectel_timer.c)
set(INCS ./inc
    ../quec_open
    $ENV{ARMINO_PATH}/include/common
    $ENV{ARMINO_PATH}/include/driver
    $ENV{ARMINO_PATH}/include/driver/hal
    $ENV{ARMINO_PATH}/include/modules
    $ENV{ARMINO_PATH}/include/os
    $ENV{ARMINO_PATH}/include/soc
    $ENV{ARMINO_PATH}/components/
    $ENV{ARMINO_PATH}/components/bk_rtos/include
    $ENV{ARMINO_PATH}/components/os_source/freertos_v10/include
    $ENV{ARMINO_PATH}/components/os_source/freertos_v10/portable/GCC/RISC-V
    $ENV{ARMINO_PATH}/components/bk_wifi/include/
    $ENV{ARMINO_PATH}/components/bk_wifi/include/bk_private
    $ENV{ARMINO_PATH}/components/bk_wifi/include/bk_private/legacy
    $ENV{ARMINO_PATH}/components/bk_rtos/freertos/v10
    $ENV{ARMINO_PATH}/components/lwip_intf_v2_1/lwip-2.1.2/port
    $ENV{ARMINO_PATH}/components/lwip_intf_v2_1/lwip-2.1.2/src/include
    $ENV{ARMINO_PATH}/middleware/dirver
    $ENV{ARMINO_PATH}/middleware/dirver/bk7256)

armino_component_register(SRCS ${SRC_FILES}
                    INCLUDE_DIRS ${INCS})