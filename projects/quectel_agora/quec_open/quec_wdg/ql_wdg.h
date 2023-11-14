/**
 * @file ql_wdg_pub.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_WDG_PUB_H
#define _QL_WDG_PUB_H

#include<quec_common/ql_platform.h>

typedef enum
{
    QL_WDG_SUCCESS      = 0,
    QL_WDG_EXECUTE_ERR,
    QL_WDG_INVALID_PARAM_ERR,
}ql_wdg_errcode_e;

/**
 * @brief 
 * 
 * @param timeout 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_init(uint32_t timeout);

/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_reload(void);


/**
 * @brief 
 * 
 * @return ql_wdg_errcode_e 
 */
ql_wdg_errcode_e ql_wdg_finalize(void);

#endif
