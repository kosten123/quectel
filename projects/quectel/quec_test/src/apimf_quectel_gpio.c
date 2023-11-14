/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"


void qlst_gpio_deinit(int param_num, char (*param_str)[PARAM_SIZE])
{
	int ret;
    if (param_num==0){
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
	ret = ql_gpio_deinit();
	quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);	
	quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR, param_num);
}
}
/**
 * @brief 
 * 
 * @param gpio_num 
 * @param mode 
 * @param levle 
 * @return ql_gpio_errcode_e 
 */
void qlst_gpio_init(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		int ret, gpio_num, level, mode;
		gpio_num = atoi(param_str[PARAM_NUM_1]);
		mode = atoi(param_str[PARAM_NUM_2]);
        level = atoi(param_str[PARAM_NUM_3]);
		
			ret = ql_gpio_init(gpio_num, mode, level);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d gpio_num=%d  mode=%d  level=%d\r\n", ret,gpio_num, mode, level);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}
}
/**
 * @brief 
 * 
 * @param gpio_num 
 * @param irq_trigger 
 * @param cb 
 * @return ql_gpio_errcode_e 
*			
*/
typedef void (*ql_gpio_irq_callback)(gpio_id_t gpio_id);
 void gpio_at_cmd_cb(gpio_id_t cmd)
{
    switch (cmd)
   {
    case  QL_IRQ_TRIGGER_LOW_LEVEL:
        quecst_printf(SYNC_LOG,"QL_IRQ_TRIGGER_LOW_LEVEL\r\n");
        break;
    case  QL_IRQ_TRIGGER_HIGH_LEVEL:
         quecst_printf(SYNC_LOG,"QL_IRQ_TRIGGER_HIGH_LEVEL\r\n");
        break;
    case  QL_IRQ_TRIGGER_RISING_EDGE:
         quecst_printf(SYNC_LOG," QL_IRQ_TRIGGER_RISING_EDGE\r\n");
        break;
    case QL_IRQ_TRIGGER_FALLING_EDGE:
         quecst_printf(SYNC_LOG,"  QL_IRQ_TRIGGER_FALLING_EDGE\r\n");
        break;
    case QL_IRQ_TRIGGER_DUAL_EDGE:
         quecst_printf(SYNC_LOG,"  QL_IRQ_TRIGGER_DUAL_EDGE\r\n");
        break;       
    default:
            break;
    }
    quecst_send_upper_data("%s=%d\r\n", "gpio_at_cmd_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	quecst_printf(SYNC_LOG, "gpio_cmd_t cmd:%d", cmd);
}
void qlst_gpio_interrupt_init (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		int ret ,gpio_num, irq_trigger;
		gpio_num = atoi(param_str[PARAM_NUM_1]);
        irq_trigger = atoi(param_str[PARAM_NUM_2]);
	
			ret = ql_gpio_interrupt_init(gpio_num,irq_trigger,gpio_at_cmd_cb);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE, CURRENT_FUNC_TYPE);		
			quecst_printf(SYNC_LOG, ":re=%d gpio_num=%d irq_trigger=%d\r\n", ret, gpio_num, irq_trigger);
		}
	   	else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}
	}

/**
 * @brief 
 * 
 * @param gpio_num 
 * @param enable 
 * @return ql_gpio_errcode_e 
 */
void qlst_gpio_interrupt_enable (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret ,gpio_num,enable ;
		gpio_num = atoi(param_str[PARAM_NUM_1]);
        enable = atoi(param_str[PARAM_NUM_2]);
		
			ret = ql_gpio_interrupt_enable(gpio_num,  enable);

		quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE, CURRENT_FUNC_TYPE);		
		quecst_printf(SYNC_LOG, ":re=%d gpio_num%d enable=%d\r\n", ret, gpio_num, enable);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}
	   
	}

 /**
 * @param gpio_num 
 * @return ql_gpio_level_e 
 */
void qlst_gpio_get_level(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int gpio_num ;
		gpio_num = atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_gpio_get_level(gpio_num);
        	quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QL_GPIO_LEVEL_LOW:QL_GPIO_LEVEL_HIGH);
			quecst_printf(SYNC_LOG, "re:%d  gpio_num:%d\r\n", ret, gpio_num);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}
		
	} 

 
/**
@brief 
 * 
 * @param gpio_num 
 * @return ql_gpio_errcode_e 
*/   
void qlst_gpio_toggle_level(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int gpio_num;
		gpio_num = atoi(param_str[PARAM_NUM_1]);	
		
			ret = ql_gpio_toggle_level(gpio_num);
       
			quecst_send_upper_data("%s=%d\r\n", funcid_str ,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d gpio_num:%d\r\n", ret, gpio_num);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}
		
	}


/**
 * @brief 
 * 
 * @param gpio_num 
 * @param level 
 * @return ql_gpio_errcode_e 
*/ 
void qlst_gpio_set_level(int param_num, char (*param_str)[PARAM_SIZE])
{  
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;
		int gpio_num=7;
		int level;
		gpio_num = atoi(param_str[PARAM_NUM_1]);
		level=atoi(param_str[PARAM_NUM_2]);
		
			ret = ql_gpio_set_level(gpio_num,level);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d gpio_num:%d level:%d\r\n ", ret,gpio_num,level);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_GPIO_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_GPIO_INVALID_PARAM_ERR, param_num);
		}

	}

