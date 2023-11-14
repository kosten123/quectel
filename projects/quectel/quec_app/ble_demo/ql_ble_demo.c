/**
 * @file ql_ble_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include<quec_ble/ql_ble.h>

#include "ql_demo_cfg.h"


#if CFG_ENABLE_QUECTEL_BLE_PERIPHERA
static ql_task_t ble_periphera_hdl = NULL;
static ql_sem_t ble_cmd_sema = NULL;
int ble_periphera_state = 0;
uint8_t adv_actv_idx = -1;
uint8_t g_conn_state = 0;
uint8_t g_conn_idx = 0;
char ble_name[32] = {0};
ble_adv_param_t g_adv_info = {0};

#define QL_DEMO_ATT_DECL_PRIMARY_SERVICE_128    {0x00,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define QL_DEMO_ATT_DECL_CHARACTERISTIC_128     {0x03,0x28,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define QL_DEMO_ATT_DESC_CLIENT_CHAR_CFG_128    {0x02,0x29,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

#define NOTIFY_CHARACTERISTIC_128               {0x15,0xFF,0x00,0x00,0x34,0x56,0x00,0x00,0x00,0x00,0x28,0x37,0x00,0x00,0x00,0x00}
static const uint8_t ql_demo_svc_uuid[16] =     {0x36,0xF5,0x00,0x00,0x34,0x56,0x00,0x00,0x00,0x00,0x28,0x37,0x00,0x00,0x00,0x00};

enum{
	QL_DEMO_IDX_SVC,
	QL_DEMO_IDX_FF03_VAL_CHAR,
	QL_DEMO_IDX_FF03_VAL_VALUE,
	QL_DEMO_IDX_FF03_VAL_NTF_CFG,

	QL_DEMO_IDX_NB,    
};

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

static void ble_at_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    switch (cmd)
    {
        case BLE_CREATE_ADV:
            os_printf("BLE_CREATE_ADV\r\n");
            //ble_periphera_state = 2;
            break;
        case BLE_SET_ADV_DATA:
            os_printf("BLE_SET_ADV_DATA\r\n");
            //ble_periphera_state = 3;
            break;
        case BLE_SET_RSP_DATA:
            break;
        case BLE_START_ADV:
            os_printf("BLE_START_ADV success !!!\r\n");
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
    if (ble_cmd_sema != NULL)
        ql_rtos_semaphore_release( ble_cmd_sema );
}


static void ql_demo_periphera_notice_cb(ble_notice_t notice, void *param)
{
    switch (notice)
    {
    case BLE_5_STACK_OK:
        os_printf("ble stack ok");
        break;
    case BLE_5_WRITE_EVENT:
    {
        ble_write_req_t *w_req = (ble_write_req_t *)param;
        os_printf("write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
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
        os_printf("read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
                  r_req->conn_idx, r_req->prf_id, r_req->att_idx);
        r_req->value[0] = 0x12;
        r_req->value[1] = 0x34;
        r_req->length = 2;
        break;
    }
    case BLE_5_REPORT_ADV:
    {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
        os_printf("r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                  r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);
        break;
    }
    case BLE_5_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        os_printf("m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        g_conn_state = 1;
        g_conn_idx = c_ind->conn_idx;
        break;
    }
    case BLE_5_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        g_conn_state = 0;
        QL_LOG_ON_ERR_VOID(ql_ble_start_advertising(adv_actv_idx, 0, ble_at_cmd_cb)); // restart advertising ,need waiting ble_cmd_sema in main thread 
        break;
    }
    case BLE_5_ATT_INFO_REQ:
    {
        ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
        os_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
        a_ind->length = 128;
        a_ind->status = QL_SUCCESS;
        break;
    }
    case BLE_5_CREATE_DB:
    {
        ble_create_db_t *cd_ind = (ble_create_db_t *)param;
        os_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
        ble_periphera_state = 1;
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
    default:
        break;
    }
}

static void ql_ble_demo_get_mac_address(uint8_t *ble_mac)
{
    if(ble_mac == NULL)
    {
        return;
    }
    bk_ble_get_mac(ble_mac);
}

static void ql_ble_peripheral_demo_entry(void *arg)
{
    unsigned char ble_mac[6] = {0};
    ble_adv_param_t *adv_info = &g_adv_info;
    int ret = 0;

    os_printf("ql_ble_peripheral_demo_entry \r\n");

    QL_LOG_ON_ERR_VOID(ql_rtos_semaphore_create(&ble_cmd_sema, 1));
    while (1)
    {
        if(ble_periphera_state == 0)
        {
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
            QL_LOG_ON_ERR_VOID(ql_ble_create_db(&ble_db_cfg));

            /* set bardcast params */
            adv_info->adv_intv_min = 120;
            adv_info->adv_intv_max = 160;
            adv_info->chnl_map = 7;
            adv_info->own_addr_type = 0;
            adv_info->adv_type = 0;
            adv_info->adv_prop = 3;
            adv_info->prim_phy = 1;
            adv_info->second_phy = 1;

            /* set ble bardcast data  */
            memset(ble_name, 0, sizeof(ble_name));
            ql_ble_demo_get_mac_address(ble_mac);
            ble_name[0] = 0x02;
            ble_name[1] = 0x01;
            ble_name[2] = 0x06;
            ble_name[4] = 0x09;
            ret = snprintf(ble_name+5, sizeof(ble_name) - 6,
                        "FCM561D_%02x:%02x:%02x:%02x:%02x:%02x",
                        ble_mac[0], ble_mac[1], ble_mac[2], ble_mac[3], ble_mac[4], ble_mac[5]);
            ble_name[3] = ret+1;

            /* get idle actv_idx */
            adv_actv_idx = ql_ble_get_idle_actv_idx_handle();
            QL_LOG_ON_ERR_VOID(ql_ble_create_advertising(adv_actv_idx, adv_info->chnl_map,adv_info->adv_intv_min,adv_info->adv_intv_max, ble_at_cmd_cb));
            ql_rtos_semaphore_wait(ble_cmd_sema, BEKEN_WAIT_FOREVER);
            QL_LOG_ON_ERR_VOID(ql_ble_set_adv_data(adv_actv_idx, (unsigned char *)ble_name, 5 + ret, ble_at_cmd_cb));
            ql_rtos_semaphore_wait(ble_cmd_sema, BEKEN_WAIT_FOREVER);
            QL_LOG_ON_ERR_VOID(ql_ble_start_advertising(adv_actv_idx, 0, ble_at_cmd_cb));
            ql_rtos_semaphore_wait(ble_cmd_sema, BEKEN_WAIT_FOREVER);
            ble_periphera_state = 1;
        }

        if (1 == g_conn_state)
        {
            g_conn_state = 2;
            //ql_ble_gatt_mtu_changes(g_conn_idx, 100, ble_at_cmd_cb);
            ql_rtos_semaphore_wait(ble_cmd_sema, BEKEN_WAIT_FOREVER);
        }
        ql_rtos_task_sleep_ms(20);
    }

    ql_rtos_task_delete(NULL);
}

