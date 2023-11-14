/**
 * @file ql_i2c_demo.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include <quec_i2c/ql_i2c.h>

#define QL_ADC_TEST_DEBUG
#ifdef QL_ADC_TEST_DEBUG
#define ql_i2c_log  os_printf
#else
#endif

#define QL_TEST_IIC_ID      QL_I2C0

static void ql_i2c_demo_thread(void *param)
{
    ql_i2c_errcode_e ret;
    uint8_t data = 0;
    ql_rtos_task_sleep_ms(3000);

    ret = ql_i2c_init(QL_TEST_IIC_ID, QL_I2C_SPEED_STANDARD, QL_I2C_ADDR_SIZE_7BIT, 0);

    if(QL_I2C_SUCCESS == ret){
        ql_i2c_log("success: success i2c init: %d\r\n",ret);
    }
    else{
        ql_i2c_log("Error: Failed to create i2c test thread: %d\r\n",ret);
    }

    while(1)
    {
        ql_i2c_master_mem_read(QL_TEST_IIC_ID, (0xBA >> 1), 0x80, QL_I2C_MEM_ADDR_SIZE_8BIT, &data, 1);
        ql_rtos_task_sleep_ms(200);
    }
}

ql_task_t i2c_test_thread_handle = NULL;
void ql_i2c_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&i2c_test_thread_handle,
                              (unsigned short)2048,
                              BEKEN_APPLICATION_PRIORITY,
                              "i2c_test",
                              ql_i2c_demo_thread,
                              0);
    if(ret != kNoErr) {
        ql_i2c_log("Error: Failed to create iic test thread: %d\r\n",ret);
        goto init_err;
    }
    return;

    init_err:
    if( i2c_test_thread_handle != NULL ) {
        ql_rtos_task_delete(i2c_test_thread_handle);
    }                              
}

