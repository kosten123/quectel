/**
 * @file ql_timer_pub.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef _QL_TIMER_PUB_H
#define _QL_TIMER_PUB_H

#include<quec_common/ql_platform.h>

typedef enum
{
	QL_TIMER_SUCCESS = 0,
	QL_TIMER_EXECUTE_ERR,
	QL_TIMER_INVALID_PARAM_ERR,
	QL_TIMER_NOT_OPEN_ERR,
	QL_TIMER_NOT_SUPPORT_ERR
}ql_timer_errcode_e;

typedef enum{
	QL_TIMER_0 =0,
	QL_TIMER_1,
	QL_TIMER_2,
	QL_TIMER_3,
	QL_TIMER_4,
	QL_TIMER_5,
}ql_timer_number_e;

typedef void (*ql_timer_callback)(uint8_t arg);

/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_ms 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_init(ql_timer_number_e timer_id, uint32_t time_ms, ql_timer_callback timer_cb);

/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_us 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_init_us(ql_timer_number_e timer_id, uint32_t time_us, ql_timer_callback timer_cb);

/**
 * @brief 
 * 
 * @param timer_id 
 * @param cont 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_get_cnt(ql_timer_number_e timer_id, uint32_t *cont);

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_stop(ql_timer_number_e timer_id);

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_deinit(ql_timer_number_e timer_id);

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_start(ql_timer_number_e timer_id);

#endif
