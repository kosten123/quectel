/**
 * @file ql_adc_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 *   QL_ADC_CHANNEL0,       0.4 * VBAT
 *   QL_ADC_CHANNEL1,       GPIO25
 *   QL_ADC_CHANNEL2,       GPIO24
 *   QL_ADC_CHANNEL3,       GPIO23
 *   QL_ADC_CHANNEL4,       GPIO28
 *   QL_ADC_CHANNEL5,       GPIO22
 *   QL_ADC_CHANNEL6,       GPIO21
 *   QL_ADC_CHANNEL7,       Inter tempeture Sensor
 * 
 * 
 */

#include <quec_adc/ql_adc.h>
#include<quec_common/ql_defines.h>

#define ADC_IO_FUNCTION_ENABLE(pin, func)       \
	do {                                        \
		gpio_dev_unmap(pin);                    \
		gpio_dev_map(pin, func);                \
	} while (0)

#define QL_BK7256_ADC_CLK               (750000)
#define QL_BK7256_ADC_FILTER            (32) //should less than 64
#define QL_BK7256_ADC_STEADY_CTRL       (7)
#define QL_BK7256_ADC_SRC_CLK           ADC_SCLK_XTAL_26M

#define adc_cfg_default { \
    .adc_filter = QL_BK7256_ADC_FILTER, \
    .adc_mode = QL_ADC_NONE_MODE, \
    .chan = 0, \
    .clk = QL_BK7256_ADC_CLK, \
    .is_open = 0, \
    .output_buf = NULL, \
    .output_buf_len = 0, \
    .sample_rate = 0, \
    .saturate_mode = ADC_SATURATE_MODE_3, \
    .src_clk = QL_BK7256_ADC_SRC_CLK, \
    .steady_ctrl = QL_BK7256_ADC_STEADY_CTRL, \
}

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
                            ql_adc_mode_e adc_mode)
{
    ql_adc_errcode_e ret;
    adc_config_t cfg = adc_cfg_default;

    if((chan<0||chan>7)||(adc_mode<0||adc_mode>4)){
        return QL_ADC_INVALID_PARAM_ERR;
    }

    bk_adc_driver_init();
    ret = bk_adc_init(chan);
    if(QL_ADC_SUCCESS != ret)
    {
        return ret;
    }

    cfg.chan            = chan;
    cfg.adc_mode        = adc_mode;
    cfg.sample_rate     = sample_rate;

    ret = bk_adc_set_config(&cfg);

    if(chan == QL_ADC_CHANNEL0){
        //VBAT monitor，VBAT*0.4
    }else if(chan == QL_ADC_CHANNEL1){
        ADC_IO_FUNCTION_ENABLE(GPIO_25, GPIO_DEV_ADC1);
    }else if(chan == QL_ADC_CHANNEL2){
        ADC_IO_FUNCTION_ENABLE(GPIO_24, GPIO_DEV_ADC2);
    }else if(chan == QL_ADC_CHANNEL3){
        ADC_IO_FUNCTION_ENABLE(GPIO_23, GPIO_DEV_ADC3);
    }else if(chan == QL_ADC_CHANNEL4){
        ADC_IO_FUNCTION_ENABLE(GPIO_28, GPIO_DEV_ADC4);
    }else if(chan == QL_ADC_CHANNEL5){
        ADC_IO_FUNCTION_ENABLE(GPIO_22, GPIO_DEV_ADC5);
    }else if(chan == QL_ADC_CHANNEL6){
        ADC_IO_FUNCTION_ENABLE(GPIO_21, GPIO_DEV_ADC6);
    }else if(chan == QL_ADC_CHANNEL7){
        //VTEMP ADC7
    }else if(chan == QL_ADC_CHANNEL8){

    }else if(chan == QL_ADC_CHANNEL9){

    }else{

    }
    return ret;
}


/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_deinit(ql_adc_channel_e chan)
{
    if(chan<0||chan>7){
        return QL_ADC_INVALID_PARAM_ERR;
    }
    return bk_adc_deinit(chan);
}

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_start(ql_adc_channel_e chan)
{
    if(chan<0||chan>7){
        return QL_ADC_INVALID_PARAM_ERR;
    }
    bk_adc_set_channel(chan);
    return bk_adc_start();
}

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_stop(ql_adc_channel_e chan)
{
    if(chan<0||chan>7){
        return QL_ADC_INVALID_PARAM_ERR;
    }
    return bk_adc_stop();
}


/**
 * @brief 
 * 
 * @param chan 
 * @param data 
 * @param timeout 
 * @return ql_adc_errcode_e 
 */
ql_adc_errcode_e ql_adc_read(ql_adc_channel_e chan, uint16_t *data, uint32_t timeout)
{
    if(chan<0||chan>7){
        return QL_ADC_INVALID_PARAM_ERR;
    }
    return bk_adc_read(data, timeout);
}

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
                                uint32_t timeout)
{
    if(chan<0||chan>7){
        return QL_ADC_INVALID_PARAM_ERR;
    }
    return bk_adc_read_raw(read_buf, size, timeout);
}
