/**
 * @file ql_gpio.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 * Beken chip supports abundant GPIO pins, some GPIOs can’t be used by the application:
 * In most Beken chips, UART2 is enabled by default and GPIO 0 and GPIO 1 are used by UART2.
 * Some GPIOs may be used by specific peripheral device, the application can’t use the GPIOs 
 * used by that device if the device is enabled by software. E.g. in BK7271, the SPI-1 can use GPIO 2/3/4/5, 
 * the application can’t use GPIO 2/3/4/5 if SPI-1 is enabled by the software, the application can still use 
 * GPIO 2/3/4/5 if the SPI-1 is disabled by software.Some GPIO groups may be used by specific peripheral device, 
 * the application can’t use that GPIO group if the device chooses that GPIO group and the device is enabled by software.
 * E.g. SPI-3 can use GPIO 30/31/32/33 or GPIO GPIO 36/37/38/39, if the software enable the SPI-3 and configure it to use
 * the first GPIO group (GPIO 30/31/32/33), the application can’t use GPIO 30/31/32/33, but the application can still use 
 * 2nd GPIO group (GPIO 36/37/38/39) if they are not used by other devices. The application can use both group if SPI-3 is
 * NOT enabled and other devices also don’t use them.
 */


#include <quec_gpio/ql_gpio.h>

static gpio_id_t ql_gpio_num2gpio_id(ql_gpio_num_e gpio_num)
{
    //TODO: covert the ql_gpio_num_e to gpio_id_t for bk7256
    return (gpio_id_t)gpio_num;
}


/**
 * @brief 
 * 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_deinit(void)
{
    return bk_gpio_driver_deinit();
}

/**
 * @brief 
 * 
 * @param gpio_num 
 * @param mode 
 * @param levle 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_init(ql_gpio_num_e gpio_num, ql_gpio_mode_e mode, ql_gpio_level_e level)
{
    gpio_config_t gpio_cfg;
    if((gpio_num<0||gpio_num>28)||(mode<0||mode>7)||(level<0||level>1)){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);

    //output mode
    if(QL_GMODE_OUT_OD == mode || QL_GMODE_OUT_PP == mode || QL_GMODE_OUTPUT == mode)
    {
        gpio_cfg.io_mode = GPIO_OUTPUT_ENABLE;
        gpio_dev_unmap(gpio_id); 
        gpio_cfg.func_mode = GPIO_SECOND_FUNC_DISABLE; //disable the gpio second function.
        bk_gpio_set_config(gpio_id, &gpio_cfg);
        bk_gpio_enable_output(gpio_id);
            if(QL_GPIO_LEVEL_HIGH == level){
        bk_gpio_set_output_high(gpio_id);
    }else{
        bk_gpio_set_output_low(gpio_id);
    }

    }else if(QL_GMODE_IN_FLOATING == mode || QL_GMODE_IN_PD == mode || QL_GMODE_IN_PU == mode){
        gpio_cfg.io_mode = GPIO_INPUT_ENABLE;
        if(QL_GMODE_IN_FLOATING == mode){
            gpio_cfg.pull_mode = GPIO_PULL_DISABLE;
        }else if(QL_GMODE_IN_PD == mode){
            gpio_cfg.pull_mode = GPIO_PULL_DOWN_EN;
        }else{
            gpio_cfg.pull_mode = GPIO_PULL_UP_EN;
        }
        gpio_dev_unmap(gpio_id); 
        gpio_cfg.func_mode = GPIO_SECOND_FUNC_DISABLE; //disable the gpio second function.
        bk_gpio_set_config(gpio_id, &gpio_cfg);
        bk_gpio_enable_input(gpio_id);
    }else{
        gpio_cfg.io_mode = GPIO_IO_DISABLE;
    }

    return QL_GPIO_SUCCESS;
}

/**
 * @brief 
 * 
 * @param gpio_num 
 * @param irq_trigger 
 * @param cb 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_interrupt_init(ql_gpio_num_e gpio_num, ql_gpio_irq_trigger_e irq_trigger, ql_gpio_irq_callback cb)
{
    if((gpio_num<0||gpio_num>28)||(irq_trigger<0||irq_trigger>5)){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    ql_gpio_errcode_e ret = 0;
    gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);
    ret =  bk_gpio_set_interrupt_type(gpio_id, irq_trigger);
    ret =  bk_gpio_register_isr(gpio_id, cb);
    return ret;
}


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param enable 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_interrupt_enable(ql_gpio_num_e gpio_num, bool enable)
{
    if(gpio_num<0||gpio_num>28){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);
    if(enable){
        return bk_gpio_enable_interrupt(gpio_id);
    }
    return bk_gpio_disable_interrupt(gpio_id);
}


/**
 * @brief 
 * 
 * @param gpio_num 
 * @return ql_gpio_level_e 
 */
ql_gpio_level_e ql_gpio_get_level(ql_gpio_num_e gpio_num)
{
    if(gpio_num<0||gpio_num>28){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);
    if(bk_gpio_get_input(gpio_id)){
        return QL_GPIO_LEVEL_HIGH;
    }else{
        return QL_GPIO_LEVEL_LOW;
    }
}

/**
 * @brief 
 * 
 * @param gpio_num 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_toggle_level(ql_gpio_num_e gpio_num)
{
    if(gpio_num<0||gpio_num>28){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    //TODO:for bk7256 this API need to develop more.
    // gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);
    return QL_GPIO_INVALID_PARAM_ERR;
}


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param level 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_set_level(ql_gpio_num_e gpio_num, ql_gpio_level_e level)
{
    if((gpio_num<0||gpio_num>28)||(level<0||level>1)){
        return QL_GPIO_INVALID_PARAM_ERR;
    }
    gpio_id_t gpio_id = ql_gpio_num2gpio_id(gpio_num);
    if(QL_GPIO_LEVEL_HIGH == level){
        return bk_gpio_set_output_high(gpio_id);
    }else{
        return bk_gpio_set_output_low(gpio_id);
    }
}
