/**
 * @file ql_ble_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_ble/ql_ble.h>

#define QL_BLE_ADV_INTERVAL_MIN 	32      //UNIT:0.625MS
#define QL_BLE_ADV_INTERVAL_MAX 	0X4000  //UNIT:0.625MS
#define QL_BLE_SCAN_INTERVAL_MIN    18      //UNIT:0.625MS
#define QL_BLE_SCAN_INTERVAL_MAX    4096    //UNIT:0.625MS
#define QL_BLE_SCAN_WINDOW_MIN      17      //UNIT:0.625MS
#define QL_BLE_SCAN_WINDOW_MAX      4096    //UNIT:0.625MS
#define QL_BLE_CONNECT_INTERVAL_MIN 6       //UNIT:1.25MS
#define QL_BLE_CONNECT_INTERVAL_MAX 3200    //UNIT:1.25MS
#define QL_BLE_CONNECT_LATENCY_MAX  499   
#define QL_BLE_CONNECT_TIMEOUT_MIN  10      //UNIT:10MS
#define QL_BLE_CONNECT_TIMEOUT_MAX  3200    //UNIT:10MS

#define QL_BLE_UNKNOW_ACT_IDX       0xFFU

/**
 * @brief 
 * 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_init(void)
{
    return bk_ble_init();
}

/**
 * @brief 
 * 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_deinit(void)
{
    return bk_ble_deinit();
}

/**
 * @brief 
 * 
 * @param ble_db_cfg 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_db(ql_ble_db_cfg* ble_db_cfg)
{
    return bk_ble_create_db((struct bk_ble_db_cfg*)ble_db_cfg);
}

/**
 * @brief 
 * 
 * @param func 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_set_notice_cb(ble_notice_cb_t func)
{
    bk_ble_set_notice_cb(func);
    return QL_BLE_SUCCESS;
}

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t ql_ble_get_idle_actv_idx_handle(void)
{
    return bk_ble_get_idle_actv_idx_handle();
}

/**
 * @brief     Get an idle connection activity
 *
 * @return
 *    - xx: the idle connection activity's index
 */
