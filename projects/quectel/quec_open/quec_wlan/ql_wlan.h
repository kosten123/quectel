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

#include<quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C" {
#endif
#define QL_MAC_ADDR_LEN 6

/// Operating Channel Bandwidth
typedef enum 
{
    // 20MHz BW
    QL_WIFI_CHNL_BW_20,
    // 40MHz BW
    QL_WIFI_CHNL_BW_40,
    // 80MHz BW
    QL_WIFI_CHNL_BW_80,
    // 160MHz BW
    QL_WIFI_CHNL_BW_160,
    // 80+80MHz BW
    QL_WIFI_CHNL_BW_80P80,
    // Reserved BW
    QL_WIFI_CHNL_BW_OTHER,
}ql_wifi_chan_bandwidth;

typedef enum
{
    QL_WIFI_AP_BAND_24G      = 0, //2.4G
    QL_WIFI_AP_BAND_5G,
}ql_wifi_ap_band_mode_e;

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
} ql_wifi_mode_e;


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
 *  @brief  Input network paras.
 */
typedef struct 
{
    char wifi_mode;               /**< wifi mode: @ref wlanInterfaceTypedef.*/
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
    ql_wifi_ap_band_mode_e  band;
} ql_network_init_s;

typedef struct
{
    char    ssid[32];    /**< SSID of the wlan that needs to be connected. Example: "SSID String". */
    char    bssid[6];    /**< BSSID of the wlan needs to be connected. Example: {0xC8 0x93 0x46 0x11 0x22 0x33}. */
    uint8_t channel;     /**< Wlan's RF frequency, channel 0-13. 1-13 means a fixed channel
                            that can speed up a connection procedure, 0 is not a fixed input
                            means all channels are possible*/
    uint8_t security;
}   ql_apinfo_s;


typedef struct ql_network_InitTypeDef_adv_st
{
    ql_apinfo_s ap_info;         /**< @ref apinfo_adv_t. */
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

typedef struct ql_network_init_ap_st
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
    ql_wifi_ap_band_mode_e  band;
} ql_network_init_ap_s;

typedef struct 
{
    char ssid[33];  
    char appower;   
    uint8_t bssid[6];
    char channel;   
    uint8_t security;   
} ql_apliststruct;

typedef  struct  ql_scan_result_t
{
	char ApNum; 	  /**< The number of access points found in scanning.*/
    ql_apliststruct *ql_aplist_s;
} ql_scan_result_s;

typedef struct
{
	uint8_t dhcp;       /**< DHCP mode: @ref DHCP_Disable, @ref DHCP_Client, @ref DHCP_Server.*/
	char    ip[16];     /**< Local IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    gate[16];   /**< Router IP address on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    mask[16];   /**< Netmask on the target wlan interface: @ref wlanInterfaceTypedef.*/
    char    dns[16];    /**< DNS server IP address.*/
    char    mac[16];    /**< MAC address, example: "C89346112233".*/
    char    broadcastip[16];
} ql_ip_status_s;

typedef struct ql_link_status_t
{
    int conn_state;       /**< The link to wlan is established or not, 0: disconnected, 1: connected. */
    int wifi_strength;      /**< Signal strength of the current connected AP */
    uint8_t  ssid[32];      /**< SSID of the current connected wlan */
    uint8_t  bssid[6];      /**< BSSID of the current connected wlan */
    int      channel;       /**< Channel of the current connected wlan */
    uint8_t security;
} ql_link_status_s;

/*
 *  sturcture of wifi event module
*/
typedef struct
{
    int mod;
    int event_id;
}ql_event_module_s;

typedef struct 
{
    /// Array of 16-bit words that make up the MAC address.
    uint16_t array[QL_MAC_ADDR_LEN/2];
}ql_mac_addr;

typedef struct 
{
    /// Frame control
    uint16_t fctl;
    /// Duration/ID
    uint16_t durid;
    /// Address 1
    ql_mac_addr addr1;
    /// Address 2
    ql_mac_addr addr2;
    /// Address 3
    ql_mac_addr addr3;
    /// Sequence control
    uint16_t seq;
} ql_mac_hdr;

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
 * @param network_init_para 
 * @return int 
 */
int ql_wlan_start(ql_network_init_s *network_init_para);


/**
 * @brief 
 * 
 * @param mode 
 * @return int 
 */
int ql_wlan_stop(ql_wifi_mode_e mode);

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
int ql_wlan_sta_scan_result(ql_scan_result_s *results);

/**
 * @brief 
 * 
 * @param ssid_ary 
 * @param ssid_num 
 * @return int 
 */
int ql_wlan_start_assign_scan(UINT8 **ssid_ary, UINT8 ssid_num, int band);

/**
 * @brief 
 * 
 * @param evt_mod 
 * @param cb 
 * @return int 
 */
int ql_wlan_status_register_cb(ql_event_module_s evt_mod, ql_event_cb_t cb);

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_start_monitor(uint16_t frequency, ql_wifi_chan_bandwidth channel_type);

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
int ql_wlan_ap_para_info_get(ql_network_init_ap_s *ap_info);

/**
 * @brief 
 * 
 * @param outNetpara 
 * @param inInterface 
 * @return int 
 */
int ql_wlan_get_ip_status(ql_ip_status_s *outnetpara, ql_wifi_mode_e interface);

/**
 * @brief 
 * 
 * @param outStatus 
 * @return int 
 */
int ql_wlan_get_link_status(ql_link_status_s *outstatus);

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
 * @param uri 
 * @return int 
 */
ql_wifi_errcode_e ql_wlan_ota_download(const char *uri);

/**
 * @brief Enter the low-power keepalive mode
 * 
 * @param uri 
 * @return int 
 */
uint32_t ql_wlan_pm_enable(void);  //TODO: set pm type


/**
 * @brief 
 * 
 * @return uint32_t 
 */
bool ql_ap_ip_is_start(void);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
bool ql_sta_ip_is_start(void);



/**
 * @brief 
 * 
 * @param txt 
 * @param addr 
 * @return int 
 */
extern int hwaddr_aton(const char *txt, u8 *addr);

#ifdef __cplusplus
}
#endif
#endif
