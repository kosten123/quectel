/**
 * @file ql_wlan_demo.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include<quec_wlan/ql_wlan.h>


#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x "
#define WLAN_DEFAULT_IP         "192.168.19.1"
#define WLAN_DEFAULT_GW         "192.168.19.1"
#define WLAN_DEFAULT_MASK       "255.255.255.0"

#define ql_wlan_log os_printf

void ql_demo_softap_app_init(char *ap_ssid, char *ap_key);
void ql_demo_sta_app_init(char *oob_ssid,char *connect_key);
void ql_demo_ip_app_init(void);
void ql_demo_state_app_init(void);
extern int ql_sta_chiper_type(void);


static int scan_cb(void *arg, int event_module, int event_id, void *event_data)
{
	
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

	ScanResult_adv apList;
	if (wlan_sta_scan_result(&apList) == 0) {
		int ap_num = apList.ApNum;
		int i;

		ql_wlan_log("Got ap count: %d\r\n", apList.ApNum);
		for (i = 0; i < ap_num; i++)
			ql_wlan_log("    \"%s\", " MACSTR "\b, %d, %s, %d\n",
					apList.ApList[i].ssid, MAC2STR(apList.ApList[i].bssid),
					apList.ApList[i].ApPower, crypto_str[apList.ApList[i].security],
					apList.ApList[i].channel);
		os_free(apList.ApList);
	}
    return 0;
}

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



void ql_wlan_test_demo(void * arg)
{

	ql_demo_sta_app_init("ASUS", "12345678");
    while(1)
	{
		ql_rtos_task_sleep_ms(5000);
		ql_demo_state_app_init();
        ql_rtos_task_sleep_ms(5000);
		ql_demo_ip_app_init();
	}
}

ql_task_t	 wlan_test_thread_handle = NULL;
void ql_wlan_test_demo_thread_create(void)
{
    int ret; 

	ql_rtos_task_sleep_ms(3000);

	ret = ql_rtos_task_create(&wlan_test_thread_handle,
						 (unsigned short)4096,
						 BEKEN_DEFAULT_WORKER_PRIORITY,
						 "wlan_test",
						 ql_wlan_test_demo,
						 0);

	if (ret != kNoErr) {
	  os_printf("Error: Failed to create wlan config net test thread: %d\r\n",ret);
	  goto init_err;
	}

	return;

	init_err:
	 if( wlan_test_thread_handle != NULL ) {
		 ql_rtos_task_delete(wlan_test_thread_handle);
	 }
}

void ql_demo_scan_app_init(void)
{
	ql_wlan_scan_ap_reg_cb(scan_cb);
	ql_wlan_start_scan(NULL);
}

void ql_demo_scan_adv_app_init(uint8_t *oob_ssid)
{
    ql_wlan_scan_ap_reg_cb(scan_cb);
	ql_wlan_start_scan(oob_ssid);
	ql_wlan_log("scan for ssid:%s\r\n", oob_ssid);
}

void ql_demo_softap_app_init(char *ap_ssid, char *ap_key)
{
    ql_network_init_s wNetConfig;
	ql_event_module_s event_mod = {EVENT_MOD_WIFI, EVENT_WIFI_AP_CONNECTED};
    int len;

    os_memset(&wNetConfig, 0x0, sizeof(ql_network_init_s));

    len = os_strlen(ap_ssid);
    if(WLAN_SSID_MAX_LEN < len)
    {
        ql_wlan_log("ssid name more than 32 Bytes\r\n");
        return;
    }

    len = os_strlen(ap_key);
    if(63 < len)
    {
        bk_printf("key  more than 63 Bytes\r\n");
        return;
    }

    os_strcpy((char *)wNetConfig.wifi_ssid, ap_ssid);
    os_strcpy((char *)wNetConfig.wifi_key, ap_key);

    wNetConfig.wifi_mode = QL_SOFT_AP;
    wNetConfig.dhcp_mode = DHCP_SERVER;
    wNetConfig.wifi_retry_interval = 100;
    os_strcpy((char *)wNetConfig.local_ip_addr, WLAN_DEFAULT_IP);
    os_strcpy((char *)wNetConfig.net_mask, WLAN_DEFAULT_MASK);
    os_strcpy((char *)wNetConfig.gateway_ip_addr, WLAN_DEFAULT_GW);
    os_strcpy((char *)wNetConfig.dns_server_ip_addr, WLAN_DEFAULT_GW);

    ql_wlan_log("ssid:%s  key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	ql_wlan_start(&wNetConfig);

	ql_wlan_status_register_cb(event_mod ,ql_wifi_event_cb);
	event_mod.event_id = EVENT_WIFI_AP_DISCONNECTED;
	ql_wlan_status_register_cb(event_mod ,ql_wifi_event_cb);
}

void ql_demo_sta_app_init(char *oob_ssid,char *connect_key)
{
	ql_network_init_s wNetConfig;
	ql_event_module_s event_mod = {EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED};
    int len;

	os_memset(&wNetConfig, 0x0, sizeof(network_InitTypeDef_st));

    len = os_strlen(oob_ssid);
    if(WLAN_SSID_MAX_LEN < len)
    {
        ql_wlan_log("ssid name more than 32 Bytes\r\n");
        return;
    }

	os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
	os_strcpy((char *)wNetConfig.wifi_key, connect_key);

	wNetConfig.wifi_mode = QL_STATION;
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;

	ql_wlan_log("ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	ql_wlan_status_register_cb(event_mod ,ql_wifi_event_cb);
	event_mod.event_id = EVENT_WIFI_STA_DISCONNECTED;
	ql_wlan_status_register_cb(event_mod ,ql_netif_event_cb);
	ql_wlan_start(&wNetConfig);
}

void ql_demo_sta_adv_app_init(char *oob_ssid,char *connect_key)
{
	ql_network_InitTypeDef_adv_s	wNetConfigAdv;

	os_memset( &wNetConfigAdv, 0x0, sizeof(network_InitTypeDef_adv_st) );

	os_strcpy((char*)wNetConfigAdv.ap_info.ssid, oob_ssid);
	hwaddr_aton("48:ee:0c:48:93:12", (u8 *)(wNetConfigAdv.ap_info.bssid));
	wNetConfigAdv.ap_info.security = QL_SECURITY_TYPE_WPA2_MIXED;
	wNetConfigAdv.ap_info.channel = 11;

	os_strcpy((char*)wNetConfigAdv.key, connect_key);
	wNetConfigAdv.key_len = os_strlen(connect_key);
	wNetConfigAdv.dhcp_mode = DHCP_CLIENT;
	wNetConfigAdv.wifi_retry_interval = 100;

	//ql_wlan_start_sta_adv(&wNetConfigAdv);

	ql_wlan_test_demo_thread_create();
}

void ql_demo_wlan_app_init(VIF_ADDCFG_PTR cfg)
{
    ql_network_init_s network_cfg;
	ql_network_InitTypeDef_adv_s	networkadv_cfg;

    if(cfg->wlan_role == QL_STATION) {
        if(cfg->adv == 1) {
        	os_memset( &networkadv_cfg, 0x0, sizeof(network_InitTypeDef_adv_st) );

        	os_strcpy((char*)networkadv_cfg.ap_info.ssid, cfg->ssid);
        	hwaddr_aton("48:ee:0c:48:93:12", (u8 *)(networkadv_cfg.ap_info.bssid));
        	networkadv_cfg.ap_info.security = QL_SECURITY_TYPE_WPA2_MIXED;
        	networkadv_cfg.ap_info.channel = 11;

        	os_strcpy((char*)networkadv_cfg.key, cfg->key);
        	networkadv_cfg.key_len = os_strlen(cfg->key);
        	networkadv_cfg.dhcp_mode = DHCP_CLIENT;
        	networkadv_cfg.wifi_retry_interval = 100;

        	//ql_wlan_start_sta_adv(&networkadv_cfg);
            return;
        } else {
        	os_memset(&network_cfg, 0x0, sizeof(network_InitTypeDef_st));

        	os_strcpy((char *)network_cfg.wifi_ssid, cfg->ssid);
        	os_strcpy((char *)network_cfg.wifi_key, cfg->key);

        	network_cfg.wifi_mode = QL_STATION;
        	network_cfg.dhcp_mode = DHCP_CLIENT;
        	network_cfg.wifi_retry_interval = 100;

        	ql_wlan_log("ssid:%s key:%s\r\n", network_cfg.wifi_ssid, network_cfg.wifi_key);
        }
    } else if(cfg->wlan_role == QL_SOFT_AP) {
        os_memset(&network_cfg, 0x0, sizeof(network_InitTypeDef_st));
        os_strcpy((char *)network_cfg.wifi_ssid, cfg->ssid);
        os_strcpy((char *)network_cfg.wifi_key, cfg->key);

        network_cfg.wifi_mode = QL_SOFT_AP;
        network_cfg.dhcp_mode = DHCP_SERVER;
        network_cfg.wifi_retry_interval = 100;
        os_strcpy((char *)network_cfg.local_ip_addr, WLAN_DEFAULT_IP);
        os_strcpy((char *)network_cfg.net_mask, WLAN_DEFAULT_MASK);
        os_strcpy((char *)network_cfg.gateway_ip_addr, WLAN_DEFAULT_GW);
        os_strcpy((char *)network_cfg.dns_server_ip_addr, WLAN_DEFAULT_GW);

        ql_wlan_log("ssid:%s  key:%s\r\n", network_cfg.wifi_ssid, network_cfg.wifi_key);
    }

    ql_wlan_start(&network_cfg);

}

void ql_demo_state_app_init(void)
{
	ql_link_status_s linkStatus;
	ql_network_init_ap_s ap_info;
	char ssid[33] = {0};

	ql_wlan_log("sta: %d, softap: %d, b/g/n\r\n", sta_ip_is_start(), uap_ip_is_start());

	/*STATION mode connection status*/
	if (sta_ip_is_start()) {
		os_memset(&linkStatus, 0x0, sizeof(LinkStatusTypeDef));
		ql_wlan_get_link_status(&linkStatus);
		os_memcpy(ssid, linkStatus.ssid, 32);
		ql_wlan_log("sta:rssi=%d,ssid=%s,bssid=" MACSTR ",channel=%d,cipher_type:",
				  linkStatus.wifi_strength, ssid, MAC2STR(linkStatus.bssid), linkStatus.channel);
		switch (ql_sta_chiper_type()) {
		case QL_SECURITY_TYPE_NONE:
			ql_wlan_log("OPEN\r\n");
			break;
		case QL_SECURITY_TYPE_WEP :
			ql_wlan_log("WEP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_TKIP:
			ql_wlan_log("TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_AES:
			ql_wlan_log("WPA_AES\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_AES:
			ql_wlan_log("CCMP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_TKIP:
			ql_wlan_log("WPA2_TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_MIXED:
			ql_wlan_log("WPA/WPA2 MIXED\r\n");
			break;
		case QL_SECURITY_TYPE_AUTO:
			ql_wlan_log("AUTO\r\n");
			break;
		case QL_SECURITY_TYPE_WPA3_SAE:
			ql_wlan_log("WPA3\n");
			break;
		case QL_SECURITY_TYPE_WPA3_WPA2_MIXED:
			ql_wlan_log("WPA2/WPA3 MIXED\n");
			break;
		default:
			ql_wlan_log("Error\r\n");
			break;
		}
	}

	/*AP mode connection status*/
	if (uap_ip_is_start()) {
		os_memset(&ap_info, 0x0, sizeof(network_InitTypeDef_ap_st));
		ql_wlan_ap_para_info_get(&ap_info);
		os_memcpy(ssid, ap_info.wifi_ssid, 32);
		ql_wlan_log("softap:ssid=%s,channel=%d,dhcp=%d,cipher_type:\r\n",
				  ssid, ap_info.channel, ap_info.dhcp_mode);
		
		switch (ap_info.security) {
		case QL_SECURITY_TYPE_NONE:
			ql_wlan_log("OPEN\r\n");
			break;
		case QL_SECURITY_TYPE_WEP :
			ql_wlan_log("WEP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA_TKIP:
			ql_wlan_log("TKIP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_AES:
			ql_wlan_log("CCMP\r\n");
			break;
		case QL_SECURITY_TYPE_WPA2_MIXED:
			ql_wlan_log("WPA/WPA2 MIXED\r\n");
			break;
		case QL_SECURITY_TYPE_AUTO:
			ql_wlan_log("AUTO\r\n");
			break;
		case QL_SECURITY_TYPE_WPA3_SAE:
			ql_wlan_log("WPA3\n");
			break;
		case QL_SECURITY_TYPE_WPA3_WPA2_MIXED:
			ql_wlan_log("WPA2/WPA3 MIXED\n");
			break;
		default:
			ql_wlan_log("Error\r\n");
			break;
		}
		ql_wlan_log("ip=%s,gate=%s,mask=%s,dns=%s\r\n",
				  ap_info.local_ip_addr, ap_info.gateway_ip_addr, ap_info.net_mask, ap_info.dns_server_ip_addr);
	}
}

void ql_demo_ip_app_init(void)
{
	/*define a struct to istore ip status*/
    ql_ip_status_s ipStatus;

	/*set struct empty*/
	os_memset(&ipStatus, 0x0, sizeof(IPStatusTypedef));
	/*get net status，and save in struct*/
	ql_wlan_get_ip_status(&ipStatus, QL_STATION);

	/*orintf net status*/
	ql_wlan_log("dhcp=%d ip=%s gate=%s mask=%s mac=" MACSTR "\r\n",
				ipStatus.dhcp, ipStatus.ip, ipStatus.gate,
				ipStatus.mask, MAC2STR((unsigned char*)ipStatus.mac));
}

/* monitor callback function*/
void ql_demo_monitor_cb(uint8_t *data, int len, wifi_link_info_t *info)
{
	ql_wlan_log("len:%d\r\n", len);
		
	//Only for reference
	/*
	User can get ssid and key by prase monitor data,
	refer to the following code, which is the way airkiss
	use monitor get wifi info from data
	*/

}
