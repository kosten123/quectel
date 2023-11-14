/**
 * @file ql_ble_pub.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_BLE_PUB_H
#define _QL_BLE_PUB_H

#include<quec_common/ql_defines.h>

#define QL_MAX_ADV_DATA_LEN        31

#define QL_PERM_SET(access, right) \
    (((BK_BLE_PERM_RIGHT_ ## right) << (BK_BLE_ ## access ## _POS)) & (BK_BLE_ ## access ## _MASK))

#define QL_PERM_GET(perm, access)\
    (((perm) & (BK_BLE_ ## access ## _MASK)) >> (BK_BLE_ ## access ## _POS))

#ifdef __cplusplus
extern "C"{
#endif

typedef enum
{
	QL_BLE_SUCCESS = 0,
	QL_BLE_ERR_PROFILE,
	QL_BLE_ERR_CREATE_DB,
	QL_BLE_ERR_CMD_NOT_SUPPORT,
	QL_BLE_ERR_UNKNOW_IDX,
	QL_BLE_ERR_BLE_STATUS,
	QL_BLE_ERR_BLE_PARAM,
	QL_BLE_ERR_ADV_DATA,
	QL_BLE_ERR_CMD_RUN,
	QL_BLE_ERR_NO_MEM,
	QL_BLE_ERR_INIT_CREATE,
	QL_BLE_ERR_INIT_STATE,
	QL_BLE_ERR_ATTC_WRITE,
	QL_BLE_ERR_ATTC_WRITE_UNREGISTER,
}ql_ble_errcode_e;

typedef enum
{
    QL_ACTV_IDX,
    QL_CONN_IDX,
}ql_idx_type_e;

typedef struct 
{
    uint8_t     uuid[16];
    uint16_t    perm;
    uint16_t    ext_perm;
    uint16_t    max_size;
}ql_attm_desc_t;

typedef struct 
{
    uint16_t        prf_task_id;
    uint8_t         uuid[16];
    uint8_t         att_db_nb;
    uint16_t        start_hdl;
    ql_attm_desc_t* att_db;
    uint8_t         svc_perm;
}ql_ble_db_cfg;

typedef struct adv_param{
    uint8_t  advData[QL_MAX_ADV_DATA_LEN];
    uint8_t  advDataLen;
    uint8_t  respData[QL_MAX_ADV_DATA_LEN];
    uint8_t  respDataLen;
    uint8_t  channel_map;
    uint16_t interval_min;
    uint16_t interval_max;
    uint16_t duration;
}ql_adv_param;

typedef struct scan_param{
    uint8_t     filter_en;
    uint8_t     channel_map;
    uint16_t    interval;
    uint16_t    window;
}ql_scan_param;


/**
 * @brief 
 * 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_init(void);

/**
 * @brief 
 * 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_deinit(void);

/**
 * @brief 
 * 
 * @param ble_db_cfg 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_db(ql_ble_db_cfg* ble_db_cfg);

/**
 * @brief 
 * 
 * @param func 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_set_notice_cb(ble_notice_cb_t func);

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t ql_ble_get_idle_actv_idx_handle(void);

/**
 * @brief     Get an idle connection activity
 *
 * @return
 *    - xx: the idle connection activity's index
 */
uint8_t ql_ble_get_idle_conn_idx_handle(void);

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
                                        ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_start_advertising(uint8_t actv_idx, uint16_t duration, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_stop_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_delete_advertising(uint8_t actv_idx, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_set_adv_data(uint8_t actv_idx, uint8_t* adv_buff, uint8_t adv_len, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_set_scan_rsp_data(uint8_t actv_idx, uint8_t* scan_buff, uint8_t scan_len, ble_cmd_cb_t callback);

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param adv 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_adv_start(uint8_t actv_idx, ql_adv_param *adv, ble_cmd_cb_t callback);

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param scan_param 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_create_scaning(uint8_t actv_idx, uint16_t scan_inty, uint16_t scan_wd, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_start_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_stop_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_delete_scaning(uint8_t actv_idx, ble_cmd_cb_t callback);

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
                                    ble_cmd_cb_t callback);

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
                                    ble_cmd_cb_t    callback);


/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_start_conn(uint8_t actv_idx, ble_cmd_cb_t callback);

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_stop_conn(uint8_t actv_idx, ble_cmd_cb_t callback);

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
ql_ble_errcode_e ql_ble_set_connect_dev_addr(uint8_t actv_idx, bd_addr_t *bdaddr, uint8_t addr_type);

/**
 * @brief 
 * 
 * @param cb 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gattc_register_event_recv_callback(app_sdp_charac_callback cb);

/*
ql_ble_errcode_e ql_ble_gattc_write_service_data_req(uint8_t con_idx, 
                                                    uint16_t handle,
                                                    uint16_t data_len,
                                                    uint8_t* data,
                                                    ble_cmd_cb_t callback);

ql_ble_errcode_e ql_ble_gattc_read_service_data_by_handle_req(uint8_t con_idx, 
                                                            uint16_t handle, 
                                                            ble_cmd_cb_t callback);
*/


/**
 * @brief 
 * 
 * @param conn_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_disconnect(uint8_t conn_idx, ble_cmd_cb_t callback);

/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ntf_value(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx);


/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @param prf_id
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ntf_value_ex(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx, uint16_t prf_id);


/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ind_value(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx);

/**
 * @brief 
 * 
 * @param con_idx 
 * @param len 
 * @param buf 
 * @param att_idx 
 * @param prf_id
 * @return ql_ble_errcode_e 
 */
ql_ble_errcode_e ql_ble_gatts_send_ind_value_ex(uint8_t con_idx, uint32_t len, uint8_t *buf, uint16_t att_idx,uint16_t prf_id);

#ifdef __cplusplus
}
#endif
#endif

