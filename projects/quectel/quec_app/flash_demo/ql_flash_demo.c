/**
 * @file ql_flash_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<quec_osi/ql_osi.h>
#include<quec_flash/ql_flash.h>

#define FLASH_TEST_DEBUG
#ifdef FLASH_TEST_DEBUG
#define ql_flash_log               os_printf

#else
#define ql_flash_log               os_null_printf
#endif


static void ql_flash_demo_thread(void *param)
{
    uint8_t outBuffer[256] = {0};
    uint8_t inBuffer[256] = {0};
    ql_rtos_task_sleep_ms(3000);

    ql_flash_log("\r\n-------- Flash Test Start --------\r\n");
    ql_flash_log("\r\nBefore Write Data,Flash Data is:\r\n");
    ql_flash_partition_read(QL_PARTITION_USR_CONFIG, 0, outBuffer, 64);
    for(uint8_t i = 0; i < 64; i++){
            ql_flash_log("0x%02x ", outBuffer[i]);
            if(0 == ((i + 1) % 16)){
                ql_flash_log("\n");
            }
            inBuffer[i] = i;
            outBuffer[i] = 0;
    }
    ql_flash_partition_write(QL_PARTITION_USR_CONFIG, 0, inBuffer, 64);
    ql_rtos_task_sleep_ms(3000);

    ql_flash_log("\r\nAfter Write Data,Flash Data is:\r\n");
    ql_flash_partition_read(QL_PARTITION_USR_CONFIG, 0, outBuffer, 64);
    for(uint8_t i = 0; i < 64; i++){
            ql_flash_log("0x%02x ", outBuffer[i]);
            if(0 == ((i + 1) % 16)){
                ql_flash_log("\n");
            }
            outBuffer[i] = 0;
    }

    ql_flash_partition_erase(QL_PARTITION_USR_CONFIG, 0, 64);
    ql_rtos_task_sleep_ms(3000);
    ql_flash_log("\r\nAfter Erase Flash Data,Flash Data is:\r\n");
    ql_flash_partition_read(QL_PARTITION_USR_CONFIG, 0, outBuffer, 64);
    for(uint8_t i = 0; i < 64; i++){
            ql_flash_log("0x%02x ", outBuffer[i]);
            if(0 == ((i + 1) % 16)){
                ql_flash_log("\n");
            }
    }
    ql_flash_log("\r\n-------- Flash Test OVER --------\r\n");

    while(1)
    {
        ql_rtos_task_sleep_ms(3000);
    }
}

ql_task_t flash_test_thread_handle = NULL;
void ql_flash_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&flash_test_thread_handle,
                              (unsigned short)2048,
                              BEKEN_APPLICATION_PRIORITY,
                              "flash_test",
                              ql_flash_demo_thread,
                              0);
    if(ret != kNoErr) {
        os_printf("Error: Failed to create flash test thread: %d\r\n",ret);
        goto init_err;
    }
    return;

    init_err:
    if( flash_test_thread_handle != NULL ) {
        ql_rtos_task_delete(flash_test_thread_handle);
    }   
}
