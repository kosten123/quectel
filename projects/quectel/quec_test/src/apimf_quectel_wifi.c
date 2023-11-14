/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#include "quec_apimf_includes.h"
#include "quec_apimf.h"


#define ql_wlan_log               os_printf

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x "
#define WLAN_DEFAULT_IP         "192.168.19.1"
#define WLAN_DEFAULT_GW         "192.168.19.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

/* CFG_WPA_CTRL_IFACE */
static const char *crypto_str[] = {
		"None",
		"WEP",
		"WPA_TKIP",
		"WPA_AES",
		"WPA2_TKIP",
		"WPA2_AES",
		"WPA2_MIXED",		////QL_SECURITY_TYPE_WPA3_SAE
		"WPA3_SAE",	  		/**< WPA3 SAE */
		"WPA3_WPA2_MIXED",	/** WPA3 SAE or WPA2 AES */
		"AUTO",
	};

//extern char funcid_str[48];

int ql_wifi_event_cb(void *arg, int event_module,
					  int event_id, void *event_data)
{
	wifi_event_ap_disconnected_t *ap_disconnected;
	wifi_event_ap_connected_t *ap_connected;
	wifi_event_sta_disconnected_t *sta_disconnected;
	wifi_event_sta_connected_t *sta_connected;
    wifi_linkstate_reason_t t_reason;

    bk_wifi_sta_get_linkstate_with_reason(&t_reason);
	BK_LOGE("event","s:%d,r:%d", t_reason.state, t_reason.reason_code);
	switch (event_id) 
	{
		case EVENT_WIFI_AP_CONNECTED:
			ap_connected = (wifi_event_ap_connected_t *)event_data;
			ql_wlan_log(BK_MAC_FORMAT" connected to AP\n", BK_MAC_STR(ap_connected->mac));
			break;

		case EVENT_WIFI_AP_DISCONNECTED:
			ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
			ql_wlan_log(BK_MAC_FORMAT" disconnected from AP\n", BK_MAC_STR(ap_disconnected->mac));
			break;

		case EVENT_WIFI_STA_CONNECTED:
			sta_connected = (wifi_event_sta_connected_t *)event_data;
			ql_wlan_log("STA connected to %s\n", sta_connected->ssid);
			break;

		case EVENT_WIFI_STA_DISCONNECTED:
			sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
			ql_wlan_log("STA disconnected, reason(%d)\n", sta_disconnected->disconnect_reason);
			break;

		default:
			ql_wlan_log("rx event <%d %d>\n", event_module, event_id);
			break;
	}

	return BK_OK;
}

