/**
 * @file ql_pwm_pub.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _QL_PWM_PUB_H
#define _QL_PWM_PUB_H

#include<quec_common/ql_platform.h>

typedef enum
{
    QL_PWM_0        = 0,
    QL_PWM_1,
    QL_PWM_2,
    QL_PWM_3,
    QL_PWM_4,
    QL_PWM_5,
    QL_PWM_6,
    QL_PWM_7,
    QL_PWM_8,
    QL_PWM_9,
    QL_PWM_10,
    QL_PWM_11,
}ql_pwm_channel_e;

typedef enum
{
    QL_PWM_INIT_LEVEL_LOW   = 0,
    QL_PWM_INIT_LEVEL_HIGH,
}ql_pwm_init_level_e;


typedef enum
{
    QL_PWM_SUCCESS      = 0,
    QL_PWM_EXECUTE_ERR,
    QL_PWM_INVALID_PARAM_ERR,
}ql_pwm_errcode_e;

/**
 * @brief 
 * 
 * @param chan 
 * @param period 
 * @param duty 
 * @param level 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_init(ql_pwm_channel_e chan, uint32_t period, uint32_t duty, ql_pwm_init_level_e level);

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_deinit(ql_pwm_channel_e pwm_channel);

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_enable(ql_pwm_channel_e pwm_channel);

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_disable(ql_pwm_channel_e pwm_channel);

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @param period 
 * @param duty_cycle 
 * @return ql_pwm_errcode_e 
 */
ql_pwm_errcode_e ql_pwm_update_param(ql_pwm_channel_e pwm_channel, uint32_t period, uint32_t duty_cycle);

#endif
