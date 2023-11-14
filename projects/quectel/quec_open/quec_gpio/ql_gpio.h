/**
 * @file ql_gpio.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _QL_GPIO_H
#define _QL_GPIO_H

#include<quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    QL_GPIO_SUCCESS             = 0,
    QL_GPIO_EXECUTE_ERR,
    QL_GPIO_INVALID_PARAM_ERR,
}ql_gpio_errcode_e;

typedef enum{
    QL_GPIO0    = 0,
    QL_GPIO1  ,
    QL_GPIO2  ,
    QL_GPIO3  ,
    QL_GPIO4  ,
    QL_GPIO5  ,
    QL_GPIO6  ,
    QL_GPIO7  ,
    QL_GPIO8  ,
    QL_GPIO9  ,
    QL_GPIO10 ,
    QL_GPIO11 ,
    QL_GPIO12 ,
    QL_GPIO13 ,
    QL_GPIO14 = 14,
    QL_GPIO15 ,
    QL_GPIO16 ,
    QL_GPIO17 ,
    QL_GPIO20 = 20,
    QL_GPIO21,
    QL_GPIO22,
    QL_GPIO23,
    QL_GPIO24,
    QL_GPIO25,    
    QL_GPIO26 = 26,
    QL_GPIO28 = 28,    
}ql_gpio_num_e;

typedef enum{  
    QL_GMODE_HIGH_IMPENDANCE = 0,
    QL_GMODE_INPUT,
    QL_GMODE_IN_FLOATING,
    QL_GMODE_IN_PD,
    QL_GMODE_IN_PU,
    QL_GMODE_OUTPUT,
    QL_GMODE_OUT_OD,
    QL_GMODE_OUT_PP,
}ql_gpio_mode_e;

typedef enum{
    QL_GPIO_LEVEL_LOW = 0,
    QL_GPIO_LEVEL_HIGH,
}ql_gpio_level_e;

typedef enum{
    QL_IRQ_TRIGGER_LOW_LEVEL    = 0,
    QL_IRQ_TRIGGER_HIGH_LEVEL,
    QL_IRQ_TRIGGER_RISING_EDGE,
    QL_IRQ_TRIGGER_FALLING_EDGE,
    QL_IRQ_TRIGGER_DUAL_EDGE,
}ql_gpio_irq_trigger_e;

typedef void (*ql_gpio_irq_callback)(gpio_id_t gpio_id);

/**
 * @brief 
 * 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_deinit(void);


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param mode 
 * @param levle 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_init(ql_gpio_num_e gpio_num, ql_gpio_mode_e mode, ql_gpio_level_e level);


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param irq_trigger 
 * @param cb 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_interrupt_init(ql_gpio_num_e gpio_num, ql_gpio_irq_trigger_e irq_trigger, ql_gpio_irq_callback cb);


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param enable 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_interrupt_enable(ql_gpio_num_e gpio_num, bool enable);


/**
 * @brief 
 * 
 * @param gpio_num 
 * @return ql_gpio_level_e 
 */
ql_gpio_level_e ql_gpio_get_level(ql_gpio_num_e gpio_num);

/**
 * @brief 
 * 
 * @param gpio_num 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_toggle_level(ql_gpio_num_e gpio_num);


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param level 
 * @return ql_gpio_errcode_e 
 */
ql_gpio_errcode_e ql_gpio_set_level(ql_gpio_num_e gpio_num, ql_gpio_level_e level);

#ifdef __cplusplus
}/*"C" */
#endif

#endif
