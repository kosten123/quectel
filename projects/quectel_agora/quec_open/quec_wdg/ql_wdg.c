/**
 * @file ql_wdg_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_wdg/ql_wdg.h>

/**
 * @brief 
 * 
 * @param timeout 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_init(uint32_t timeout)
{
    if(timeout<46){
        return QL_WDG_INVALID_PARAM_ERR;
    }
    bk_wdt_driver_init();
    return bk_wdt_start(timeout);
}

/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_reload(void)
{
    return bk_wdt_feed();
}


/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_finalize(void)
{
    return bk_wdt_stop();
}
