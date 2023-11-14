/**
 * @file ql_wlan_bk7256.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<quec_wlan/ql_wlan.h>

#define WLAN_DEBUG
#ifdef WLAN_DEBUG
#define ql_wlan_log os_printf
#else
#define ql_wlan_log os_null_printf
#endif



/**
 * @brief 
 * 
 * @param ct 
 * @return int 
 */
ql_wifi_errcode_e ql_wlan_set_country(ql_wifi_country_e ct)
{
    if(ct<0||ct>2){
        return -1;
    }

    wifi_country_t cty={0};
    switch (ct)
    {
    case CN:
        // strcpy(cty.cc,"CN");
        cty.cc[0]='C';
        cty.cc[1]='N';
        cty.cc[2]='\0';
        cty.schan=1;
        cty.nchan=13;
        cty.max_tx_power=0;
        cty.policy=WIFI_COUNTRY_POLICY_AUTO;
        break;
    case JP:
        // strcpy(cty.cc,"JP");
        cty.cc[0]='J';
        cty.cc[1]='P';
        cty.cc[2]='\0';
        cty.schan=1;
        cty.nchan=14;
        cty.max_tx_power=0;
        cty.policy=WIFI_COUNTRY_POLICY_AUTO;
        break;
    case USA:
        cty.cc[0]='U';
        cty.cc[1]='S';
        cty.cc[2]='A';
        cty.schan=1;
        cty.nchan=11;
        cty.max_tx_power=0;
        cty.policy=WIFI_COUNTRY_POLICY_AUTO;
        break;
    default:
        break;
    }
    if(bk_wifi_set_country(&cty)==0){
        return QL_WIFI_SUCCESS;
    }
    else{
        return QL_WIFI_EXECUTE_ERR;
    }
}

int ql_wlan_start(ql_network_init_s *inNetworkInitPara)
{
    if(inNetworkInitPara == NULL)
    {
        return -1;
    }
    
    ql_wlan_log("mode:%d\r\n", inNetworkInitPara->wifi_mode);
    if (QL_SOFT_AP == inNetworkInitPara->wifi_mode)
    {
        wifi_ap_config_t ap_config = WIFI_DEFAULT_AP_CONFIG();
        netif_ip4_config_t ip4_config = {0};

        strncpy(ip4_config.ip, inNetworkInitPara->local_ip_addr, NETIF_IP4_STR_LEN);
        strncpy(ip4_config.mask, inNetworkInitPara->net_mask, NETIF_IP4_STR_LEN);
        strncpy(ip4_config.gateway, inNetworkInitPara->gateway_ip_addr, NETIF_IP4_STR_LEN);
        strncpy(ip4_config.dns, inNetworkInitPara->dns_server_ip_addr, NETIF_IP4_STR_LEN);
        QL_LOG_ON_ERR(bk_netif_set_ip4_config(NETIF_IF_AP, &ip4_config));

        strncpy((char *)ap_config.ssid, inNetworkInitPara->wifi_ssid, WIFI_SSID_STR_LEN);
        strncpy(ap_config.password, inNetworkInitPara->wifi_key, WIFI_PASSWORD_LEN);

        ql_wlan_log("ssid:%s  key:%s\r\n", ap_config.ssid, ap_config.password);
        QL_LOG_ON_ERR(bk_wifi_ap_set_config(&ap_config));
        QL_LOG_ON_ERR(bk_wifi_ap_start());
    }
    else if (QL_STATION == inNetworkInitPara->wifi_mode)
    {
        wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();

        strncpy((char *)sta_config.ssid, inNetworkInitPara->wifi_ssid, WIFI_SSID_STR_LEN);
        strncpy(sta_config.password, inNetworkInitPara->wifi_key, WIFI_PASSWORD_LEN);
        strncpy((char *)sta_config.bssid, inNetworkInitPara->wifi_bssid, WIFI_BSSID_LEN);

        ql_wlan_log("ssid:%s password:%s\n", sta_config.ssid, sta_config.password);
        QL_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
        QL_LOG_ON_ERR(bk_wifi_sta_start());
    }

    return 0;
}

/**
 * @brief 
 * 
 * @param inNetworkInitParaAdv 
 * @return int 
 */
