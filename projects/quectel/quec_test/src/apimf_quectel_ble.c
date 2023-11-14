/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#include "quec_apimf_includes.h"
#include "quec_apimf.h"

//static ql_sem_t ble_cmd_sema = NULL;
uint8_t scan_actv_idx = -1;
uint8_t conn_actv_idx = -1;
uint8_t adv_actv_idx = -1;
ble_scan_param_t g_scan_info;
int ble_periphera_state = 0;
uint8_t g_conn_state = 0;
uint8_t g_conn_idx = 0;
uint8_t conn_addr[6] = {0xc8,0x47,0x8c,0xEE,0xDC,0x23};
uint8_t adv_addr_type = 1; //0:public ,1:random
uint8_t scan_success_flag = 0;

//extern char funcid_str[48];
typedef struct gapAdStructure_tag
{
    uint8_t         length;     /*!< Total length of the [adType + aData] fields. Equal to 1 + lengthOf(aData). */
    uint8_t         adType;     /*!< AD Type of this AD Structure. */
    uint8_t*        aData;      /*!< Data contained in this AD Structure; length of this array is equal to (gapAdStructure_t.length - 1). */
} gapAdStructure_t;
typedef struct 
{
    uint16_t char_hdl;
    uint16_t val_hdl;
    uint8_t prop;
}ql_char_t;
static ql_char_t g_char_info = {0, 0, 0};
static  uint16_t char_noti_enable = 0;

enum{
	QL_DEMO_IDX_SVC,
	QL_DEMO_IDX_FF03_VAL_CHAR,
	QL_DEMO_IDX_FF03_VAL_VALUE,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG,

