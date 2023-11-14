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
 * @param sample_rate 
 * @param adc_mode 
 * @return ql_adc_errcode_e 
 */
void qlst_adc_init(int param_num, char (*param_str)[PARAM_SIZE])         
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		 int ret, chan , adc_mode ;
		 chan  = atoi(param_str[PARAM_NUM_1]);
		 uint32_t sample_rate  = atoi(param_str[PARAM_NUM_2]);
         adc_mode = atoi(param_str[PARAM_NUM_3]);
		 
			ret = ql_adc_init(chan,sample_rate, adc_mode);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
		 	quecst_printf(SYNC_LOG, ":re=%d chan=%d sample_rate=%d  adc_mode=%d\r\n", ret, chan ,sample_rate, adc_mode);
	 } else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
	 }

	}

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
void qlst_adc_deinit(int param_num, char (*param_str)[PARAM_SIZE])         
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int chan ;
		chan = atoi(param_str[PARAM_NUM_1]);
			ret = ql_adc_deinit (chan);
				quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
				quecst_printf(SYNC_LOG, "re:%d  chan:%d\r\n", ret,chan);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
		}
	} 
    

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
void qlst_adc_start(int param_num, char (*param_str)[PARAM_SIZE])      
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int chan ;
		chan = atoi(param_str[PARAM_NUM_1]);
			ret = ql_adc_start(chan);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d  chan:%d\r\n", ret,chan);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
		}
	} 
    

/**
 * @brief 
 * 
 * @param chan 
 * @return ql_adc_errcode_e 
 */
void qlst_adc_stop(int param_num, char (*param_str)[PARAM_SIZE])      
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int chan ;
		chan = atoi(param_str[PARAM_NUM_1]);
			ret = ql_adc_stop(chan);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d  chan:%d\r\n", ret,chan);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
		}
	} 
    


/**
 * @brief 
 * 
 * @param chan 
 * @param data 
 * @param timeout 
 * @return ql_adc_errcode_e 
 */
void qlst_adc_read(int param_num, char (*param_str)[PARAM_SIZE])      
{
	if (param_num == PARAM_NUM_COUNT(PARAM_NUM_2) ){
		int ret;
		int chan ;
        uint16_t data = 0;
        uint32_t timeout;
		chan = atoi(param_str[PARAM_NUM_1]);
        timeout = atoi(param_str[PARAM_NUM_2]);
			ret = ql_adc_read(chan,&data,timeout);
			quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE, data);
			quecst_printf(SYNC_LOG, "re=%d  chan=%d data=%d timeout=%d\r\n", ret,chan, data,timeout);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
		}
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
void qlst_adc_read_raw(int param_num, char (*param_str)[PARAM_SIZE])     
{
	if (param_num == PARAM_NUM_COUNT(PARAM_NUM_2) ){
		 int ret;
		 int chan ;
         uint16_t read_buf = 0;
         uint32_t size, timeout;
		size=(sizeof(read_buf));
		 chan = atoi(param_str[PARAM_NUM_1]);
        // read_buf= param_str[PARAM_NUM_2];
        //size= atoi(param_str[PARAM_NUM_2]);
         timeout = atoi(param_str[PARAM_NUM_2]);
			ret = ql_adc_read_raw(chan,&read_buf,size,timeout);
			quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,size,read_buf);
			quecst_printf(SYNC_LOG, "re=%d  chan=%d  read_buf=%d  size=%d  timeout=%d\r\n", ret,chan,read_buf,size, timeout);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_ADC_INVALID_PARAM_ERR);
		 	quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_ADC_INVALID_PARAM_ERR, param_num);
		}
	} 
