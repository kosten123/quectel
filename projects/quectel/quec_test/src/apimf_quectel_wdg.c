/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"

/**
 * @brief 
 * 
 * @param timeout 
 * @return ql_wdg_errcode_e 
 */

void qlst_wdg_init (int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret ;
        uint32_t timeout;
		timeout = atoi(param_str[PARAM_NUM_1]);
       

	   	ret = ql_wdg_init(timeout);

		quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
		quecst_printf(SYNC_LOG, ":re=%d timeout=%d\r\n", ret, timeout );
	}
	 else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR, param_num);
	}
}

/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
void qlst_wdg_reload(int param_num, char (*param_str)[PARAM_SIZE])
{
	int ret;
    if (param_num==0){
       // quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
	ret = ql_wdg_reload();
	quecst_send_upper_data(funcid_str);	
	quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
  }  else{
		quecst_send_upper_data(funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR, param_num);
}
}
/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
void qlst_wdg_finalize(int param_num, char (*param_str)[PARAM_SIZE])
{
	int ret;
    if (param_num==0){
       // quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);

	ret = ql_wdg_finalize();
	quecst_send_upper_data(funcid_str);	
	quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
 }   else{
		quecst_send_upper_data(funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR, param_num);
	}
}