	QL_DEMO_IDX_NB,    
};

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128    {0x00,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC_128     {0x03,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128    {0x02,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

#define NOTIFY_CHARACTERISTIC_128               {0x15,0xFF,0x00,0x00,0x34,0x56,0x00,0x00,0x00,0x00,0x28,0x37,0x00,0x00,0x00,0x00}
static const uint8_t ql_demo_svc_uuid[16] =     {0x36,0xF5,0x00,0x00,0x34,0x56,0x00,0x00,0x00,0x00,0x28,0x37,0x00,0x00,0x00,0x00};


ql_attm_desc_t ql_demo_att_db[QL_DEMO_IDX_NB] =
{
	//  Service Declaration
	[QL_DEMO_IDX_SVC]              = {QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128, QL_PERM_SET(RD, ENABLE), 0, 0},
    [QL_DEMO_IDX_FF03_VAL_CHAR]    = {QL_DEMO_ATT_DECL_CHARACTERISTIC_128,  QL_PERM_SET(RD, ENABLE), 0, 0},
	//  Level Characteristic Value
	[QL_DEMO_IDX_FF03_VAL_VALUE]   = {NOTIFY_CHARACTERISTIC_128,   QL_PERM_SET(WRITE_REQ, ENABLE) | QL_PERM_SET(NTF, ENABLE), QL_PERM_SET(RI, ENABLE)|QL_PERM_SET(UUID_LEN, UUID_16), 512},
	//  Level Characteristic - Client Characteristic Configuration Descriptor
	[QL_DEMO_IDX_FF03_VAL_NTF_CFG] = {QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128, QL_PERM_SET(RD, ENABLE)|QL_PERM_SET(WRITE_REQ, ENABLE), 0, 0},
};

static void ble_central_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param);
static void ql_demo_periphera_notice_cb(ble_notice_t notice, void *param);

void qlst_ble_init(int param_num, char (*param_str)[PARAM_SIZE])      
{
	int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
	ret = ql_ble_init();
    quecst_send_upper_data("%s=%d,%d\r\n",funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}

void qlst_ble_deinit(int param_num, char (*param_str)[PARAM_SIZE])      
{
	int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
	ret = ql_ble_deinit();
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
	quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
    
}

void qlst_ble_create_db(int param_num, char (*param_str)[PARAM_SIZE])      
{
    int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    /* set ble event callback */
    ql_ble_set_notice_cb(ql_demo_periphera_notice_cb);

    /* add ble services */
    ql_ble_db_cfg ble_db_cfg;
    ble_db_cfg.att_db = ql_demo_att_db;
    ble_db_cfg.att_db_nb = QL_DEMO_IDX_NB;
    ble_db_cfg.prf_task_id = 10;
    ble_db_cfg.start_hdl = 0;
    ble_db_cfg.svc_perm = QL_PERM_SET(SVC_UUID_LEN, UUID_128);
    memcpy(&(ble_db_cfg.uuid[0]), &ql_demo_svc_uuid[0], 16);
    ret=ql_ble_create_db(&ble_db_cfg);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
	quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
    
}

void qlst_ble_get_idle_actv_idx_handle(int param_num, char (*param_str)[PARAM_SIZE])
{      
    int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    ret=ql_ble_get_idle_actv_idx_handle();
    quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,QLST_SUCCESS,CURRENT_FUNC_TYPE,ret);
    quecst_printf(SYNC_LOG, "actv_idx:%d\r\n", ret);
    
}

/**
 * @brief     Get an idle connection activity
 *
 * @return
 *    - xx: the idle connection activity's index
 */
void qlst_ble_get_idle_conn_idx_handle(int param_num, char (*param_str)[PARAM_SIZE])      
{
    int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    ret=ql_ble_get_idle_conn_idx_handle();
    quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,QLST_SUCCESS,CURRENT_FUNC_TYPE,ret);	
    quecst_printf(SYNC_LOG, "conn_idx:%d\r\n", ret);
    
}

static void ble_at_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    switch (cmd)
    {
        case BLE_CREATE_ADV:
            quecst_printf(SYNC_LOG,"BLE_CREATE_ADV\r\n");
            //ble_periphera_state = 2;
            break;
        case BLE_SET_ADV_DATA:
            quecst_printf(SYNC_LOG,"BLE_SET_ADV_DATA\r\n");
            //ble_periphera_state = 3;
            break;
        case BLE_SET_RSP_DATA:
            break;
        case BLE_START_ADV:
            quecst_printf(SYNC_LOG,"BLE_START_ADV success !!!\r\n");
            //ble_periphera_state = 0;
            break;
        case BLE_STOP_ADV:
            break;
        case BLE_CREATE_SCAN:
            break;
        case BLE_START_SCAN:
            break;
        case BLE_STOP_SCAN:
            break;
        case BLE_INIT_CREATE:
            break;
        case BLE_INIT_START_CONN:
            break;
        case BLE_INIT_STOP_CONN:
            break;
        case BLE_CONN_DIS_CONN:
            break;
        case BLE_CONN_UPDATE_PARAM:
            break;
        case BLE_DELETE_ADV:
            break;
        case BLE_DELETE_SCAN:
            break;
        case BLE_CONN_READ_PHY:
            break;
        case BLE_CONN_SET_PHY:
            break;
        case BLE_CONN_UPDATE_MTU:
            break;
        default:
            break;
    }
    // if (ble_cmd_sema != NULL)
    //     ql_rtos_semaphore_release( ble_cmd_sema );
    quecst_send_upper_data("%s=%d,%d\r\n", "ble_at_cmd_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	quecst_printf(SYNC_LOG, "ble_cmd_t cmd:%d", cmd);
}

static void ql_demo_periphera_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
    case BLE_5_STACK_OK:
        quecst_printf(SYNC_LOG,"ble stack ok");
        break;
    case BLE_5_WRITE_EVENT:
    {
        ble_write_req_t *w_req = (ble_write_req_t *)param;
        quecst_printf(SYNC_LOG,"write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
                  w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);
#if 1
        // tranfer back
        ql_ble_gatts_send_ntf_value(w_req->conn_idx, w_req->len , w_req->value , QL_DEMO_IDX_FF03_VAL_VALUE);
#endif

        break;
    }
    case BLE_5_READ_EVENT:
    {
        ble_read_req_t *r_req = (ble_read_req_t *)param;
        quecst_printf(SYNC_LOG,"read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
                  r_req->conn_idx, r_req->prf_id, r_req->att_idx);
        r_req->value[0] = 0x12;
        r_req->value[1] = 0x34;
        r_req->length = 2;
        break;
    }
    case BLE_5_REPORT_ADV:
    {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
        quecst_printf(SYNC_LOG,"r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                  r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);
        break;
    }
    case BLE_5_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        quecst_printf(SYNC_LOG,"m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        quecst_printf(SYNC_LOG,"c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        g_conn_state = 1;
        g_conn_idx = c_ind->conn_idx;
        break;
    }
    case BLE_5_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        quecst_printf(SYNC_LOG,"d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        g_conn_state = 0;
       // QL_LOG_ON_ERR_VOID(ql_ble_start_advertising(adv_actv_idx, 0, ble_at_cmd_cb)); // restart advertising ,need waiting ble_cmd_sema in main thread 
        break;
    }
    case BLE_5_ATT_INFO_REQ:
    {
        ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
        quecst_printf(SYNC_LOG,"a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
        a_ind->length = 128;
        a_ind->status = QL_SUCCESS;
        break;
    }
    case BLE_5_CREATE_DB:
    {
        ble_create_db_t *cd_ind = (ble_create_db_t *)param;
        quecst_printf(SYNC_LOG,"cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
        ble_periphera_state = 1;
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        quecst_printf(SYNC_LOG,"BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        quecst_printf(SYNC_LOG,"BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        quecst_printf(SYNC_LOG,"BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
    default:
        break;
    }
    quecst_send_upper_data("%s=%d,%d\r\n", "ql_demo_periphera_notice_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	quecst_printf(SYNC_LOG, "ble_notice_t notice:%d", notice);
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
//0 7 120 160
void qlst_ble_create_advertising(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
		int ret;

		uint8_t actv_idx = atoi(param_str[PARAM_NUM_1]);
		uint8_t chnl_map = atoi(param_str[PARAM_NUM_2]);
		uint32_t intv_min = atoi(param_str[PARAM_NUM_3]);
		uint32_t intv_max = atoi(param_str[PARAM_NUM_4]);
		// QL_LOG_ON_ERR_VOID(ql_rtos_semaphore_create(&ble_cmd_sema, 1));
		/* set ble event callback */
        //ql_ble_set_notice_cb(ql_demo_periphera_notice_cb);
		 /* get idle actv_idx */
        //adv_actv_idx = ql_ble_get_idle_actv_idx_handle();
		adv_actv_idx=actv_idx; //maybe used in later
		ret = ql_ble_create_advertising(actv_idx,chnl_map,intv_min,intv_max,ble_at_cmd_cb);
        quecst_send_upper_data("%s=%d,%d\r\n",funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
        quecst_printf(SYNC_LOG, "ret=%d,id:%d,chnl:%d,min:%d,max:%d\r\n", ret,actv_idx,chnl_map,intv_min,intv_max);		
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief Start a ble advertising
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
//=0, 0
void qlst_ble_start_advertising(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;
		uint8_t actv_idx  = PARAM_INVALID;
        uint16_t duration  = PARAM_INVALID;

		actv_idx = atoi(param_str[PARAM_NUM_1]);
		duration = atoi(param_str[PARAM_NUM_2]);

		ret = ql_ble_start_advertising(actv_idx, duration,ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d,dutation:%d\r\n", ret,duration);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_stop_advertising(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		uint8_t actv_idx  = PARAM_INVALID;

		actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_stop_advertising(actv_idx,ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_delete_advertising(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		uint8_t actv_idx  = PARAM_INVALID;

		actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_delete_advertising(actv_idx,ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_set_adv_data(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
        char ble_name[32] = {0};
        unsigned char ble_mac[6] = {0};
		//uint8_t adv_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37, 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
        uint8_t actv_idx  = atoi(param_str[PARAM_NUM_1]);
        /* set ble bardcast data  */
        memset(ble_name, 0, sizeof(ble_name));
        bk_ble_get_mac(ble_mac);
        ble_name[0] = 0x02;
        ble_name[1] = 0x01;
        ble_name[2] = 0x06;
        ble_name[4] = 0x09;
        ret = snprintf(ble_name+5, sizeof(ble_name) - 6,
                    "FCM561D_%02x:%02x:%02x:%02x:%02x:%02x",
                    ble_mac[0], ble_mac[1], ble_mac[2], ble_mac[3], ble_mac[4], ble_mac[5]);
        ble_name[3] = ret+1;

		ret = ql_ble_set_adv_data(actv_idx,(unsigned char *)ble_name, 5 + ret,ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_set_scan_rsp_data(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		uint8_t actv_idx  = PARAM_INVALID;
		uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37, 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};

		actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_set_scan_rsp_data(actv_idx,scan_data, sizeof(scan_data),ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param adv 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
void qlst_ble_adv_start(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		uint8_t actv_idx  = PARAM_INVALID;
		ql_adv_param adv={
        .channel_map = 7,
		.interval_min =120,
		.interval_max =160,
		.advData={0x02, 0x01, 0x06, 0x0A, 0x09, 0x37, 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45},
		.advDataLen=14,
		.respData={0x02, 0x01, 0x06, 0x0A, 0x09, 0x37, 0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45},
		.respDataLen=14,
		.duration=0
        };

		actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_adv_start(actv_idx,(ql_adv_param*)&adv,ble_at_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

void app_demo_ble_adv_report_deal(const ble_recv_adv_t * p_adv_report)
{
    uint8_t index = 0;
    uint8_t loop_cnt = 0;

    while(index < p_adv_report->data_len)
    {
        gapAdStructure_t adElement;

        adElement.length = p_adv_report->data[index];
        adElement.adType = p_adv_report->data[index + 1];
        adElement.aData = (uint8_t *)&p_adv_report->data[index + 2];

        if(!memcmp(adElement.aData , "FCM561D" , strlen("FCM561D")))  //check adv name
        {
            int8_t adv_rssi = p_adv_report->rssi;
            quecst_printf(SYNC_LOG,"Found Device NAME:\"%s\", RSSI:%ddBm, type:%d, mac:", "FCM561D"  , adv_rssi, p_adv_report->adv_addr_type);
            for(uint8_t i = 0; i < 6; i++)
            {
                quecst_printf(SYNC_LOG,"%02X ", p_adv_report->adv_addr[i]);
                conn_addr[i] = p_adv_report->adv_addr[i];
            }
            quecst_printf(SYNC_LOG,"\r\n");

            adv_addr_type = p_adv_report->adv_addr_type;
            scan_success_flag = 1;
            //stop scan
            ql_ble_stop_scaning(scan_actv_idx , ble_central_cmd_cb);
            return;
        }

        /* Move on to the next AD elemnt type */
        index += adElement.length + sizeof(uint8_t);

        loop_cnt++;
        if(loop_cnt >= 250)
        {
            break;
        }
    }
}

static void ql_demo_central_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
    case BLE_5_STACK_OK:
        quecst_printf(SYNC_LOG,"ble stack ok");
        break;
    case BLE_5_WRITE_EVENT:
    {
        ble_write_req_t *w_req = (ble_write_req_t *)param;
        quecst_printf(SYNC_LOG,"write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
                  w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);

        break;
    }
    case BLE_5_READ_EVENT:
    {
        ble_read_req_t *r_req = (ble_read_req_t *)param;
        quecst_printf(SYNC_LOG,"read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
                  r_req->conn_idx, r_req->prf_id, r_req->att_idx);
        r_req->value[0] = 0x12;
        r_req->value[1] = 0x34;
        r_req->length = 2;
        break;
    }
    case BLE_5_REPORT_ADV:
    {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
        quecst_printf(SYNC_LOG,"r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                  r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);

        app_demo_ble_adv_report_deal(r_ind);
        break;
    }
    case BLE_5_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        quecst_printf(SYNC_LOG,"m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        quecst_printf(SYNC_LOG,"c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

        break;
    }
    case BLE_5_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        quecst_printf(SYNC_LOG,"d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        /* reset connected infomation*/
        memset(&g_char_info, 0, sizeof(g_char_info));
        char_noti_enable = 0;
        break;
    }
    case BLE_5_ATT_INFO_REQ:
    {
        ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
        quecst_printf(SYNC_LOG,"a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
        a_ind->length = 128;
        a_ind->status = QL_SUCCESS;
        break;
    }
    case BLE_5_CREATE_DB:
    {
        ble_create_db_t *cd_ind = (ble_create_db_t *)param;
        quecst_printf(SYNC_LOG,"cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        quecst_printf(SYNC_LOG,"BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        quecst_printf(SYNC_LOG,"BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        break;
    }
    case BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT:
    {
        quecst_printf(SYNC_LOG,"BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT:");
        break;
    }
    case BLE_5_DISCOVERY_CHAR_EVENT:
    {
        quecst_printf(SYNC_LOG,"BLE_5_DISCOVERY_CHAR_EVENT:");
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        quecst_printf(SYNC_LOG,"BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
    default:
        break;
    }
    quecst_send_upper_data("%s=%d,%d\r\n", "ql_demo_central_notice_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	quecst_printf(SYNC_LOG, "ql_demo_central_notice_cb notice:%d", notice);
}

static void ble_central_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    quecst_printf(SYNC_LOG,"cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
    switch (cmd)
    {
        case BLE_CREATE_ADV:
        case BLE_SET_ADV_DATA:
        case BLE_SET_RSP_DATA:
        case BLE_START_ADV:
        case BLE_STOP_ADV:
        case BLE_CREATE_SCAN:
            quecst_printf(SYNC_LOG,"BLE_CREATE_SCAN\r\n");
        case BLE_START_SCAN:
        case BLE_STOP_SCAN:
        case BLE_INIT_CREATE:
        case BLE_INIT_START_CONN:
        case BLE_INIT_STOP_CONN:
        case BLE_CONN_DIS_CONN:
        case BLE_CONN_UPDATE_PARAM:
        case BLE_DELETE_ADV:
        case BLE_DELETE_SCAN:
        case BLE_CONN_READ_PHY:
        case BLE_CONN_SET_PHY:
        case BLE_CONN_UPDATE_MTU:
            // if (ble_central_cmd_sema != NULL)
            //     ql_rtos_semaphore_release(ble_central_cmd_sema);
            break;
        default:
            break;
    }
    quecst_send_upper_data("%s=%d,%d\r\n", "ble_central_cmd_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
 	quecst_printf(SYNC_LOG, "ble_central_cmd_cb cmd:%d", cmd);
}

//0 48 32
void qlst_ble_create_scaning(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		int ret;
        ble_scan_param_t *scan_info = &g_scan_info;

		uint8_t actv_idx = atoi(param_str[PARAM_NUM_1]);
        scan_info->scan_intv = atoi(param_str[PARAM_NUM_2]);
        scan_info->scan_wd = atoi(param_str[PARAM_NUM_3]);
		/* set ble event callback */
		ql_ble_set_notice_cb(ql_demo_central_notice_cb);
		/* set scan param */
		scan_info->scan_phy = 0x07;
        scan_info->own_addr_type  = 0;
		// scan_info->scan_intv = 0x30;
		// scan_info->scan_wd = 0x20;
		/* get idle actv_idx */
		//scan_actv_idx = bk_ble_get_idle_conn_idx_handle();

		ret = ql_ble_create_scaning(actv_idx , scan_info->scan_intv, scan_info->scan_wd , ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_start_scaning(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		// scan_actv_idx = bk_ble_get_idle_conn_idx_handle();
        uint8_t actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_start_scaning(actv_idx , ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_stop_scaning(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		// scan_actv_idx = bk_ble_get_idle_conn_idx_handle();
         uint8_t actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_stop_scaning(actv_idx , ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
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
void qlst_ble_delete_scaning(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		// scan_actv_idx = bk_ble_get_idle_conn_idx_handle();
        uint8_t actv_idx = atoi(param_str[PARAM_NUM_1]);
		ret = ql_ble_delete_scaning(actv_idx , ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

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
//=0
void qlst_ble_update_param(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_5)){
		int ret;
		// scan_actv_idx = bk_ble_get_idle_conn_idx_handle();
        uint8_t conn_idx = atoi(param_str[PARAM_NUM_1]);
        uint16_t intv_min = atoi(param_str[PARAM_NUM_2]);
        uint16_t intv_max = atoi(param_str[PARAM_NUM_3]);
        uint16_t latency = atoi(param_str[PARAM_NUM_4]);
        uint16_t sup_to = atoi(param_str[PARAM_NUM_5]);
		ret = ql_ble_update_param(conn_idx ,intv_min,intv_max,latency,sup_to, NULL);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

//receive slave data
void quec_app_sdp_charac_cb(CHAR_TYPE type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data)
{
    quecst_printf(SYNC_LOG,"[APP]type:%x conidx:%d,handle:0x%02x(%d),len:%d,0x",type,conidx,hdl,hdl,len);
    for(int i = 0; i< len; i++)
    {
        quecst_printf(SYNC_LOG,"%02x ",data[i]);
    }
    quecst_send_upper_data("%s=%d,%d\r\n", "quec_app_sdp_charac_cb", QLST_SUCCESS, CALLBACK_FUNC_TYPE);
    quecst_printf(SYNC_LOG,"\r\n");
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
//=0,64, 0, 512
void qlst_ble_create_conn(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
		int ret;
		/*get currnt idle actv_idx*/
        uint8_t conn_actv_idx = atoi(param_str[PARAM_NUM_1]);
        uint16_t con_interval = atoi(param_str[PARAM_NUM_2]);
        uint16_t con_latency = atoi(param_str[PARAM_NUM_3]);
        uint16_t sup_to = atoi(param_str[PARAM_NUM_4]);
		
        //conn_actv_idx = ql_ble_get_idle_conn_idx_handle();
		quecst_printf(SYNC_LOG,"conn_actv_idx = %d\r\n" , conn_actv_idx);

		/*register callbak functions*/
		//ql_ble_gattc_register_event_recv_callback(quec_app_sdp_charac_cb);
		//ql_ble_register_app_sdp_common_callback(ble_sdp_comm_callback);
		/*create contention actv*/
		quecst_printf(SYNC_LOG,"creat conn\r\n");
		ret = ql_ble_create_conn(conn_actv_idx, con_interval, con_latency, sup_to, ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

//=0,0
void qlst_ble_set_connect_dev_addr(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;
		/*get currnt idle actv_idx*/
        uint8_t conn_actv_idx = atoi(param_str[PARAM_NUM_1]);
        uint8_t adv_addr_type = atoi(param_str[PARAM_NUM_2]);

		quecst_printf(SYNC_LOG,"creat conn\r\n");
		ret = ql_ble_set_connect_dev_addr(conn_actv_idx,(bd_addr_t *)conn_addr, adv_addr_type);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
void qlst_ble_start_conn(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		/*get currnt idle actv_idx*/
         uint8_t conn_actv_idx = atoi(param_str[PARAM_NUM_1]);
		//conn_actv_idx = ql_ble_get_idle_conn_idx_handle();
		//quecst_printf(SYNC_LOG,"conn_actv_idx = %d\r\n" , conn_actv_idx);
		// quecst_printf(SYNC_LOG,"set connect address\r\n");
		// QL_LOG_ON_ERR_VOID(ql_ble_set_connect_dev_addr(conn_actv_idx, (bd_addr_t *)conn_addr, adv_addr_type));
		quecst_printf(SYNC_LOG,"start connect ....\r\n");
		ret = ql_ble_start_conn(conn_actv_idx,ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param actv_idx 
 * @param callback 
 * @return ql_ble_errcode_e 
 */
void qlst_ble_stop_conn(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		/*get currnt idle actv_idx*/
        uint8_t conn_actv_idx = atoi(param_str[PARAM_NUM_1]);
		//conn_actv_idx = ql_ble_get_idle_conn_idx_handle();
		//quecst_printf(SYNC_LOG,"conn_actv_idx = %d\r\n" , conn_actv_idx);
		quecst_printf(SYNC_LOG,"stop connect ....\r\n");
		ret = ql_ble_stop_conn(conn_actv_idx, ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

void qlst_ble_gatts_disconnect(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
        uint8_t conn_idx = atoi(param_str[PARAM_NUM_1]);
	
		ret = ql_ble_gatts_disconnect(conn_idx, ble_central_cmd_cb);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

void qlst_ble_gatts_send_ntf_value(int param_num, char (*param_str)[PARAM_SIZE])
{
     uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37,0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;
        uint8_t conn_idx = atoi(param_str[PARAM_NUM_1]);
        uint8_t att_idx = atoi(param_str[PARAM_NUM_2]);
	
		ret = ql_ble_gatts_send_ntf_value(conn_idx,sizeof(scan_data), scan_data,att_idx);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

void qlst_ble_gatts_send_ind_value(int param_num, char (*param_str)[PARAM_SIZE])
{
     uint8_t scan_data[] = {0x02, 0x01, 0x06, 0x0A, 0x09, 0x37,0x32, 0x33, 0x31, 0x4e, 0x5f, 0x42, 0x4c, 0x45};
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;
        uint8_t conn_idx = atoi(param_str[PARAM_NUM_1]);
        uint8_t att_idx = atoi(param_str[PARAM_NUM_2]);
	
		ret = ql_ble_gatts_send_ind_value(conn_idx,sizeof(scan_data), scan_data,att_idx);
		quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
		quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
	}
	else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
















