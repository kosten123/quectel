
#include<quec_osi/ql_osi.h>
#include<quec_common/ql_defines.h>

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "wizchip_ethnetif.h"
#include "lwip/prot/ethernet.h"
#include "lwip/etharp.h"
#include "lwip/ping.h"

#include "wizchip_conf.h"
#include "w5500.h"
#include<quec_wlan/ql_wlan.h>
#include<quec_timer/ql_timer.h>

#define wz_log	bk_printf

#define USE_DHCP  0

#define W5500_MAC  {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC}
#define W5500_MAC_1 {0x00, 0x0e, 0xc6, 0x25, 0x8a, 0x9e}

#if USE_DHCP
#define W5500_IP   "0.0.0.0"
#define W5500_MASK "0.0.0.0"
#define W5500_GW   "0.0.0.0"

#else
#define W5500_IP   "192.168.3.50"
#define W5500_MASK "255.255.255.0"
#define W5500_GW   "192.168.3.1"

#endif

#define TCP_SERVER_DOMAIN "192.168.3.51"
#define TCP_SERVER_PORT 8000


#define TCP_CONNECT_TIMEOUT_S 10
#define TCP_RECV_TIMEOUT_S 3
#define TCP_CLOSE_LINGER_TIME_S 10
#define TCP_CLIENT_SEND_STR "GET / HTTP/1.1\r\nHost: www.baidu.com:80\r\n\r\n"


#define WZ_SOCKET_TEST 0

#if (WZ_SOCKET_TEST == 1)

ql_task_t wz_socket_test_ref = NULL;
void wz_socket_test(void *argv) {
	int				ret			= 0;
    int				sock_nbio	= 1;
	int				sock_fd     = -1;
	int				sock_error  = 0;
	socklen_t		optlen = 0;
	fd_set 			read_fds, write_fds;
	struct timeval	t;
    struct addrinfo		* res, hints;
	struct sockaddr_in	* ip4_svr_addr;
    int dns_success = 0;
	unsigned char recv_buf[1024] = {0};
	struct netif * netif = NULL;
	char ip[16];

	
	//ip_addr_t ip_t , mask , gw ;

    ql_rtos_task_sleep_s(1);

    wz_log("wz_socket_test start\n");

    uint8_t mac[6] = W5500_MAC;

    if(wz_ethnetif_register(mac, W5500_IP, W5500_MASK, W5500_GW, WZ_ETHNETIF_NODE))
    {
        wz_log("W5500 NIC FAILLLLLLL");
        goto exit;
    }
    else
    {
        wz_log("W5500 NIC OKKKKKKKKK \n");
        wz_ethnetif_set_dns("8.8.8.8", "114.114.114.114");
    #if USE_DHCP
        wz_ethnetif_dhcp();
    #else
        wz_ethnetif_set_up();
    #endif
    }

	netif = netif_find("wz");
	if(netif)
	{
		os_strcpy(ip, ipaddr_ntoa(&netif->ip_addr));
		wz_log("********find netif ip:%s\n", ip );
	}

	
#if 0	
	while(1)
	{
		ql_rtos_task_sleep_s(1);
	}
#endif

#if 1
	memset(&hints, 0, sizeof(struct addrinfo));
	//return;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(TCP_SERVER_DOMAIN, NULL, &hints, &res) != 0)
	{
		wz_log("*** DNS fail ***\r\n");
		goto exit;
	}

	dns_success = 1;

	ret = socket(AF_INET, SOCK_STREAM, 0);
	if(ret < 0)
	{
		wz_log("*** socket create fail ***\r\n");
		goto exit;
	}

	sock_fd = ret;

    ioctlsocket(sock_fd, FIONBIO, &sock_nbio);

	ip4_svr_addr = (struct sockaddr_in *)res->ai_addr;
	ip4_svr_addr->sin_port = htons(TCP_SERVER_PORT);

	ret = connect(sock_fd, (struct sockaddr *)ip4_svr_addr, sizeof(struct sockaddr));

	wz_log("connect ret: %d, errno: %u\r\n", ret, errno);

	if(ret == -1 && errno != EINPROGRESS)
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	t.tv_sec = TCP_CONNECT_TIMEOUT_S;
	t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	FD_SET(sock_fd, &read_fds);
	FD_SET(sock_fd, &write_fds);

	ret = select(sock_fd + 1, &read_fds, &write_fds, NULL, &t);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(!FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else if(FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		optlen = sizeof(sock_error);
		ret = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &sock_error, &optlen);
		if(ret == 0 && sock_error == 0)
		{
			wz_log("connect success\r\n");
		}
		else
		{
			wz_log("*** connect fail, sock_err = %d, errno = %u ***\r\n", sock_error, errno);
			goto exit;
		}
	}
	else if(!FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("connect success\r\n");
	}
	else if(FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	ret = send(sock_fd, (const void*)TCP_CLIENT_SEND_STR, strlen(TCP_CLIENT_SEND_STR), 0);
	if(ret < 0)
	{
		wz_log("*** send fail ***\r\n");
		goto exit;
	}

_recv_:

	//t.tv_sec = TCP_RECV_TIMEOUT_S;
	t.tv_sec = 30;
	t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);

	ret = select(sock_fd + 1, &read_fds, NULL, NULL, &t);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(FD_ISSET(sock_fd, &read_fds))
	{
		ret = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
		if(ret > 0)
		{
			wz_log("recv data: [%d]%s\r\n", ret, recv_buf);
			memset(recv_buf,0,1024);
            goto _recv_;
		}
		else if(ret == 0)
		{
			wz_log("*** peer closed ***\r\n");
			goto exit;
		}
		else
		{
			if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
			{
				wz_log("*** error occurs ***\r\n");
				goto exit;
			}
			else
			{
				wz_log("wait for a while\r\n");
				goto _recv_;
			}
		}
	}

exit:
    if(dns_success) freeaddrinfo(res);
	
	if(sock_fd >= 0)
	{
		struct linger linger = {0};

		linger.l_onoff = 1;
		linger.l_linger = TCP_CLOSE_LINGER_TIME_S;

		setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger));
      //  setsockopt(sock_fd, IPPROTO_TCP, TCP_CLOSE_TIMEROUT, &linger.l_linger, sizeof(linger.l_linger));
		
		close(sock_fd);
	}

    ql_rtos_task_delete(wz_socket_test_ref);
