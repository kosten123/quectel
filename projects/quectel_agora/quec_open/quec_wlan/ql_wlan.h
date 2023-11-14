/**
 * @file ql_wlan_pub.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __QL_WLAN_H_
#define __QL_WLAN_H_

#include<quec_common/ql_platform.h>

typedef enum
{
    QL_WIFI_SUCCESS      = 0,
    QL_WIFI_EXECUTE_ERR,
    QL_WIFI_INVALID_PARAM_ERR,
}ql_wifi_errcode_e;

typedef enum
{
    CN = 0,
    JP,
    USA,
}ql_wifi_country_e;

typedef struct{
    char country[3];
    uint8_t schan;
    uint8_t nchan;
    int8_t max_tx_power;
    wifi_country_policy_t policy;
}ql_wifi_country_t;

typedef struct
{
	int8_t rssi;
}ql_wifi_link_info_t;

/**
 *  @brief  wlan network interface enumeration definition.
 */
typedef enum
{
    QL_SOFT_AP ,  /**< Act as an access point, and other station can connect, 4 stations Max*/
    QL_STATION ,   /**< Act as a station which can connect to an access point*/
} ql_wlanInterface_Typedef_e;


/**
 *  @brief  Wi-Fi security type enumeration definition.
 */
typedef enum 
{
    QL_SECURITY_TYPE_NONE,        /**< Open system. */
    QL_SECURITY_TYPE_WEP,         /**< Wired Equivalent Privacy. WEP security. */
    QL_SECURITY_TYPE_WPA_TKIP,    /**< WPA /w TKIP */
    QL_SECURITY_TYPE_WPA_AES,     /**< WPA /w AES */
    QL_SECURITY_TYPE_WPA2_TKIP,   /**< WPA2 /w TKIP */
    QL_SECURITY_TYPE_WPA2_AES,    /**< WPA2 /w AES */
    QL_SECURITY_TYPE_WPA2_MIXED,  /**< WPA2 /w AES or TKIP */
    QL_SECURITY_TYPE_WPA3_SAE,	  /**< WPA3 SAE */
    QL_SECURITY_TYPE_WPA3_WPA2_MIXED, /** WPA3 SAE or WPA2 AES */
    QL_SECURITY_TYPE_EAP,
    QL_SECURITY_TYPE_OWE,
    QL_SECURITY_TYPE_AUTO,        /**< It is used when calling @ref bkWlanStartAdv, _BK_ read security type from scan result. */
}ql_wlan_sec_type_e;

/**
 *  @brief  Input network paras, used in bk_wlan_start function.
 */
typedef struct 
{
    char wifi_mode;               /**< DHCP mode: @ref wlanInterfaceTypedef.*/
    char wifi_ssid[33];           /**< SSID of the wlan needs to be connected.*/
    char wifi_key[64];            /**< Security key of the wlan needs to be connected, ignored in an open system.*/
    char local_ip_addr[16];       /**< Static IP configuration, Local IP address. */
    char net_mask[16];            /**< Static IP configuration, Netmask. */
    char gateway_ip_addr[16];     /**< Static IP configuration, Router IP address. */
    char dns_server_ip_addr[16];   /**< Static IP configuration, DNS server IP address. */
    char dhcp_mode;                /**< DHCP mode, @ref DHCP_Disable, @ref DHCP_Client and @ref DHCP_Server. */
    char wifi_bssid[6];
    char reserved[26];
    int  wifi_retry_interval;     /**< Retry interval if an error is occured when connecting an access point,
                                     time unit is millisecond. */
} ql_network_InitTypeDef_s;

typedef struct
{
    char    ssid[32];    /**< SSID of the wlan that needs to be connected. Example: "SSID String". */
    char    bssid[6];    /**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
    uint8_t channel;     /**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
                            that can speed up a connection procedure, 0 is not a fixed input
                            means all channels are possible*/
    uint8_t security;
}   ql_apinfo_adv_s;