void ql_ble_demo_periphera_thread_create(void)
{
    QlOSStatus err = kNoErr;
    // todo  create new thread;
    err = ql_rtos_task_create(  &ble_periphera_hdl,
                                8*1024,
                                BEKEN_APPLICATION_PRIORITY,
                                "ble_demo",
                                ql_ble_peripheral_demo_entry,
                                0);

    if (err != kNoErr)
    {
        os_printf("Error: Failed to create ble test thread: %d\r\n", err);
        goto init_err ;
    }

    return;

init_err:
    if(ble_periphera_hdl != NULL){
        ql_rtos_task_delete(ble_periphera_hdl);
    }
}

#endif

#if CFG_ENABLE_QUECTEL_BLE_CENTRAL
/*! Definition of an AD Structure as contained in Advertising and Scan Response packets. An Advertising or Scan Response packet contains several AD Structures. */
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

int ble_central_state = 0;
static ql_task_t ble_central_hdl = NULL;
uint8_t scan_actv_idx = -1;
uint8_t conn_actv_idx = -1;
static ql_char_t g_char_info = {0, 0, 0};

ble_scan_param_t g_scan_info;

uint8_t conn_addr[6] = {0xc8,0x47,0x8c,0xEE,0xDC,0x23};
static  uint8_t char_uuid[16] = {0x15,0xFF,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static  uint16_t char_noti_enable = 0;
uint8_t adv_addr_type = 1; //0:public ,1:random
uint8_t scan_success_flag = 0;
uint8_t data_handle = 0;

static ql_sem_t ble_central_cmd_sema = NULL;

static void ble_central_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param);



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
            os_printf("Found Device NAME:\"%s\", RSSI:%ddBm, type:%d, mac:", "FCM561D"  , adv_rssi, p_adv_report->adv_addr_type);
            for(uint8_t i = 0; i < 6; i++)
            {
                os_printf("%02X ", p_adv_report->adv_addr[i]);
                conn_addr[i] = p_adv_report->adv_addr[i];
            }
            os_printf("\r\n");

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
        os_printf("ble stack ok");
        break;
    case BLE_5_WRITE_EVENT:
    {
        ble_write_req_t *w_req = (ble_write_req_t *)param;
        os_printf("write_cb:conn_idx:%d, prf_id:%d, add_id:%d, len:%d, data[0]:%02x\r\n",
                  w_req->conn_idx, w_req->prf_id, w_req->att_idx, w_req->len, w_req->value[0]);

        break;
    }
    case BLE_5_READ_EVENT:
    {
        ble_read_req_t *r_req = (ble_read_req_t *)param;
        os_printf("read_cb:conn_idx:%d, prf_id:%d, add_id:%d\r\n",
                  r_req->conn_idx, r_req->prf_id, r_req->att_idx);
        r_req->value[0] = 0x12;
        r_req->value[1] = 0x34;
        r_req->length = 2;
        break;
    }
    case BLE_5_REPORT_ADV:
    {
        ble_recv_adv_t *r_ind = (ble_recv_adv_t *)param;
        os_printf("r_ind:actv_idx:%d, adv_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  r_ind->actv_idx, r_ind->adv_addr[0], r_ind->adv_addr[1], r_ind->adv_addr[2],
                  r_ind->adv_addr[3], r_ind->adv_addr[4], r_ind->adv_addr[5]);

        app_demo_ble_adv_report_deal(r_ind);
        break;
    }
    case BLE_5_MTU_CHANGE:
    {
        ble_mtu_change_t *m_ind = (ble_mtu_change_t *)param;
        os_printf("m_ind:conn_idx:%d, mtu_size:%d\r\n", m_ind->conn_idx, m_ind->mtu_size);
        break;
    }
    case BLE_5_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("c_ind:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);

        break;
    }
    case BLE_5_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("d_ind:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        /* reset connected infomation*/
        os_memset(&g_char_info, 0, sizeof(g_char_info));
        char_noti_enable = 0;
        break;
    }
    case BLE_5_ATT_INFO_REQ:
    {
        ble_att_info_req_t *a_ind = (ble_att_info_req_t *)param;
        os_printf("a_ind:conn_idx:%d\r\n", a_ind->conn_idx);
        a_ind->length = 128;
        a_ind->status = QL_SUCCESS;
        break;
    }
    case BLE_5_CREATE_DB:
    {
        ble_create_db_t *cd_ind = (ble_create_db_t *)param;
        os_printf("cd_ind:prf_id:%d, status:%d\r\n", cd_ind->prf_id, cd_ind->status);
        break;
    }
    case BLE_5_INIT_CONNECT_EVENT:
    {
        ble_conn_ind_t *c_ind = (ble_conn_ind_t *)param;
        os_printf("BLE_5_INIT_CONNECT_EVENT:conn_idx:%d, addr_type:%d, peer_addr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
                  c_ind->conn_idx, c_ind->peer_addr_type, c_ind->peer_addr[0], c_ind->peer_addr[1],
                  c_ind->peer_addr[2], c_ind->peer_addr[3], c_ind->peer_addr[4], c_ind->peer_addr[5]);
        break;
    }
    case BLE_5_INIT_DISCONNECT_EVENT:
    {
        ble_discon_ind_t *d_ind = (ble_discon_ind_t *)param;
        os_printf("BLE_5_INIT_DISCONNECT_EVENT:conn_idx:%d,reason:%d\r\n", d_ind->conn_idx,d_ind->reason);
        break;
    }
    case BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT:
    {
        os_printf("BLE_5_DISCOVERY_PRIMARY_SERVICE_EVENT:");
        break;
    }
    case BLE_5_DISCOVERY_CHAR_EVENT:
    {
        os_printf("BLE_5_DISCOVERY_CHAR_EVENT:");
        break;
    }
    case BLE_5_SDP_REGISTER_FAILED:
        os_printf("BLE_5_SDP_REGISTER_FAILED\r\n");
        break;
    default:
        break;
    }
}

