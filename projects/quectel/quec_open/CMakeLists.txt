set(SRC_FILES   ./quec_osi/ql_osi.c
                ./quec_gpio/ql_gpio.c
                ./quec_i2c/ql_i2c.c
                ./quec_adc/ql_adc.c
                ./quec_ble/ql_ble.c
                ./quec_flash/ql_flash.c
                ./quec_pwm/ql_pwm.c
                ./quec_spi/ql_spi.c
                ./quec_timer/ql_timer.c
                ./quec_uart/ql_uart.c
                ./quec_wdg/ql_wdg.c
                ./quec_wlan/ql_wlan.c
                ./quec_sys/ql_sys.c)

set(INCS ./
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
    $ENV{ARMINO_PATH}/middleware/dirver
    $ENV{ARMINO_PATH}/middleware/dirver/bk7256)

armino_component_register(SRCS ${SRC_FILES}
                    INCLUDE_DIRS ${INCS})