int ql_netif_event_cb(void *arg, int event_module,
					  int event_id, void *event_data)
{
	netif_event_got_ip4_t *got_ip;

	switch (event_id) {
	case EVENT_NETIF_GOT_IP4:
		got_ip = (netif_event_got_ip4_t *)event_data;
		ql_wlan_log("%s got ip\n", got_ip->netif_if == NETIF_IF_STA ? "STA" : "unknown netif");
		break;
	default:
		ql_wlan_log("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
}

/**
 * @brief 
 * 
 * @param inNetworkInitPara 
 * @return int 
 */
void qlst_wlan_start(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		int ret;

        char *oob_ssid =param_str[PARAM_NUM_1];
        char *connect_key =param_str[PARAM_NUM_2];
        /*定义一个结构体，用于传入参数*/
        ql_network_init_s wNetConfig;
        ql_event_module_s event_mod = {EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED};
        int len;

        /*置空结构体*/
        memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

        /*检查SSID的长度，不能超过32字节*/
        len = os_strlen(oob_ssid);
        if(WLAN_SSID_MAX_LEN < len)
        {
            ql_wlan_log("ssid name more than 32 Bytes\r\n");
            return;
        }

        /*将SSID跟密码传入结构体*/
        os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
        os_strcpy((char *)wNetConfig.wifi_key, connect_key);

        /*当前为客户端模式*/
        wNetConfig.wifi_mode = QL_STATION;
        /*采用DHCP CLIENT的方式获得，从路由器动态获取IP地址*/
        wNetConfig.dhcp_mode = DHCP_CLIENT;
        wNetConfig.wifi_retry_interval = 100;

        ql_wlan_log("input ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
        ql_wlan_status_register_cb(event_mod ,ql_wifi_event_cb);
	    event_mod.event_id = EVENT_WIFI_STA_DISCONNECTED;
	    ql_wlan_status_register_cb(event_mod ,ql_netif_event_cb);
        /*启动ap*/
        ret =ql_wlan_start(&wNetConfig);
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
 * @param mode 
 * @return int 
 */
void qlst_wlan_stop(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
        uint8_t  mode =atoi(param_str[PARAM_NUM_1]);
        ret =ql_wlan_stop((ql_wifi_mode_e)mode);
        quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE,mode);	
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
 * @param ssid 
 * @return int 
 */
void qlst_wlan_start_scan(int param_num, char (*param_str)[PARAM_SIZE])
{
    char *oob_ssid=NULL;
    
    if (param_num == 0)
        oob_ssid=NULL;
    else if(param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1))
        oob_ssid =param_str[PARAM_NUM_1];
    else{
        oob_ssid=NULL;
        quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
 		quecst_printf(SYNC_LOG, "ret=%x param_num:%d\r\n", PARAM_ERROR,param_num);
        return;
    }
    int ret;
    ret =ql_wlan_start_scan((uint8_t*)oob_ssid);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}

/**
 * @brief 
 * 
 * @param results 
 * @return int 
 */
void qlst_wlan_sta_scan_result(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;
    ql_scan_result_s apList;
    ret =ql_wlan_sta_scan_result(&apList);

    int ap_num = apList.ApNum;
    int i;
    os_printf("Got ap count: %d\r\n", apList.ApNum);
    for (i = 0; i < ap_num; i++)
    os_printf("    \"%s\", " MACSTR "\b, %d, %s, %d\n",
                apList.ql_aplist_s[i].ssid, MAC2STR(apList.ql_aplist_s[i].bssid),
                apList.ql_aplist_s[i].appower, crypto_str[apList.ql_aplist_s[i].security],
                apList.ql_aplist_s[i].channel);
    free(apList.ql_aplist_s);

    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);

}

/**
 * @brief 
//  * 
//  * @param ssid_ary 
//  * @param ssid_num 
//  * @return int 
//  */
// int ql_wlan_start_assign_scan(UINT8 **ssid_ary, UINT8 ssid_num);


/**
 * @brief 
 * 
 * @return int 
 */
void qlst_wlan_start_monitor(int param_num, char (*param_str)[PARAM_SIZE])
{
    int ret;
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    ret =ql_wlan_start_monitor(0,0);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}

/**
 * @brief 
 * 
 * @return int 
 */
void qlst_wlan_stop_monitor(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;
    ret =ql_wlan_stop_monitor();
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}

/**
 * @brief 
 * 
 * @param ap_info 
 * @return int 
 */
void qlst_wlan_ap_para_info_get(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;
    char ssid[33] = {0};
    ql_network_init_ap_s ap_info;
    ret =ql_wlan_ap_para_info_get(&ap_info);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
    /*打印出获取的连接状态值*/
    os_memcpy(ssid, ap_info.wifi_ssid, 32);
	quecst_printf(SYNC_LOG,"softap:ssid=%s,channel=%d,dhcp=%d\r\n",
				  ssid, ap_info.channel, ap_info.dhcp_mode);
}
/**
 * @brief 
 * 
 * @param outNetpara 
 * @param inInterface 
 * @return int 
 */

void qlst_wlan_get_ip_status(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;
    ql_ip_status_s ipStatus;
    ret =ql_wlan_get_ip_status(&ipStatus,QL_STATION);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
    	/*打印获取的网络状态*/
	quecst_printf(SYNC_LOG, "dhcp=%d ip=%s gate=%s mask=%s mac=" MACSTR "\r\n",
				ipStatus.dhcp, ipStatus.ip, ipStatus.gate,
				ipStatus.mask, MAC2STR((unsigned char*)ipStatus.mac));
}
/**
 * @brief 
 * 
 * @param outStatus 
 * @return int 
 */
void qlst_wlan_get_link_status(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;
    char ssid[33] = {0};
    ql_link_status_s linkStatus;
    ret =ql_wlan_get_link_status(&linkStatus);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
    /*打印连接状态*/
    memcpy(ssid, linkStatus.ssid, 32);
	quecst_printf(SYNC_LOG, "sta:rssi=%d,ssid=%s,bssid=" MACSTR ",channel=%d",
				  linkStatus.wifi_strength, ssid, MAC2STR(linkStatus.bssid), linkStatus.channel);
}
/**
 * @brief 
 * 
 * @return int 
 */

void qlst_wlan_get_channel(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    int ret;

    ret =ql_wlan_get_channel();
    quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret>=0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE,ret);	
    quecst_printf(SYNC_LOG, "channel number=%d\r\n", ret);
}
/**
 * @brief 
 * 
 * @param channel 
 * @return int 
 */

void qlst_wlan_set_channel(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){ 
        int ret;
        int channel = atoi(param_str[PARAM_NUM_1]);
        ret =ql_wlan_set_channel(channel);
        quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE,ret);	
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
 * @param uri 
 * @return int 
 */

void qlst_wlan_ota_download(int param_num, char (*param_str)[PARAM_SIZE])
{
    uint32_t ret;
    char *uri=param_str[PARAM_NUM_1];
    ret =ql_wlan_ota_download((const char*)uri);
    quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
void qlst_sta_ip_is_start(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num)
        quecst_printf(SYNC_LOG, "<%s> should be no param", __func__);
    uint32_t ret;

    ret =ql_sta_ip_is_start();
    quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,CURRENT_FUNC_TYPE,ret);	
    quecst_printf(SYNC_LOG, "ret=%d\r\n", ret);
}


