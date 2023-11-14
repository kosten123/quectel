
#include<quec_osi/ql_osi.h>
#include<quec_common/ql_defines.h>

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#include "lwip/prot/ethernet.h"
#include "lwip/etharp.h"
#include "lwip/ping.h"

#include<quec_wlan/ql_wlan.h>

#include "modules/pm.h"
#include "modules/wifi.h"
#include <driver/gpio.h>
#include <modules/ble.h>

#define wz_log	bk_printf

#define TCP_SERVER_DOMAIN "192.168.50.199"  //局域网可以用
#define TCP_SERVER_PORT 8000

// #define TCP_SERVER_DOMAIN "115.236.153.174"
// #define TCP_SERVER_PORT 40169

#define TCP_CONNECT_TIMEOUT_S 10
#define TCP_RECV_TIMEOUT_S 3
#define TCP_CLOSE_LINGER_TIME_S 10
#define TCP_CLIENT_SEND_STR "GET / HTTP/1.1\r\nHost: www.baidu.com:80\r\n\r\n"

void Enter_low_voltage_mode(void);
void Exit_low_voltage_mode(void);

void do_tcp_client(void *arg)
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
    wz_log("*** enter ***\r\n");
    do{
        wz_log("*** wait wifi connect ***\r\n");
        ql_rtos_task_sleep_s(5);
    }while(!sta_ip_is_start());

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
			if (strncmp((char*)recv_buf, "wakeup", strlen("wakeup")) == 0) {
                Exit_low_voltage_mode();
            }
			if (strncmp((char*)recv_buf, "sleep", strlen("sleep")) == 0) {
				memset(recv_buf,0,1024);
            	Enter_low_voltage_mode();
				goto exit;
            }
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

}

ql_task_t lowpwr_test_thread_handle = NULL;
void ql_lowpwr_test_demo_thread_create(void)
{
    int ret;
    ret = ql_rtos_task_create(&lowpwr_test_thread_handle,
                              (unsigned short)1024*8,
                              BEKEN_APPLICATION_PRIORITY,
                              "lowpower_test",
                              do_tcp_client,
                              0);

    if(ret != kNoErr){
        os_printf("Error: Failed to create test thread: %d\r\n", ret);
        goto init_err;
    }
    return;

init_err:
    if( lowpwr_test_thread_handle != NULL ){
        ql_rtos_task_delete(lowpwr_test_thread_handle);
    }
}


/*
pm [sleep_mode] [wake_source] [vote1] [vote2] [vote3] [param1] [param2] [param3]
sleep_mode：代表休眠模式(参数范围：0-3): 0:normal sleep; 1:low voltage; 2:deep sleep;3:none
wake_source：表示唤醒源（取值范围0-5）： 0：GPIO唤醒；1：RTC唤醒；2：WIFI或者BT唤醒；3：USB唤醒（BK7256不支持，其他项目支持）4：touch唤醒；5：none
Vote1: vote2: vote3: 代表投的票（取值范围8(BT)，9(WIFI)，12(APP)
param1：代表休眠时间;
param2：在RTC唤醒时没有用该参数，默认是0;
param3：BT休眠时间，该参数对于发送命令是兼容接口，建议设置的时间1000000000（100s）,设置这么大的值为了使用BT模块内部模块设置的时间为准。

param2：根据唤醒源有不同含义 -- GPIO：代表唤醒方式（0：低电平唤醒，1：高电平唤醒，2：上升沿唤醒，3：下降沿唤醒）  RTC和touch：唤醒时不用该参数，默认是0
*/

void cli_lowvoltage_gpio_callback(gpio_id_t gpio_id)
{

    bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0x0,0x0);
	os_printf("cli_lowvoltage_gpio_callback[%d]\r\n",bk_pm_exit_low_vol_wakeup_source_get());
}

void Enter_low_voltage_mode(void)
{
	os_printf("enter low power mode\r\n");
        //gpio wakeup lowvoltage
    bk_gpio_register_isr(GPIO_20, cli_lowvoltage_gpio_callback);
    bk_gpio_register_wakeup_source(GPIO_20,0);
    bk_pm_wakeup_source_set(PM_WAKEUP_SOURCE_INT_GPIO, NULL);

     //wifi wakeup lowvoltage
    bk_ble_deinit();
    bk_wifi_send_listen_interval_req(10);
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,1,0);
    bk_pm_sleep_mode_set(PM_MODE_LOW_VOLTAGE);
}

void Exit_low_voltage_mode(void)
{
    bk_pm_sleep_mode_set(PM_MODE_DEFAULT);
    bk_pm_module_vote_sleep_ctrl(PM_SLEEP_MODULE_NAME_APP,0,0);
	os_printf("exit low power mode\r\n");
}





