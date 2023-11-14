set(INCS ../quec_app
         ../quec_open
         ../user_code/inc
         ../quec_test/inc)


armino_component_register(SRCS "app_main.c"
                    INCLUDE_DIRS ${INCS})
