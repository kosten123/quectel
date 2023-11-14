/**
 * @file ql_pwm_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 *  PWM_CHANNEL_0       GPIO6
 *  PWM_CHANNEL_1       GPIO7
 *  PWM_CHANNEL_2       GPIO8
 *  PWM_CHANNEL_3       GPIO9
 *  PWM_CHANNEL_4       GPIO24
 *  PWM_CHANNEL_5       GPIO25
 *  PWM_CHANNEL_6       GPIO32
 *  PWM_CHANNEL_7       GPIO33
 *  PWM_CHANNEL_8       GPIO34
 *  PWM_CHANNEL_9       GPIO35
 *  PWM_CHANNEL_10      GPIO36
 *  PWM_CHANNEL_11      GPIO37
 */


#include<quec_pwm/ql_pwm.h>
#include<quec_common/ql_defines.h>

/**
 * @brief 
 * 
 * @param chan 
 * @param period  1/26MHz for fcm561d
 * @param duty 
 * @param level 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_init(ql_pwm_channel_e chan, uint32_t period, uint32_t duty, ql_pwm_init_level_e level)
{
    ql_pwm_errcode_e err;
    pwm_init_config_t cfg = {0};

    if((chan<0||chan>7)||(level<0||level>1)){
        return QL_PWM_INVALID_PARAM_ERR;
    }

    if(duty > period){
        return QL_PWM_INVALID_PARAM_ERR;
    }

    cfg.period_cycle    = period;
    cfg.duty_cycle      = duty;
    cfg.duty2_cycle     = 0;
    cfg.duty3_cycle     = 0;

    if(BK_OK != bk_pwm_init(chan, &cfg)){
        return QL_PWM_EXECUTE_ERR;
    }

    if(QL_PWM_INIT_LEVEL_LOW == level){
        err = bk_pwm_set_init_signal_low(chan);
    }else{
        err = bk_pwm_set_init_signal_high(chan);
    }
    return err;
}

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_deinit(ql_pwm_channel_e pwm_channel)
{
    if(pwm_channel<0||pwm_channel>11){
        return QL_PWM_INVALID_PARAM_ERR;
    }
    return bk_pwm_deinit(pwm_channel);
}

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_enable(ql_pwm_channel_e pwm_channel)
{
    if(pwm_channel<0||pwm_channel>11){
        return QL_PWM_INVALID_PARAM_ERR;
    }
    return bk_pwm_start(pwm_channel);
}

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_disable(ql_pwm_channel_e pwm_channel)
{
    if(pwm_channel<0||pwm_channel>11){
        return QL_PWM_INVALID_PARAM_ERR;
    }
    return bk_pwm_stop(pwm_channel);
}

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @param period 
 * @param duty_cycle 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_update_param(ql_pwm_channel_e pwm_channel, uint32_t period, uint32_t duty_cycle)
{
    if(pwm_channel<0||pwm_channel>11){
        return QL_PWM_INVALID_PARAM_ERR;
    }
    pwm_period_duty_config_t cfg = {0};

    cfg.period_cycle = period;
    cfg.duty_cycle   = duty_cycle;
    cfg.duty2_cycle  = 0;
    cfg.duty3_cycle  = 0;

    return bk_pwm_set_period_duty(pwm_channel, &cfg);    
}