#endif
}
#endif 


void wifi_connect(void);

void wifi_connect_task(void *argv);
void net_link_check(void *argv);
void do_tcp_client(void);
volatile uint8_t net_wire_link_state =0;
volatile uint8_t net_usable_flag = 0;
static uint8_t net_link_status_change_flag =0;
volatile uint8_t wifi_satate=0;
ql_task_t wz_socket_wifi_connect_ref = NULL;

ql_task_t wz_socket_wifi_test_ref = NULL;
ql_task_t wz_link_check_ref = NULL;
ql_sem_t wifi_connect_sem;



void wz_socket_wifi_test(void *argv) {

	struct netif * netif = NULL;
	char ip[16];	
	//ip_addr_t ip_t , mask , gw ;
    ql_rtos_task_sleep_s(1);

    wz_log("wz_socket_test start\n");
    uint8_t mac[6] = W5500_MAC_1;

    if(wz_ethnetif_register(mac, W5500_IP, W5500_MASK, W5500_GW, WZ_ETHNETIF_NODE))
    {
        wz_log("W5500 NIC FAILLLLLLL");
        goto exit;
    }
    else
    {
        wz_log("W5500 NIC OKKKKKKKKK \n");
		ql_rtos_task_create(&wz_link_check_ref, 1024*8, 4, "net_link_check", net_link_check, NULL);	
		ql_rtos_task_sleep_ms(1000);

		while(net_wire_link_state == PHYCFGR_LNK_OFF)
		{
			ql_rtos_task_sleep_ms(100);
		}
		wz_log(" net wire inserted already!!!\n");
		if(sta_ip_is_start())
			ql_wlan_stop(QL_STATION);

        wz_ethnetif_set_dns("8.8.8.8", "114.114.114.114");
		//wz_ethnetif_set_dns("10.33.192.251", "114.114.114.114");
    #if USE_DHCP
        wz_ethnetif_dhcp();
    #else
        wz_ethnetif_set_up();
    #endif
    }

	netif = netif_find("wz");
	if(netif)
	{
		os_strcpy(ip, ipaddr_ntoa(&netif->ip_addr));
		wz_log("********find netif ip:%s\n", ip );
	}

   	while(net_wire_link_state == 1)
	{
		do_tcp_client();//正常不会退出
		net_link_status_change_flag=1;
		ql_rtos_task_sleep_ms(5000);
		while(net_wire_link_state != 1);//一直等待网线重新连接
	}

exit:
	ql_rtos_task_delete(wz_socket_wifi_test_ref);
}