uint8_t ql_ble_get_idle_conn_idx_handle(void)
{
    return bk_ble_get_idle_conn_idx_handle();
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param chnl_map 
 * @param intv_min 
 * @param intv_max 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_advertising(uint8_t actv_idx, 
                                        uint8_t chnl_map, 
                                        uint32_t intv_min, 
                                        uint32_t intv_max, 
                                        ble_cmd_cb_t callback)
{
    ble_adv_param_t adv_param;

    if(chnl_map > 7 || chnl_map < 1)
        return QL_BLE_ERR_BLE_PARAM;
    if(intv_min > QL_BLE_ADV_INTERVAL_MAX || intv_min < QL_BLE_ADV_INTERVAL_MIN)
        return QL_BLE_ERR_BLE_PARAM;
    if(intv_max > QL_BLE_ADV_INTERVAL_MAX || intv_max < QL_BLE_ADV_INTERVAL_MIN || intv_min > intv_max)
        return QL_BLE_ERR_BLE_PARAM;

    adv_param.adv_intv_max  = intv_max;
    adv_param.adv_intv_min  = intv_min;
    adv_param.chnl_map      = chnl_map;
    adv_param.adv_prop      = 3;
    adv_param.adv_type      = 0; //ADV_IND
    adv_param.prim_phy      = 1; //1M
    adv_param.second_phy    = 1; //1M
    adv_param.own_addr_type = 0; //BLE_STATIC_ADDR Own address type:  public=0 / random=1 / rpa_or_pub=2 / rpa_or_rnd=3

    return bk_ble_create_advertising(actv_idx, &adv_param, callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param duration 
 * @param callback 
 * 
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 * 
 * @return ql_ble_errcode_e 
 *      
 */
ql_ble_errcode_e ql_ble_start_advertising(uint8_t actv_idx, uint16_t duration, ble_cmd_cb_t callback)
{
    return bk_ble_start_advertising(actv_idx, duration, callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * 
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_start_advertising
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_stop_advertising(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_stop_advertising(actv_idx, callback);
}

/**
 * @brief     Delete the advertising that has been created
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_DELETE_ADV
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 * @return
 * 
 */
ql_ble_errcode_e ql_ble_delete_advertising(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_delete_advertising(actv_idx, callback);
}

/**
 * @brief     Set the advertising data
 *
 * @param
 *    - actv_idx: the index of activity
 *    - adv_buff: advertising data
 *    - adv_len: the length of advertising data
 *    - callback: register a callback for this action, ble_cmd_t: BLE_SET_ADV_DATA
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_advertising
 *
 *
 * User example:
 * @code
 *     const uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
 *     bk_ble_set_adv_data(actv_idx, adv_data, sizeof(adv_data), ble_at_cmd_cb);
 * @endcode
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_set_adv_data(uint8_t actv_idx, uint8_t* adv_buff, uint8_t adv_len, ble_cmd_cb_t callback)
{
    return bk_ble_set_adv_data(actv_idx,adv_buff, adv_len, callback);
}

/**
 * @brief     Set the scan response data
 *
 * @param
 *    - actv_idx: the index of activity
 *    - scan_buff: scan response data
 *    - scan_len: the length of scan response data
 *    - callback: register a callback for this action, ble_cmd_t: BLE_SET_RSP_DATA
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.scan rsp data similaly to adv data
 * @attention 3.must used after bk_ble_create_advertising
 *
 *
 * User example:
 * @code
 *     const uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
 *     bk_ble_set_scan_rsp_data(actv_idx, scan_data, sizeof(scan_data), ble_at_cmd_cb);
 * @endcode
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_set_scan_rsp_data(uint8_t actv_idx, uint8_t* scan_buff, uint8_t scan_len, ble_cmd_cb_t callback)
{
    return bk_ble_set_scan_rsp_data(actv_idx, scan_buff, scan_len,callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param adv 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_adv_start(uint8_t actv_idx, ql_adv_param *adv, ble_cmd_cb_t callback)
{
    ble_adv_param_t adv_param;

    if(adv->channel_map > 7 || adv->channel_map < 1)
        return QL_BLE_ERR_BLE_PARAM;
    if(adv->interval_min > QL_BLE_ADV_INTERVAL_MAX || adv->interval_min < QL_BLE_ADV_INTERVAL_MIN)
        return QL_BLE_ERR_BLE_PARAM;
    if(adv->interval_max > QL_BLE_ADV_INTERVAL_MAX || adv->interval_max < QL_BLE_ADV_INTERVAL_MIN || adv->interval_min > adv->interval_max)
        return QL_BLE_ERR_BLE_PARAM;

    adv_param.own_addr_type = 0;//BLE_STATIC_ADDR
    adv_param.adv_type      = 0; //ADV_IND
    adv_param.chnl_map      = adv->channel_map;
    adv_param.adv_prop      = 3;
    adv_param.adv_intv_min  = adv->interval_min; //min
    adv_param.adv_intv_max  = adv->interval_max; //max
    adv_param.prim_phy      = 1;// 1M
    adv_param.second_phy    = 1;// 1M

    actv_idx = bk_ble_get_idle_actv_idx_handle();
    if (actv_idx != QL_BLE_UNKNOW_ACT_IDX) {
            bk_ble_create_advertising(actv_idx, &adv_param, callback);
    }

    //wait for ble_at_cmd_cb
    bk_ble_set_adv_data(actv_idx, adv->advData, adv->advDataLen, callback);

    //wait for ble_at_cmd_cb
    bk_ble_set_scan_rsp_data(actv_idx, adv->respData, adv->respDataLen, callback);

    //wait for ble_at_cmd_cb
    bk_ble_start_advertising(actv_idx, adv->duration, callback);  

    return QL_BLE_SUCCESS;  
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param scan_param 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_scaning(uint8_t actv_idx, uint16_t scan_inty, uint16_t scan_wd, ble_cmd_cb_t callback)
{
    ble_scan_param_t scan_param;

    if(scan_inty > QL_BLE_SCAN_INTERVAL_MAX || scan_inty < QL_BLE_SCAN_INTERVAL_MIN)
        return QL_BLE_ERR_BLE_PARAM;
    if(scan_wd > QL_BLE_SCAN_WINDOW_MAX || scan_wd < QL_BLE_SCAN_WINDOW_MIN )
        return QL_BLE_ERR_BLE_PARAM;

    scan_param.own_addr_type    = 0;
    scan_param.scan_intv        = scan_inty;
    scan_param.scan_wd          = scan_wd;
    scan_param.scan_phy         = 0x07;
    return bk_ble_create_scaning(actv_idx, &scan_param, callback);
}

/**
 * @brief     Start a ble scan
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_START_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_scaning
 * @attention 3.adv will report in ble_notice_cb_t as BLE_5_REPORT_ADV
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_start_scaning(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_start_scaning(actv_idx, callback);
}

/**
 * @brief     Stop the scan that has been started
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_STOP_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_start_scaning
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_stop_scaning(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_stop_scaning(actv_idx, callback);
}

/**
 * @brief     Delete the scan that has been created
 *
 * @param
 *    - actv_idx: the index of activity
 *    - callback: register a callback for this action, ble_cmd_t: BLE_DELETE_SCAN
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_scaning
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_delete_scaning(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_delete_scaning(actv_idx, callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param scan 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_scan_start(uint8_t actv_idx, struct scan_param *scan, ble_cmd_cb_t callback);

/**
 * @brief 
 * 
 * @param conn_idx 
 * @param intv_min 
 * @param intv_max 
 * @param latency 
 * @param sup_to 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_update_param(uint8_t conn_idx, 
                                    uint16_t intv_min, 
                                    uint16_t intv_max,
                                    uint16_t latency,
                                    uint16_t sup_to,
                                    ble_cmd_cb_t callback)
{
    ble_conn_param_t conn_param;

    if(intv_min > QL_BLE_CONNECT_INTERVAL_MAX || intv_min < QL_BLE_CONNECT_INTERVAL_MIN)
        return QL_BLE_ERR_BLE_PARAM;
    if(intv_max > QL_BLE_CONNECT_INTERVAL_MAX || intv_max < QL_BLE_CONNECT_INTERVAL_MIN ||intv_min > intv_max)
        return QL_BLE_ERR_BLE_PARAM;
    if(latency > QL_BLE_CONNECT_LATENCY_MAX)
        return QL_BLE_ERR_BLE_PARAM;
    if(sup_to > QL_BLE_CONNECT_TIMEOUT_MAX || sup_to < QL_BLE_CONNECT_TIMEOUT_MIN )
        return QL_BLE_ERR_BLE_PARAM;

    conn_param.intv_min         = intv_min;
    conn_param.intv_max         = intv_max;
    conn_param.con_latency      = latency;
    conn_param.sup_to           = sup_to;
    conn_param.init_phys        = INIT_PHY_TYPE_LE_1M;
    return bk_ble_update_param(conn_idx, &conn_param, callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param con_interval 
 * @param con_latency 
 * @param sup_to 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_conn(uint8_t         actv_idx,
                                    uint16_t        con_interval,
                                    uint16_t        con_latency,
                                    uint16_t        sup_to,
                                    ble_cmd_cb_t    callback)
{
    ble_conn_param_t conn_param;

    if(con_interval > QL_BLE_CONNECT_INTERVAL_MAX || con_interval < QL_BLE_CONNECT_INTERVAL_MIN)
        return QL_BLE_ERR_BLE_PARAM;
    if(con_latency > QL_BLE_CONNECT_LATENCY_MAX)
        return QL_BLE_ERR_BLE_PARAM;
    if(sup_to > QL_BLE_CONNECT_TIMEOUT_MAX || sup_to < QL_BLE_CONNECT_TIMEOUT_MIN )
        return QL_BLE_ERR_BLE_PARAM;

    conn_param.intv_min             = con_interval;
    conn_param.intv_max             = con_interval;
    conn_param.con_latency          = con_latency;
    conn_param.sup_to               = sup_to;
    conn_param.init_phys            = INIT_PHY_TYPE_LE_1M;

    return bk_ble_create_init(actv_idx, &conn_param, callback);
}


/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_start_conn(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_init_start_conn(actv_idx, callback);
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_stop_conn(uint8_t actv_idx, ble_cmd_cb_t callback)
{
    return bk_ble_init_stop_conn(actv_idx, callback);
}

/**
 * @brief     Set the address of the device to be connected
 *
 * @param
 *    - connidx: the index of connection
 *    - bdaddr: the address of the device to be connected
 *    - addr_type: the address type of the device to be connected, 1: public 0: random
 *
 *
 * @attention 1.you must wait callback status, 0 mean success.
 * @attention 2.must used after bk_ble_create_init
 * @attention 3.addr_type must right, if wrong, cant connect
 *
 * @return
 *    - BK_ERR_BLE_SUCCESS: succeed
 *    - others: other errors.
 */
ql_ble_errcode_e ql_ble_set_connect_dev_addr(uint8_t actv_idx, bd_addr_t *bdaddr, uint8_t addr_type)
{
    return bk_ble_init_set_connect_dev_addr(actv_idx, bdaddr, addr_type);
}


/**
 * @brief 
 * 
 * @param conn_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_disconnect(uint8_t conn_idx, ble_cmd_cb_t callback)
{
    return bk_ble_disconnect(conn_idx, callback);
}

/**
 * @brief 
 * 
 * @param cb 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gattc_register_event_recv_callback(app_sdp_charac_callback cb)
{
    bk_ble_register_app_sdp_charac_callback(cb);
    return QL_BLE_SUCCESS;
}

/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ntf_value(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx)
{
    uint16_t prf_id = 10;

    return bk_ble_send_noti_value(con_idx, len, buf, prf_id, att_idx);
}

/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ind_value(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx)
{
    uint16_t prf_id = 0;
    return bk_ble_send_ind_value(con_idx, len, buf, prf_id, att_idx);
}

