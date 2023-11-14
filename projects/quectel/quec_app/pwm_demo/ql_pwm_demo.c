/**
 * @file ql_pwm_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include<quec_pwm/ql_pwm.h>

void ql_pwm_demo_thread(void *param)
{
    ql_pwm_errcode_e ret;
    ql_rtos_task_sleep_ms(3000);

    ret = ql_pwm_init(QL_PWM_0, 5, 1, QL_PWM_INIT_LEVEL_HIGH);
    ret = ql_pwm_enable(QL_PWM_0);

    if(QL_PWM_SUCCESS == ret)
    {
        os_printf("success: success pwm init: %d\r\n",ret);
    }else
    {
        os_printf("Error: Failed to create pwm test thread: %d\r\n",ret);
    }

    while(1)
    {
        ql_rtos_task_sleep_ms(1000);
        ql_pwm_update_param(QL_PWM_0, 2, 1);
        ql_rtos_task_sleep_ms(1000);
        ql_pwm_update_param(QL_PWM_0, 50000, 30000);
    }
}

ql_task_t pwm_test_thread_handle = NULL;
void ql_pwm_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&pwm_test_thread_handle,
                              (unsigned short)2048,
                              BEKEN_APPLICATION_PRIORITY,
                              "pwm_test",
                              ql_pwm_demo_thread,
                              0);
    if(ret != kNoErr) {
        os_printf("Error: Failed to create pwm test thread: %d\r\n",ret);
        goto init_err;
    }
    return;

    init_err:
    if( pwm_test_thread_handle != NULL ) {
        ql_rtos_task_delete(pwm_test_thread_handle);
    }
}
