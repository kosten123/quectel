/**
 * @file ql_adc.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_ADC_H
#define _QL_ADC_H


#include<quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef enum
{
    QL_ADC_CHANNEL0     = 0,
    QL_ADC_CHANNEL1,
    QL_ADC_CHANNEL2,
    QL_ADC_CHANNEL3,
    QL_ADC_CHANNEL4,
    QL_ADC_CHANNEL5,
    QL_ADC_CHANNEL6,
    QL_ADC_CHANNEL7,
    QL_ADC_CHANNEL8,
    QL_ADC_CHANNEL9,
    QL_ADC_CHANNEL10,
    QL_ADC_CHANNEL11,
    QL_ADC_CHANNEL12,
    QL_ADC_CHANNEL13,
    QL_ADC_MAX,
}ql_adc_channel_e;

typedef enum
{
    QL_ADC_SLEEP_MODE           = 0,
    QL_ADC_SINGLE_STEP_MODE,
    QL_ADC_SOFTWARE_CONTRL_MODE,
    QL_ADC_CONTINUOUS_MODE,
    QL_ADC_NONE_MODE,
}ql_adc_mode_e;

typedef enum
{
    QL_ADC_SUCCESS          = 0,
    QL_ADC_EXECUTE_ERR,
    QL_ADC_INVALID_PARAM_ERR,
}ql_adc_errcode_e;

/**
 * @brief 
 * 
 * @param chan 
 * @param sample_rate 
 * @param adc_mode 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_init(ql_adc_channel_e chan, 
                            uint32_t sample_rate, 
                            ql_adc_mode_e adc_mode);


/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_deinit(ql_adc_channel_e chan);

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_start(ql_adc_channel_e chan);

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_stop(ql_adc_channel_e chan);


/**
 * @brief 
 * 
 * @param chan 
 * @param data 
 * @param timeout 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_read(ql_adc_channel_e chan, uint16_t *data, uint32_t timeout);

/**
 * @brief 
 * 
 * @param chan 
 * @param read_buf 
 * @param size 
 * @param timeout 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_read_raw(ql_adc_channel_e chan, 
                                uint16_t *read_buf, 
                                uint32_t size, 
				                uint32_t timeout);

#ifdef __cplusplus
}
#endif
#endif