typedef struct ql_network_InitTypeDef_adv_st
{
    ql_apinfo_adv_s ap_info;         /**< @ref apinfo_adv_t. */
    char  key[64];                /**< Security key or PMK of the wlan. */
    int   key_len;                /**< The length of the key. */
    char  local_ip_addr[16];      /**< Static IP configuration, Local IP address. */
    char  net_mask[16];           /**< Static IP configuration, Netmask. */
    char  gateway_ip_addr[16];    /**< Static IP configuration, Router IP address. */
    char  dns_server_ip_addr[16];  /**< Static IP configuration, DNS server IP address. */
    char  dhcp_mode;               /**< DHCP mode, @ref DHCP_Disable, @ref DHCP_Client and @ref DHCP_Server. */
    char  reserved[32];
    int   wifi_retry_interval;    /**< Retry interval if an error is occured when connecting an access point,
                                  time unit is millisecond. */
} ql_network_InitTypeDef_adv_s;

typedef struct ql_network_InitTypeDef_ap_st
{
    char wifi_ssid[32];
    char wifi_key[64];
    uint8_t channel;
    wifi_security_t security;
	uint8_t ssid_hidden;
	uint8_t max_con;
    char local_ip_addr[16];
    char net_mask[16];
    char gateway_ip_addr[16];
    char dns_server_ip_addr[16];
    char dhcp_mode;
    char reserved[32];
    int  wifi_retry_interval;
} ql_network_InitTypeDef_ap_s;

typedef  struct  ql_ScanResult_adv
{
	char ApNum; 	  /**< The number of access points found in scanning.*/
	struct ql_ApListStruct
	{
		char ssid[33];	
		char ApPower;	
		uint8_t bssid[6];
		char channel;	
		uint8_t security;	
	} *ql_ApList_s;
} ql_ScanResult_adv_s;

typedef struct
{
	uint8_t dhcp;       /**< DHCP mode: @ref DHCP_Disable, @ref DHCP_Client, @ref DHCP_Server.*/
	char    ip[16];     /**< Local IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    gate[16];   /**< Router IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    mask[16];   /**< Netmask on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    dns[16];    /**< DNS server IP address.*/
    char    mac[16];    /**< MAC address, example: "C89346112233".*/
    char    broadcastip[16];
} ql_IPStatusTypedef_s;

typedef struct ql_linkStatus_t
{
    int conn_state;       /**< The link to wlan is established or not, 0: disconnected, 1: connected. */
    int wifi_strength;      /**< Signal strength of the current connected AP */
    uint8_t  ssid[32];      /**< SSID of the current connected wlan */
    uint8_t  bssid[6];      /**< BSSID of the current connected wlan */
    int      channel;       /**< Channel of the current connected wlan */
    uint8_t security;
} ql_LinkStatusTypeDef_s;

/*
 *  sturcture of wifi event module
*/
typedef struct
{
    int mod;
    int event_id;
}ql_EventModule_s;

/**
 * @brief 
 * 
 */
typedef void (*ql_func_scan_callback)(void *arg, unsigned char vif_idx);

/**
 * @brief 
 * 
 */
typedef void (*ql_monitor_cb_t)(uint8_t *data, int len, ql_wifi_link_info_t *info);

/**
 * @brief 
 * 
 */
typedef int (*ql_event_cb_t)(void *arg, int event_module,int event_id, void *event_data);

/**
 * @brief 
 * @param  ct
 * @return int 
 */
ql_wifi_errcode_e ql_wlan_set_country(ql_wifi_country_e ct);

/**
 * @brief 
 * 
 * @param inNetworkInitPara 
 * @return int 
 */
int ql_wlan_start(ql_network_InitTypeDef_s *inNetworkInitPara);

/**
 * @brief 
 * 
 * @param inNetworkInitParaAdv 
 * @return int 
 */
int ql_wlan_start_sta_adv(ql_network_InitTypeDef_adv_s *inNetworkInitParaAdv);

