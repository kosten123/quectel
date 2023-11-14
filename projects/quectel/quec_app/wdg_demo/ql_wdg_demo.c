/**
 * @file ql_wdg_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include<quec_wdg/ql_wdg.h>

void ql_wdg_demo_thread(void *param)
{
    ql_wdg_init(2000);

    while(1){
        ql_wdg_reload();
        os_printf("wdg feed\r\n");
        ql_rtos_task_sleep_ms(1000);
    }
}

ql_task_t wdg_test_thread_handle = NULL;
void ql_wdg_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&wdg_test_thread_handle,
                              (unsigned short)1024,
                              BEKEN_APPLICATION_PRIORITY,
                              "wdg_test",
                              ql_wdg_demo_thread,
                              0);

    if(ret != kNoErr){
        os_printf("Error: Failed to create spi test thread: %d\r\n", ret);
        goto init_err;
    }
    return;

init_err:
    if( wdg_test_thread_handle != NULL ){
        ql_rtos_task_delete(wdg_test_thread_handle);
    }
}

