/**
 * @file ql_timer_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<quec_osi/ql_osi.h>
#include<quec_timer/ql_timer.h>

#define QL_SPI_TEST_DEBUG
#ifdef QL_SPI_TEST_DEBUG
#define ql_timer_log  os_printf
#else
#define ql_timer_log  os_null_printf
#endif

static uint32_t timer_cnt = 0;

void timer_callback(uint8_t argv)
{
    ql_timer_log("timeout,cnt: %d\n", timer_cnt);
    timer_cnt ++;
}

void ql_timer_demo_thread(void *param)
{
    ql_timer_errcode_e ret;
    ql_rtos_task_sleep_ms(3000);

    ret = ql_timer_init(QL_TIMER_0, 500, timer_callback);
    if(QL_TIMER_SUCCESS == ret){
        ql_timer_log("success: success spi init: %d\r\n",ret);
    }
    else{
        ql_timer_log("Error: Failed to create spi test thread: %d\r\n",ret);
    } 

    ql_timer_start(QL_TIMER_0);

    while(1){
        ql_rtos_task_sleep_ms(3000);
        ql_timer_log("ql rtos task sleep out,timer cnt:%d \r\n",timer_cnt);
        timer_cnt = 0;
    }
}


ql_task_t timer_test_thread_handle = NULL;
void ql_timer_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&timer_test_thread_handle,
                              (unsigned short)2048,
                              BEKEN_APPLICATION_PRIORITY,
                              "timer_test",
                              ql_timer_demo_thread,
                              0);

    if(ret != kNoErr){
        os_printf("Error: Failed to create spi test thread: %d\r\n", ret);
        goto init_err;
    }
    return;

init_err:
    if( timer_test_thread_handle != NULL ){
        ql_rtos_task_delete(timer_test_thread_handle);
    }
}