/**
 * @brief 
 * 
 * @param mode 
 * @return int 
 */
int ql_wlan_stop(ql_wlanInterface_Typedef_e mode);

/**
 * @brief 
 * 
 * @param ssid 
 * @return int 
 */
int ql_wlan_start_scan(uint8_t *ssid);

/**
 * @brief 
 * 
 * @param ind_cb 
 * @return int 
 */
int ql_wlan_scan_ap_reg_cb(ql_event_cb_t ind_cb);

/**
 * @brief 
 * 
 * @param results 
 * @return int 
 */
int ql_wlan_sta_scan_result(ql_ScanResult_adv_s *results);

/**
 * @brief 
 * 
 * @param ssid_ary 
 * @param ssid_num 
 * @return int 
 */
int ql_wlan_start_assign_scan(UINT8 **ssid_ary, UINT8 ssid_num);

/**
 * @brief 
 * 
 * @param evt_mod 
 * @param cb 
 * @return int 
 */
int ql_wlan_status_register_cb(ql_EventModule_s evt_mod, ql_event_cb_t cb);

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_start_monitor(void);

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_stop_monitor(void);

/**
 * @brief 
 * 
 * @param fn 
 * @return int 
 */
int ql_wlan_register_monitor_cb(ql_monitor_cb_t fn);

/**
 * @brief 
 * 
 * @param ap_info 
 * @return int 
 */
int ql_wlan_ap_para_info_get(ql_network_InitTypeDef_ap_s *ap_info);

/**
 * @brief 
 * 
 * @param outNetpara 
 * @param inInterface 
 * @return int 
 */
int ql_wlan_get_ip_status(ql_IPStatusTypedef_s *outNetpara, ql_wlanInterface_Typedef_e inInterface);

/**
 * @brief 
 * 
 * @param outStatus 
 * @return int 
 */
int ql_wlan_get_link_status(ql_LinkStatusTypeDef_s *outStatus);

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_get_channel(void );

/**
 * @brief 
 * 
 * @param channel 
 * @return int 
 */
int ql_wlan_set_channel(int channel);

/**
 * @brief 
 * 
 * @param channel 
 * @return int 
 */
int ql_wlan_set_channel_sync(int channel);

/**
 * @brief 
 * 
 * @return int 
 */
int ql_sta_chiper_type(void);

/**
 * @brief 
 * 
 * @param uri 
 * @return int 
 */
ql_wifi_errcode_e ql_wlan_ota_download(const char *uri);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t ql_uap_ip_is_start(void);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t ql_sta_ip_is_start(void);

/**
 * @brief 
 * 
 */
void ql_demo_scan_app_init(void);

/**
 * @brief 
 * 
 * @param oob_ssid 
 */
void ql_demo_scan_adv_app_init(uint8_t * oob_ssid);

/**
 * @brief 
 * 
 * @param ap_ssid 
 * @param ap_key 
 */
void ql_demo_softap_app_init(char *ap_ssid, char *ap_key);

/**
 * @brief 
 * 
 * @param oob_ssid 
 * @param connect_key 
 */
void ql_demo_sta_app_init(char *oob_ssid,char *connect_key);

/**
 * @brief 
 * 
 * @param oob_ssid 
 * @param connect_key 
 */
void ql_demo_sta_adv_app_init(char *oob_ssid,char *connect_key);

/**
 * @brief 
 * 
 * @param cfg 
 */
void ql_demo_wlan_app_init(VIF_ADDCFG_PTR cfg);

/**
 * @brief 
 * 
 */
void ql_demo_state_app_init(void);

/**
 * @brief 
 * 
 */
void ql_demo_ip_app_init(void);

/**
 * @brief 
 * 
 */
void ql_wlan_cfgnet_demo_thread_creat(void);

/**
 * @brief 
 * 
 * @param txt 
 * @param addr 
 * @return int 
 */
extern int hwaddr_aton(const char *txt, u8 *addr);

#endif
