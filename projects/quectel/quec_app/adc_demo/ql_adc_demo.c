/**
 * @file ql_adc_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include <quec_adc/ql_adc.h>

#define ADC_SAMPLE_RATE         (64)

#define TEST_ADC_CHANNEL        (QL_ADC_CHANNEL4)
#define ADC_CHANNEL4_PIN        (GPIO_28)


#define QL_ADC_TEST_DEBUG
#ifdef QL_ADC_TEST_DEBUG
#define ql_adc_log  os_printf
#else
#endif

#define ADC_IO_FUNCTION_ENABLE(pin, func)   \
	do {                                    \
		gpio_dev_unmap(pin);                \
		gpio_dev_map(pin, func);            \
	} while (0)


void ql_adc_demo_thread(void *param)
{
    ql_adc_errcode_e ret;
    uint16_t adc_value      = 0;

    ql_rtos_task_sleep_ms(3000);

    ADC_IO_FUNCTION_ENABLE(ADC_CHANNEL4_PIN, GPIO_DEV_ADC4);

    ret = ql_adc_init(TEST_ADC_CHANNEL,
                      ADC_SAMPLE_RATE,
                      QL_ADC_SINGLE_STEP_MODE);

    if(QL_ADC_SUCCESS == ret){
        ql_adc_log("success: success adc init: %d\r\n",ret);
    }
    else{
        ql_adc_log("Error: Failed to init adc: %d\r\n",ret);
    } 
    
    while(1){
        ql_adc_start(TEST_ADC_CHANNEL);
        ql_adc_read(TEST_ADC_CHANNEL, &adc_value, 200);
        ql_adc_log("adc val=%d\n", adc_value);
        adc_value = 0;
        rtos_delay_milliseconds(2000);
    }
}

ql_task_t adc_test_thread_handle = NULL;
void ql_adc_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&adc_test_thread_handle,
                              (unsigned short)2048,
                              BEKEN_APPLICATION_PRIORITY,
                              "timer_test",
                              ql_adc_demo_thread,
                              0);

    if(ret != kNoErr){
        os_printf("Error: Failed to create adc test thread: %d\r\n", ret);
        goto init_err;
    }
    return;

init_err:
    if( adc_test_thread_handle != NULL ){
        ql_rtos_task_delete(adc_test_thread_handle);
    }
}