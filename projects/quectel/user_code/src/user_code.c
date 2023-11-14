/**
 * @file user_code.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "user_code.h"

void task_thread(void *param)
{
    while(1)
    {
       ql_rtos_task_sleep_ms(1000);
	   os_printf("Hello World\r\n");
    }
}



#define test_task_stack     4096
ql_task_t user_code_thread_handle = NULL;
void ql_user_code_demo_thread_create(void)
{
	int ret;
	ret = ql_rtos_task_create(&user_code_thread_handle,
							  (unsigned short)test_task_stack,
							  BEKEN_APPLICATION_PRIORITY,
							  "uart_test",
							  task_thread,
							  0);

	if (ret != kNoErr)
	{
		os_printf("Error: Failed to create uart test thread: %d\r\n", ret);
		goto init_err;
	}

	return;

init_err:
	if (user_code_thread_handle != NULL)
	{
		ql_rtos_task_delete(user_code_thread_handle);
	}
}


void ql_user_code_main(void)
{
#if CFG_ENABLE_USER_CODE
	ql_user_code_demo_thread_create();
#endif
}