int ql_wlan_start_sta_adv(ql_network_InitTypeDef_adv_s *inNetworkInitParaAdv)
{
    if(inNetworkInitParaAdv == NULL)
        return -1;

    wifi_sta_config_t sta_config = WIFI_DEFAULT_STA_CONFIG();

    strncpy((char *)sta_config.ssid, inNetworkInitParaAdv->ap_info.ssid, WIFI_SSID_STR_LEN);
    strncpy(sta_config.password, inNetworkInitParaAdv->key, WIFI_PASSWORD_LEN);
    strncpy((char *)sta_config.bssid, inNetworkInitParaAdv->ap_info.bssid, WIFI_BSSID_LEN);
    sta_config.channel = inNetworkInitParaAdv->ap_info.channel;
    sta_config.security = inNetworkInitParaAdv->ap_info.security;

    ql_wlan_log("ssid:%s password:%s\n", sta_config.ssid, sta_config.password);
    QL_LOG_ON_ERR(bk_wifi_sta_set_config(&sta_config));
    QL_LOG_ON_ERR(bk_wifi_sta_start());

    return 0;
}

/**
 * @brief 
 * 
 * @param mode 
 * @return int 
 */
int ql_wlan_stop(ql_wifi_mode_e mode)
{
    if (QL_SOFT_AP == mode)
    {
        QL_LOG_ON_ERR(bk_wifi_ap_stop());
    }
    else if (QL_STATION == mode)
    {
        QL_LOG_ON_ERR(bk_wifi_sta_stop());
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param ssid 
 * @return int 
 */
int ql_wlan_start_scan(uint8_t *ssid)
{
    wifi_scan_config_t scan_config = {0};
    wifi_scan_config_t *p_config = NULL;

    if (NULL != ssid)
    {
        strncpy(scan_config.ssid, (char *)ssid, WIFI_SSID_STR_LEN);
        p_config = &scan_config;
    }
    BK_LOG_ON_ERR(bk_wifi_scan_start(p_config));
    return 0;
}

/**
 * @brief 
 * 
 * @param ind_cb 
 * @return int 
 */
int ql_wlan_scan_ap_reg_cb(ql_event_cb_t ind_cb)
{
    QL_LOG_ON_ERR(bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_SCAN_DONE,
							   (event_cb_t)ind_cb, NULL));
    return 0;
}

/**
 * @brief 
 * 
 * @param evt_mod 
 * @param cb 
 * @return int 
 */
int ql_wlan_status_register_cb(ql_event_module_s evt_mod, ql_event_cb_t cb)
{
    if (EVENT_MOD_COUNT <= evt_mod.mod)
    {
        return -1;
    }
    QL_LOG_ON_ERR(bk_event_register_cb(evt_mod.mod, evt_mod.event_id,
							   (event_cb_t)cb, NULL));
    return 0;
}

/**
 * @brief 
 * 
 * @param results 
 * @return int 
 */
int ql_wlan_sta_scan_result(ql_scan_result_s *results)
{

    return wlan_sta_scan_result((ScanResult_adv*)results);

}

/**
 * @brief 
 * 
 * @param ssid_ary 
 * @param ssid_num 
 * @return int 
 */
int ql_wlan_start_assign_scan(UINT8 **ssid_ary, UINT8 ssid_num,int band)
{
    bk_wlan_start_assign_scan(ssid_ary , ssid_num);
    return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_start_monitor(uint16_t frequency, ql_wifi_chan_bandwidth channel_type)
{
    return bk_wifi_monitor_start();
}

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_stop_monitor(void)
{
    return bk_wifi_monitor_stop();
}

/**
 * @brief 
 * 
 * @param fn 
 * @return int 
 */
int ql_wlan_register_monitor_cb(ql_monitor_cb_t fn)
{
    bk_wifi_monitor_register_cb((wifi_monitor_cb_t)fn);
    return 0;
}

/**
 * @brief 
 * 
 * @param outNetpara 
 * @param inInterface 
 * @return int 
 */
int ql_wlan_get_ip_status(ql_ip_status_s *outNetpara, ql_wifi_mode_e inInterface)
{
    if(outNetpara == NULL)
    {
        return -1;
    }
	bk_err_t res = BK_OK;
    netif_ip4_config_t t_ip4 ;
    netif_if_t t_netif = NETIF_IF_STA;

    if (QL_SOFT_AP == inInterface)
    {
        t_netif = NETIF_IF_AP;
    }

    memset(&t_ip4, 0, sizeof(netif_ip4_config_t));
    res = bk_netif_get_ip4_config(t_netif, &t_ip4);
    if (BK_OK != res)
    {
        ql_wlan_log("get ip error:%d\n", res);
        return -1;
    }
    strncpy(outNetpara->ip, t_ip4.ip, NETIF_IP4_STR_LEN);
    strncpy(outNetpara->mask, t_ip4.mask, NETIF_IP4_STR_LEN);
    strncpy(outNetpara->gate, t_ip4.gateway, NETIF_IP4_STR_LEN);
    strncpy(outNetpara->dns, t_ip4.dns, NETIF_IP4_STR_LEN);

    return 0;
}

/**
 * @brief 
 * 
 * @param outStatus 
 * @return int 
 */
int ql_wlan_get_link_status(ql_link_status_s *outStatus)
{
    if(outStatus == NULL)
        return -1;

    return bk_wlan_get_link_status((LinkStatusTypeDef *)outStatus);
}

/**
 * @brief 
 * 
 * @param ap_info 
 * @return int 
 */
int ql_wlan_ap_para_info_get(ql_network_init_ap_s *ap_info)
{
    if(ap_info == NULL)
    {
        return -1;
    }

	bk_err_t res = BK_OK;
    netif_ip4_config_t t_ip4 ;
    wifi_link_status_t t_link;

    memset(&t_ip4, 0, sizeof(netif_ip4_config_t));
    memset(&t_link, 0, sizeof(wifi_link_status_t));

    res = bk_netif_get_ip4_config(NETIF_IF_STA, &t_ip4);
    if (BK_OK != res)
    {
        ql_wlan_log("get ip error:%d\n", res);
        return -1;
    }
    strncpy(ap_info->local_ip_addr, t_ip4.ip, NETIF_IP4_STR_LEN);
    strncpy(ap_info->net_mask, t_ip4.mask, NETIF_IP4_STR_LEN);
    strncpy(ap_info->gateway_ip_addr, t_ip4.gateway, NETIF_IP4_STR_LEN);
    strncpy(ap_info->dns_server_ip_addr, t_ip4.dns, NETIF_IP4_STR_LEN);

    res = bk_wifi_sta_get_link_status(&t_link);
    if (BK_OK != res)
    {
        ql_wlan_log("get link status error:%d\n", res);
        return -1;
    }
    strncpy(ap_info->wifi_ssid, t_link.ssid, WIFI_SSID_STR_LEN);
    strncpy(ap_info->wifi_key, t_link.password, WIFI_PASSWORD_LEN);
    ap_info->channel = t_link.channel;
    ap_info->security = t_link.security;

    return 0;
}

/**
 * @brief 
 * 
 * @return int 
 */
int ql_wlan_get_channel(void )
{
    return bk_wifi_get_channel();
}

/**
 * @brief 
 * 
 * @param channel 
 * @return int 
 */
int ql_wlan_set_channel(int channel)
{
    return bk_wlan_set_channel(channel);
}

/**
 * @brief 
 * 
 * @param channel 
 * @return int 
 */
int ql_wlan_set_channel_sync(int channel)
{
    return bk_wlan_set_channel_sync(channel);
}

/**
 * @brief 
 * 
 * @return int 
 */
int ql_sta_chiper_type(void)
{
    return bk_sta_cipher_type();
}

/**
 * @brief 
 * 
 * @param uri 
 * @return int 
 */
ql_wifi_errcode_e ql_wlan_ota_download(const char *uri)
{
    if(uri == NULL)
        return QL_WIFI_INVALID_PARAM_ERR;
    
    return bk_http_ota_download(uri);
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t ql_uap_ip_is_start(void)
{ 
    return uap_ip_is_start();
}

/**
 * @brief 
 * 
 * @return uint32_t 
 */
bool ql_sta_ip_is_start(void)
{
    return sta_ip_is_start();
}
