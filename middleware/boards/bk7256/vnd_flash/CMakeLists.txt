if (NOT CONFIG_OVERRIDE_FLASH_PARTITION)

set(incs .
	)
set(srcs
	vnd_flash.c
	)
endif()

armino_component_register(SRCS "${srcs}" INCLUDE_DIRS "${incs}" PRIV_REQUIRES bk_libs user_driver)
