set(SRC_FILES ./src/user_code.c)
 

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
    $ENV{ARMINO_PATH}/middleware/driver
    $ENV{ARMINO_PATH}/middleware/driver/bk7256)

armino_component_register(SRCS ${SRC_FILES}
                    INCLUDE_DIRS ${INCS})