void do_tcp_client(void)
{
	int				ret			= 0;
    int				sock_nbio	= 1;
	int				sock_fd     = -1;
	int				sock_error  = 0;
	socklen_t		optlen = 0;
	fd_set 			read_fds, write_fds;
	struct timeval	t;
    struct addrinfo		* res, hints;
	struct sockaddr_in	* ip4_svr_addr;
    int dns_success = 0;
	unsigned char recv_buf[1024] = {0};
	// struct netif * netif = NULL;
	// char ip[16];

	memset(&hints, 0, sizeof(struct addrinfo));
	//return;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(TCP_SERVER_DOMAIN, NULL, &hints, &res) != 0)
	{
		wz_log("*** DNS fail ***\r\n");
		goto exit;
	}

	dns_success = 1;
	ret = socket(AF_INET, SOCK_STREAM, 0);
	if(ret < 0)
	{
		wz_log("*** socket create fail ***\r\n");
		goto exit;
	}

	sock_fd = ret;

    ioctlsocket(sock_fd, FIONBIO, &sock_nbio);

	ip4_svr_addr = (struct sockaddr_in *)res->ai_addr;
	ip4_svr_addr->sin_port = htons(TCP_SERVER_PORT);

	ret = connect(sock_fd, (struct sockaddr *)ip4_svr_addr, sizeof(struct sockaddr));

	wz_log("connect ret: %d, errno: %u\r\n", ret, errno);

	if(ret == -1 && errno != EINPROGRESS)
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	t.tv_sec = TCP_CONNECT_TIMEOUT_S;
	t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_ZERO(&write_fds);

	FD_SET(sock_fd, &read_fds);
	FD_SET(sock_fd, &write_fds);

	ret = select(sock_fd + 1, &read_fds, &write_fds, NULL, &t);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(!FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else if(FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		optlen = sizeof(sock_error);
		ret = getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &sock_error, &optlen);
		if(ret == 0 && sock_error == 0)
		{
			wz_log("connect success\r\n");
			net_usable_flag=1;
		}
		else
		{
			wz_log("*** connect fail, sock_err = %d, errno = %u ***\r\n", sock_error, errno);
			goto exit;
		}
	}
	else if(!FD_ISSET(sock_fd, &read_fds) && FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("connect success\r\n");
	}
	else if(FD_ISSET(sock_fd, &read_fds) && !FD_ISSET(sock_fd, &write_fds))
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}
	else
	{
		wz_log("*** connect fail ***\r\n");
		goto exit;
	}

	ret = send(sock_fd, (const void*)TCP_CLIENT_SEND_STR, strlen(TCP_CLIENT_SEND_STR), 0);
	if(ret < 0)
	{
		wz_log("*** send fail ***\r\n");
		goto exit;
	}

_recv_:

	//t.tv_sec = TCP_RECV_TIMEOUT_S;
	// t.tv_sec = 30;
	// t.tv_usec = 0;

	FD_ZERO(&read_fds);
	FD_SET(sock_fd, &read_fds);

	ret = select(sock_fd + 1, &read_fds, NULL, NULL, NULL);

	wz_log("select ret: %d\r\n", ret);

	if(ret <= 0)
	{
		wz_log("*** select timeout or error ***\r\n");
		//return;
		goto exit;
	}

	if(FD_ISSET(sock_fd, &read_fds))
	{
		ret = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
		if(ret > 0)
		{
			wz_log("recv data: [%d]%s\r\n", ret, recv_buf);
			memset(recv_buf,0,1024);
            goto _recv_;
		}
		else if(ret == 0)
		{
			wz_log("*** peer closed ***\r\n");
			goto exit;
		}
		else
		{
			if(!(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN))
			{
				wz_log("*** error occurs ***\r\n");
				goto exit;
			}
			else
			{
				wz_log("wait for a while\r\n");
				goto _recv_;
			}
		}
	}

exit:
    if(dns_success) freeaddrinfo(res);
	
	if(sock_fd >= 0)
	{
		struct linger linger = {0};

		linger.l_onoff = 1;
		linger.l_linger = TCP_CLOSE_LINGER_TIME_S;

		setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger));
      //  setsockopt(sock_fd, IPPROTO_TCP, TCP_CLOSE_TIMEROUT, &linger.l_linger, sizeof(linger.l_linger));
		
		close(sock_fd);
	}
	net_usable_flag =0;
	net_wire_link_state=0;
	ql_rtos_semaphore_release(wifi_connect_sem);

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
	wz_log("event","s:%d,r:%d", t_reason.state, t_reason.reason_code);
	switch (event_id) 
	{
		case EVENT_WIFI_AP_CONNECTED:
			ap_connected = (wifi_event_ap_connected_t *)event_data;
			wz_log(BK_MAC_FORMAT" connected to AP\n", BK_MAC_STR(ap_connected->mac));
			break;

		case EVENT_WIFI_AP_DISCONNECTED:
			ap_disconnected = (wifi_event_ap_disconnected_t *)event_data;
			wz_log(BK_MAC_FORMAT" disconnected from AP\n", BK_MAC_STR(ap_disconnected->mac));
			break;

		case EVENT_WIFI_STA_CONNECTED:
			sta_connected = (wifi_event_sta_connected_t *)event_data;
			wifi_satate =1;
			wz_log("STA connected to %s\n", sta_connected->ssid);
			break;

		case EVENT_WIFI_STA_DISCONNECTED:
			sta_disconnected = (wifi_event_sta_disconnected_t *)event_data;
			wifi_satate =0;
			wz_log("STA disconnected, reason(%d)\n", sta_disconnected->disconnect_reason);
			break;

		default:
			wz_log("rx event <%d %d>\n", event_module, event_id);
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
		wz_log("%s got ip\n", got_ip->netif_if == NETIF_IF_STA ? "STA" : "unknown netif");
		wifi_satate =2;
		break;
	default:
		wz_log("rx event <%d %d>\n", event_module, event_id);
		break;
	}

	return BK_OK;
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
        wz_log("ssid name more than 32 Bytes\r\n");
        return;
    }

	os_strcpy((char *)wNetConfig.wifi_ssid, oob_ssid);
	os_strcpy((char *)wNetConfig.wifi_key, connect_key);

	wNetConfig.wifi_mode = QL_STATION;
	wNetConfig.dhcp_mode = DHCP_CLIENT;
	wNetConfig.wifi_retry_interval = 100;

	wz_log("ssid:%s key:%s\r\n", wNetConfig.wifi_ssid, wNetConfig.wifi_key);
	ql_wlan_status_register_cb(event_mod ,ql_wifi_event_cb);
	event_mod.event_id = EVENT_WIFI_STA_DISCONNECTED;
	ql_wlan_status_register_cb(event_mod ,ql_netif_event_cb);
	ql_wlan_start(&wNetConfig);
}

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x "

