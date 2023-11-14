/**
 * @file ql_gpio_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include <quec_gpio/ql_gpio.h>
/***************gpio test******************/

#define ql_gpio_log os_printf

void (ql_gpio_input_cb)(gpio_id_t gpio_id)
{
    ql_gpio_log("gpio%d is tirgger, and level is : %d\r\n", gpio_id, ql_gpio_get_level(gpio_id));

}

void ql_gpio_demo_thread(void *param)
{
	ql_gpio_errcode_e ret;

	ql_rtos_task_sleep_ms(3000);

	ret = ql_gpio_init(QL_GPIO4, QL_GMODE_OUT_PP, QL_GPIO_LEVEL_HIGH);

    if(ret == QL_GPIO_SUCCESS)
	  ql_gpio_log("output init success\r\n");
	else
	  ql_gpio_log("output init fail\r\n");

	// ql_gpio_set_level(QL_GPIO25, QL_GPIO_LEVEL_LOW);
	ret = ql_gpio_init(QL_GPIO5, QL_GMODE_IN_PU, QL_GPIO_LEVEL_HIGH);
    if(ret == QL_GPIO_SUCCESS)
	  ql_gpio_log("input init success\r\n");
	else
	  ql_gpio_log("input init fail \r\n");

	ql_gpio_interrupt_init(QL_GPIO5, QL_IRQ_TRIGGER_RISING_EDGE, ql_gpio_input_cb);
    ql_gpio_interrupt_enable(QL_GPIO5, true);

	ql_gpio_log("gpio  test start \r\n");

	while(1)
	{
	  ql_rtos_task_sleep_ms(100);
      ql_gpio_set_level(QL_GPIO4, QL_GPIO_LEVEL_LOW);
      ql_rtos_task_sleep_ms(100);
      ql_gpio_set_level(QL_GPIO4, QL_GPIO_LEVEL_HIGH);
	}
}

ql_task_t	gpio_test_thread_handle = NULL;
void ql_gpio_demo_thread_create(void)
{
    int ret;
	ret = ql_rtos_task_create(&gpio_test_thread_handle,
		               (unsigned short)2048,
					   BEKEN_APPLICATION_PRIORITY,
					   "gpio_test",
					   ql_gpio_demo_thread,	   
					   0);

	if (ret != kNoErr) {
		os_printf("Error: Failed to create gpio test thread: %d\r\n",ret);
		goto init_err;
	}
	return;
   
	init_err:
	   if( gpio_test_thread_handle != NULL ) {
		   ql_rtos_task_delete(gpio_test_thread_handle);
	    }
}
