/**
 * @file ql_ttimer_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_timer/ql_timer.h>
#include<quec_common/ql_defines.h>

typedef struct {
    ql_timer_callback callback;
    uint32_t time_cnt;
}timer_info_t;

static timer_info_t timer_arry[8] = {0};

/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_ms 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_init(ql_timer_number_e timer_id, uint32_t time_ms, ql_timer_callback timer_cb)
{
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    timer_arry[timer_id].time_cnt   = time_ms;
    timer_arry[timer_id].callback   = timer_cb;
    return bk_timer_enable(timer_id);
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @param time_us 
 * @param timer_cb 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_init_us(ql_timer_number_e timer_id, uint32_t time_us, ql_timer_callback timer_cb)
{
    
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    return QL_TIMER_NOT_SUPPORT_ERR;
    // //TODO:for bk7256 not support us delay timer
    // return QL_TIMER_EXECUTE_ERR;
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @param cont 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_get_cnt(ql_timer_number_e timer_id, uint32_t *cont)
{
    //TODO: for bk7256 not support get cnt value
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    return QL_TIMER_NOT_SUPPORT_ERR;
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_stop(ql_timer_number_e timer_id)
{
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    return bk_timer_stop(timer_id);
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_deinit(ql_timer_number_e timer_id)
{
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    return bk_timer_disable(timer_id);
}

/**
 * @brief 
 * 
 * @param timer_id 
 * @return ql_timer_errcode_e 
 */
ql_timer_errcode_e ql_timer_start(ql_timer_number_e timer_id)
{
    if(timer_id<0||timer_id>4){
        return QL_TIMER_INVALID_PARAM_ERR;
    }
    return bk_timer_start(timer_id, timer_arry[timer_id].time_cnt, (timer_isr_t)timer_arry[timer_id].callback);
}