void wifi_connect_task(void *argv) {
	ql_link_status_s linkStatus;
	ql_ip_status_s ipStatus;
	char ssid[33] = {0};

	wz_log("***********wifi connect task start \n");
	while(1){
		ql_rtos_semaphore_wait(wifi_connect_sem, BEKEN_NEVER_TIMEOUT);
		ql_demo_sta_app_init("qwifi_test", "9876543210");
		ql_rtos_task_sleep_s(20);
		if (sta_ip_is_start()) {
			os_memset(&linkStatus, 0x0, sizeof(LinkStatusTypeDef));
			ql_wlan_get_link_status(&linkStatus);
			os_memcpy(ssid, linkStatus.ssid, 32);
			wz_log("sta:rssi=%d,ssid=%s,bssid=" MACSTR ",channel=%d,cipher_type:",
					linkStatus.wifi_strength, ssid, MAC2STR(linkStatus.bssid), linkStatus.channel);

			/*set struct empty*/
			os_memset(&ipStatus, 0x0, sizeof(IPStatusTypedef));
			/*get net status，and save in struct*/
			ql_wlan_get_ip_status(&ipStatus, QL_STATION);

			/*orintf net status*/
			wz_log("dhcp=%d ip=%s gate=%s mask=%s mac=" MACSTR "\r\n",
						ipStatus.dhcp, ipStatus.ip, ipStatus.gate,
						ipStatus.mask, MAC2STR((unsigned char*)ipStatus.mac));
			
			ping_start("www.baidu.com", 4, 0);
		}
	}
}

void net_link_check(void *argv)
{
	uint8_t times=0;
	uint8_t wifi_connect_resume_flag=0;
	
	wz_log("***********net link check task start \n");
	wz_log("please check net wire status first!!!\n");
	while(1){
			if(net_usable_flag == 0&& wifi_connect_resume_flag ==0)
			{
				ql_rtos_semaphore_release(wifi_connect_sem);
				wifi_connect_resume_flag=1;
			}

			if(net_usable_flag == 0 && net_wire_link_state ==0){			
				while(((wizphy_getphylink()) & PHYCFGR_LNK_ON) == PHYCFGR_LNK_OFF)
				{
					if (net_link_status_change_flag == 0 )
					{
						net_link_status_change_flag = 1;
						wz_log(" net wire not checked, pleack check net wire link status!!!\n");
					}
					ql_rtos_task_sleep_ms(1000);
				}
				net_wire_link_state =1;
				net_link_status_change_flag=0;
			}
	
			times++;
			ql_rtos_task_sleep_ms(1000);			
	}
}

void w5500_ethernet_test(void) {
	os_printf("***********w5500 demo start \n");
	ql_rtos_task_sleep_ms(10);
#if (WZ_SOCKET_TEST == 1)
    ql_rtos_task_create(&wz_socket_test_ref, 10240, 5, "wz_socket_test", wz_socket_test, NULL);
#else
	 QL_LOG_ON_ERR_VOID(ql_rtos_semaphore_create(&wifi_connect_sem, 1));
	 ql_rtos_task_create(&wz_socket_wifi_test_ref, 2048*10, 6, "wz_socket_test", wz_socket_wifi_test, NULL);
	 ql_rtos_task_create(&wz_socket_wifi_connect_ref, 1024*8, 5, "wifi_connect_task", wifi_connect_task, NULL);
#endif
}
