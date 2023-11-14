/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"

#define CALLBACK_FUNC_TYPE 	1
/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_ms 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
static uint32_t timer_cnt=0 ;
typedef void (*ql_timer_callback)(uint8_t arg);
   
   void timer_callback(uint8_t argv)
{  
     
     quecst_printf(SYNC_LOG,"timeout,cnt: %d", argv);
     timer_cnt ++;
 
    quecst_send_upper_data("%s=%d\r\n", "timer_callback", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	//quecst_printf(SYNC_LOG, "gpio_cmd_t cmd:%d", cmd);
}
void qlst_timer_init (int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){//3
		int ret,timer_id;
		uint32_t time_ms;
		timer_id= atoi(param_str[PARAM_NUM_1]);
		time_ms = atoi(param_str[PARAM_NUM_2]);
		quecst_printf(SYNC_LOG, "test timer_id=%d time_ms=%d  \r\n",timer_id, time_ms);
		
			ret = ql_timer_init(timer_id,time_ms, timer_callback);
			// quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE, CURRENT_FUNC_TYPE);		
			// quecst_printf(SYNC_LOG, ":re=%d timer_id=%d time_ms=%d  timer_callback=%d\r\n", ret, timer_id, time_ms, timer_callback);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d timer_id=%d time_ms=%d\r\n", ret, timer_id, time_ms);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_TIMER_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_TIMER_INVALID_PARAM_ERR, param_num);
		}		
	}


/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_ms 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
   
void qlst_timer_init_us (int param_num, char (*param_str)[PARAM_SIZE])
 {  
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret,timer_id;
         uint32_t time_ms;
	     timer_id= atoi(param_str[PARAM_NUM_1]);
         time_ms = atoi(param_str[PARAM_NUM_2]);
		
			ret = ql_timer_init_us(timer_id,  time_ms, timer_callback);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret);		
			quecst_printf(SYNC_LOG, ":re=%d timer_id=%d time_ms=%d\r\n", ret, timer_id, time_ms);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_TIMER_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_TIMER_INVALID_PARAM_ERR, param_num);
		}
        
	}

/**
 * @brief 
 * 
 * @param timer_id 
 * @param cont 
 * @return ql_timer_errcode_e 
 */
void qlst_timer_get_cnt (int param_num, char (*param_str)[PARAM_SIZE])
 {  
	int ret,timer_id;
	uint32_t cont=0;
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
	     timer_id= atoi(param_str[PARAM_NUM_1]);
         //cont = atoi(param_str[PARAM_NUM_2]);

         ret = ql_timer_get_cnt(timer_id, &cont);

		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret,cont);		
		quecst_printf(SYNC_LOG, ":re=%d timer_id=%d cont=%d\r\n", ret, timer_id, cont);
	}
	 else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR, param_num);
	}
	cont=0;
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
void qlst_timer_stop (int param_num, char (*param_str)[PARAM_SIZE])
 {  
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret,timer_id;        
		timer_id= atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_timer_stop(timer_id);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d timer_id=%d \r\n", ret, timer_id);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_TIMER_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_TIMER_INVALID_PARAM_ERR, param_num);
		}      
			
	}

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
void qlst_timer_deinit (int param_num, char (*param_str)[PARAM_SIZE])
 {  
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret,timer_id;        
		timer_id= atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_timer_deinit(timer_id);

			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d timer_id=%d \r\n", ret, timer_id);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_TIMER_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_TIMER_INVALID_PARAM_ERR, param_num);
		}        
			
	}


/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
void qlst_timer_start (int param_num, char (*param_str)[PARAM_SIZE])
 {  
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
         int ret,timer_id;        
		timer_id= atoi(param_str[PARAM_NUM_1]);
	
			ret = ql_timer_start(timer_id);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d timer_id=%d \r\n", ret, timer_id);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_TIMER_INVALID_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_TIMER_INVALID_PARAM_ERR, param_num);
		}        
			
	}

