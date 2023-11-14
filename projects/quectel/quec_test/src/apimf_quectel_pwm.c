/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"

/**
 * @brief 
 * 
 * @param chan 
 * @param period 
 * @param duty 
 * @param level 
 * @return ql_pwm_errcode_e 
 */
void qlst_pwm_init (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
		int ret,chan,level;
         uint32_t period, duty;
		chan = atoi(param_str[PARAM_NUM_1]);
        period = atoi(param_str[PARAM_NUM_2]);
		duty = atoi(param_str[PARAM_NUM_3]);
        level = atoi(param_str[PARAM_NUM_4]);
			ret = ql_pwm_init(chan , period, duty, level);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d chan=%d period=%d duty=%d level=%d\r\n", ret, chan , period, duty, level);
		}
	  	else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_PWM_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_PWM_INVALID_PARAM_ERR, param_num);
		}
	}


/**
 * @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
void qlst_pwm_deinit (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret ,pwm_channel ;
		pwm_channel  = atoi(param_str[PARAM_NUM_1]);
       
			ret = ql_pwm_deinit(pwm_channel);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d pwm_channel=%d\r\n", ret, pwm_channel );
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_PWM_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_PWM_INVALID_PARAM_ERR, param_num);
		}

	   
	}

/**
* @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
void qlst_pwm_enable (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret ,pwm_channel ;
		pwm_channel  = atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_pwm_enable(pwm_channel);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d pwm_channel=%d\r\n", ret, pwm_channel );
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_PWM_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_PWM_INVALID_PARAM_ERR, param_num);
		}

			
	}

/**
* @brief 
 * 
 * @param pwm_channel 
 * @return ql_pwm_errcode_e 
 */
void qlst_pwm_disable (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret ,pwm_channel ;
		pwm_channel  = atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_pwm_disable(pwm_channel);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d pwm_channel=%d\r\n", ret, pwm_channel );
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_PWM_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_PWM_INVALID_PARAM_ERR, param_num);
		}

			
	}

/**
 * @brief 
 * 
 * @param pwm_channel 
 * @param period 
 * @param duty_cycle 
 * @return ql_pwm_errcode_e 
 */
void qlst_pwm_update_param (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		int ret ,pwm_channel;
        uint32_t period,duty_cycle ;
		pwm_channel  = atoi(param_str[PARAM_NUM_1]);
        period  = atoi(param_str[PARAM_NUM_2]);
        duty_cycle  = atoi(param_str[PARAM_NUM_3]);
		
			ret = ql_pwm_update_param(pwm_channel, period, duty_cycle);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d pwm_channel=%d period=%d  duty_cycle=%d \r\n", ret, pwm_channel,period,duty_cycle );
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_PWM_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_PWM_INVALID_PARAM_ERR, param_num);
		}	
	}