static void ble_central_cmd_cb(ble_cmd_t cmd, ble_cmd_param_t *param)
{
    os_printf("cmd:%d idx:%d status:%d\r\n", cmd, param->cmd_idx, param->status);
    switch (cmd)
    {
        case BLE_CREATE_ADV:
        case BLE_SET_ADV_DATA:
        case BLE_SET_RSP_DATA:
        case BLE_START_ADV:
        case BLE_STOP_ADV:
        case BLE_CREATE_SCAN:
            os_printf("BLE_CREATE_SCAN\r\n");
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
            if (ble_central_cmd_sema != NULL)
                ql_rtos_semaphore_release(ble_central_cmd_sema);
            break;
        default:
            break;
    }
}

static void ble_sdp_comm_callback(MASTER_COMMON_TYPE type,uint8 conidx,void *param)
{
    // os_printf("event:%d, idx:%d \r\n", type, conidx);
    if (MST_TYPE_ATT_UUID == type)
    {
        struct ble_sdp_char_inf *p_char = (struct ble_sdp_char_inf *) param ;
        if ((!os_memcmp(char_uuid, p_char->uuid, 2)))
        {
            g_char_info.char_hdl =  p_char->char_hdl;
            g_char_info.val_hdl = p_char->val_hdl;
            g_char_info.prop = p_char->prop;
            os_printf("char:%d, val:%d \r\n", p_char->char_hdl, p_char->val_hdl);
        }
    }

    if (MST_TYPE_SDP_END == type)
    {
        os_printf("%s MST_TYPE_SDP_END \n", __func__);
    }

    if (type == MST_TYPE_UPP_ASK)
    {
        struct mst_comm_updata_para *tmp = (typeof(tmp))param;
        os_printf("%s MST_TYPE_UPP_ASK accept\n", __func__);
        tmp->is_agree = 1;
    }
}

//receive slave data
void quec_app_sdp_charac_cb(CHAR_TYPE type,uint8 conidx,uint16_t hdl,uint16_t len,uint8 *data)
{
    os_printf("[APP]type:%x conidx:%d,handle:0x%02x(%d),len:%d,0x",type,conidx,hdl,hdl,len);
    for(int i = 0; i< len; i++)
    {
        os_printf("%02x ",data[i]);
    }

    os_printf("\r\n");
}

static void ql_ble_central_demo_entry(void *arg)
{
    ble_scan_param_t *scan_info = &g_scan_info;
    ble_err_t ret = 0;

    QL_LOG_ON_ERR_VOID(ql_rtos_semaphore_create(&ble_central_cmd_sema, 1));
    while (1)
    {
        if(ble_central_state == 0)
        {
            /* set ble event callback */
            ql_ble_set_notice_cb(ql_demo_central_notice_cb);

            /* set scan param */
            scan_info->scan_phy = 0x07;
            scan_info->scan_intv = 0x30;
            scan_info->scan_wd = 0x20;
            scan_info->own_addr_type  = 0;

            /* get idle actv_idx */
            scan_actv_idx = bk_ble_get_idle_conn_idx_handle();
            // if (scan_actv_idx >= ql_ble_get_max_idx_count(QL_ACTV_IDX))
            // {
            //     return ;
            // }
            /* as actv_idx idle */
            ql_ble_create_scaning(scan_actv_idx , scan_info->scan_intv, scan_info->scan_wd , ble_central_cmd_cb);
            ql_rtos_semaphore_wait(ble_central_cmd_sema, BEKEN_WAIT_FOREVER);
            ql_ble_start_scaning(scan_actv_idx, ble_central_cmd_cb);
            ql_rtos_semaphore_wait(ble_central_cmd_sema, BEKEN_WAIT_FOREVER);

            ble_central_state = 1;
        }

        if(1 == ble_central_state)
        {
            if(1 == scan_success_flag)
            {
                /*wait scaning stop*/
                ql_rtos_semaphore_wait(ble_central_cmd_sema, 5000);  // wait stoping  scan finished 

                /*get currnt idle actv_idx*/
                conn_actv_idx = ql_ble_get_idle_conn_idx_handle();
                os_printf("conn_actv_idx = %d\r\n" , conn_actv_idx);
                // if (conn_actv_idx >= ql_ble_get_max_idx_count(QL_CONN_IDX))
                // {
                //     return ;
                // }

                /*register callbak functions*/
                ql_ble_gattc_register_event_recv_callback(quec_app_sdp_charac_cb);
                //ql_ble_register_app_sdp_common_callback(ble_sdp_comm_callback);
                /*create contention actv*/
                os_printf("creat conn\r\n");
                ret = ql_ble_create_conn(conn_actv_idx, 0X40, 0, 0x200, ble_central_cmd_cb);
                os_printf("creat ret:%d\r\n", ret);
                ql_rtos_semaphore_wait(ble_central_cmd_sema, BEKEN_WAIT_FOREVER);
                os_printf("set connect address\r\n");
                QL_LOG_ON_ERR_VOID(ql_ble_set_connect_dev_addr(conn_actv_idx, (bd_addr_t *)conn_addr, adv_addr_type));
                os_printf("start connect ....\r\n");
                QL_LOG_ON_ERR_VOID(ql_ble_start_conn(conn_actv_idx , ble_central_cmd_cb));
                ql_rtos_semaphore_wait(ble_central_cmd_sema, BEKEN_WAIT_FOREVER);
                scan_success_flag = 0;
            }
        }
        ql_rtos_task_sleep_ms(20);
        if (0 != g_char_info.val_hdl )
        {
            if (0 == char_noti_enable)
            {
                bk_ble_gatt_write_ccc(conn_actv_idx, g_char_info.val_hdl+1, 0x01);  // enable notify
                char_noti_enable = 1;
            }
            os_printf("send data\r\n");
            bk_ble_gatt_write_value(conn_actv_idx, g_char_info.val_hdl, 16, char_uuid);
            ql_rtos_task_sleep_ms(2000);
        }        
    }

    ql_rtos_task_delete(NULL);
}

void ql_ble_demo_central_thread_create(void)
{
    QlOSStatus err = kNoErr;
    // todo  create new thread;
    err = ql_rtos_task_create(&ble_central_hdl,
                                8*1024,
                                BEKEN_APPLICATION_PRIORITY,
                                "ble_central_demo",
                                ql_ble_central_demo_entry,
                                0);

    if (err != kNoErr)
    {
        return ;
    }

    return;
}

#endif
