/**
 * @file ql_demo_cli.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<quec_common/ql_defines.h>
#include<quec_gpio/ql_gpio.h>
#include<quec_adc/ql_adc.h>
#include<quec_wlan/ql_wlan.h>
#include<quec_uart/ql_uart.h>
#include<quec_pwm/ql_pwm.h>
#include<quec_i2c/ql_i2c.h>
#include<quec_timer/ql_timer.h>

#include "ql_demo.h"
#include "bk_private/bk_wifi_wrapper.h"
#include <bk_cli/cli.h>
#include <utf8/conv_utf8_pub.h>
#include <bk_cli/include/bk_private/bk_cli.h>

#define TAG "ql_demo_cli"

#define EEPROM_DEV_ADDR          0x50
#define EEPROM_MEM_ADDR          0x10
#define I2C_SLAVE_ADDR           0x73
#define I2C_WRITE_WAIT_MAX_MS    (500)
#define I2C_READ_WAIT_MAX_MS     (500)
#define CAMERA_DEV_ADDR          (0x21)

static void cli_gpio_help(void)
{
	CLI_LOGI("ql_gpio [direct] [gpio_num] [gpio_mode] [gpio_value]\r\n");
	CLI_LOGI("ql_gpio [intr] [gpio_num] [gpio_mode]\r\n");
}

static void cli_adc_help(void)
{
	CLI_LOGI("ql_adc [init/deinit] [chan] [sample_rate] [adc_mode]\r\n");
	CLI_LOGI("ql_adc [start/stop] [chan]\r\n");
	CLI_LOGI("ql_adc [read] [chan] [raw] [size]\r\n");
}

static void cli_uart_help(void)
{
	CLI_LOGI("ql_uart [set] {uart_id} {baudrate} {data_bit} {parity_bit} {stop_bit} {flow_ctrl}\n");
	CLI_LOGI("ql_uart [recv] {uart_id} {len} {timeout}\n");
	CLI_LOGI("ql_uart [get] {uart_id}\n");
}

static void cli_pwm_help(void)
{
	CLI_LOGI("ql_pwm [chan] init [period_v] [duty_v] [duty2_v] [duty3_v]\n");
	CLI_LOGI("ql_pwm [chan] {start|stop|deinit}\n");
	CLI_LOGI("ql_pwm [chan] signal {low|high}\n");
	CLI_LOGI("ql_pwm [chan] duty [period_v] [duty_v] [duty2_v] [duty3_v]\n");
}

static void cli_i2c_help(void)
{
	CLI_LOGI("ql_i2c iic_id {init|deinit|memory_write|memory_read} [...]\r\n");
}

void ql_cli_wifi_scan_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc < 2) {
		demo_scan_app_init();
	} else {
		uint8_t *ap_ssid;

		ap_ssid = (uint8_t *)argv[1];
		demo_scan_adv_app_init(ap_ssid);
	}

	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

void ql_cli_wifi_ap_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ap_ssid = NULL;
	char *ap_key = "";
	char *ap_channel = NULL;
	int ret = 0;
	char *msg = NULL;

#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
	if (wifi_cli_find_id(argc, argv, "-w") > 0 ||
		wifi_cli_find_id(argc, argv, "-e") > 0) {
		if (argc == 3)
			ap_ssid = argv[2];
		else if (argc == 4) {
			ap_ssid = argv[2];
			if (os_strlen(argv[3]) <= 2)
				ap_channel = argv[3];
			else
				ap_key = argv[3];
		} else if (argc == 5) {
			ap_ssid = argv[2];
			ap_key = argv[3];
			ap_channel = argv[4];
		}
	} else {
#endif
		if (argc == 3) {
			if(0 == strcmp("-s", argv[1]) || 0 == strcmp("-ssid", argv[1])){
				ap_ssid = argv[2];
			}else if(0 == strcmp("-p", argv[1]) || 0 == strcmp("-password", argv[1])){
				ap_key = argv[2];
			}else if(0 == strcmp("-n", argv[1]) || 0 == strcmp("-channel", argv[1])){
				ap_channel = argv[2];
			}
		} else if (argc == 7) {
			if(0 == strcmp("-s", argv[1]) || 0 == strcmp("-ssid", argv[1])){
				ap_ssid = argv[2];
			}

			if(0 == strcmp("-p", argv[3]) || 0 == strcmp("-password", argv[3]))
			{
				ap_key = argv[4];
			}
			if(0 == strcmp("-n", argv[5]) || 0 == strcmp("-channel", argv[5])){
				ap_channel = argv[6];
			}
		}else if(5 == argc){
			if(0 == strcmp("-s", argv[1]) || 0 == strcmp("-ssid", argv[1])){
				ap_ssid = argv[2];
			}
			if(0 == strcmp("-p", argv[3]) || 0 == strcmp("-password", argv[3]))
			{
				ap_key = argv[4];
			}
		}
#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
	}
#endif

	if (ap_ssid) {
		ret = demo_softap_app_init(ap_ssid, ap_key, ap_channel);
#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
		bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
								fast_connect_cb);
		if (wifi_cli_find_id(argc, argv, "-w") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if ((info_t.flag & 0xf0l) == 0x70l)
				info_t.flag |= 0x2l;
			else
				info_t.flag = 0x72l;
			os_strcpy((char *)info_t.ap_ssid, (char *)ap_ssid);
			os_strcpy((char *)info_t.ap_pwd, ap_key);
			fast_connect_cb(NULL, 0, 0, NULL);
		} else if (wifi_cli_find_id(argc, argv, "-e") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if (info_t.flag == 0x72l || info_t.flag == 0x73l) {
				info_t.flag &= ~0x2l;
				bk_flash_set_protect_type(FLASH_PROTECT_NONE);
				bk_flash_erase_sector(pt->partition_start_addr + pt->partition_length -4096);
				bk_flash_write_bytes(pt->partition_start_addr + pt->partition_length -4096,
									(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
				bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			}
		}
#endif
	}
	if(ret == 0)
		msg = WIFI_CMD_RSP_SUCCEED;
	else
		msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#define CMD_WLAN_MAX_BSS_CNT	50
beken_semaphore_t ql_wifi_cmd_sema = NULL;
int ql_wifi_cmd_status = 0;

void ql_cli_wifi_sta_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	char *ssid = NULL;
	char *password = "";
	char *msg = NULL;

	if ((argc < 2) || (argc > 6)) {
		CLI_LOGI("invalid argc number\n");
		goto error;
	}

	int err = rtos_init_semaphore(&ql_wifi_cmd_sema, 1);
	if (err != kNoErr) {
		goto error;
	}

#ifdef CONFIG_BSSID_CONNECT
	uint8_t bssid[6] = {0};
	if (os_strcmp(argv[1], "bssid") == 0) {
		if(argc >= 3) {
			hexstr2bin(argv[2], bssid, 6);
		}
		if(argc >= 4) {
			password = argv[3];
		}
		demo_sta_bssid_app_init(bssid, password);
		if (ql_wifi_cmd_sema != NULL)
		{
			err = rtos_get_semaphore(&ql_wifi_cmd_sema, 10000);
			if (err != kNoErr)
			{
				goto error;
			}
			else
			{
				if (ql_wifi_cmd_status == 1)
				{
					msg = WIFI_EVT_GOT_IP;
					os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
					rtos_deinit_semaphore(&ql_wifi_cmd_sema);
					ql_wifi_cmd_status = 0;
					return;
				}
				else
				{
					goto error;
				}
			}
		}

	}
#endif

#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
	if (wifi_cli_find_id(argc, argv, "-w") > 0 ||
		wifi_cli_find_id(argc, argv, "-e") > 0) {
		if (argc >= 2)
			ssid = argv[2];

		if (argc >= 3)
			password = argv[3];
	} else {
#endif
		if (argc >= 2)
			ssid = argv[1];

		if (argc >= 3)
			password = argv[2];
#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
	}
#endif

#if 0 //TODO support BSSID/Channel configuration
	if (argc >= 4)
		bssid = argv[3];

	if (argc >= 5)
		channel = argv[4];
#endif

	char *oob_ssid_tp = ssid;
#if CONFIG_USE_CONV_UTF8
	oob_ssid_tp = (char *)conv_utf8((uint8_t *)ssid);
#endif

	if (oob_ssid_tp) {
		demo_sta_app_init((char *)oob_ssid_tp, password);

#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
		if (wifi_cli_find_id(argc, argv, "-w") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);
			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if ((info_t.flag & 0xf0l) == 0x70l)
				info_t.flag |= 0x1l;
			else
				info_t.flag = 0x71l;
			os_strcpy((char *)info_t.sta_ssid, (char *)oob_ssid_tp);
			os_strcpy((char *)info_t.sta_pwd, password);
			bk_event_register_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
									fast_connect_cb, &info_t);
		} else if (wifi_cli_find_id(argc, argv, "-e") > 0) {
			bk_logic_partition_t *pt = bk_flash_partition_get_info(BK_PARTITION_USR_CONFIG);

			bk_flash_read_bytes(pt->partition_start_addr + pt->partition_length -4096,
						(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
			if (info_t.flag == 0x71l || info_t.flag == 0x73l) {
				info_t.flag &= ~0x1l;
				bk_flash_set_protect_type(FLASH_PROTECT_NONE);
				bk_flash_write_bytes(pt->partition_start_addr + pt->partition_length -4096,
									(uint8_t *)&info_t, sizeof(BK_FAST_CONNECT_T));
				bk_flash_set_protect_type(FLASH_UNPROTECT_LAST_BLOCK);
			}
		}
#endif
#if CONFIG_USE_CONV_UTF8
		os_free(oob_ssid_tp);
#endif
		if (ql_wifi_cmd_sema != NULL)
		{
			err = rtos_get_semaphore(&ql_wifi_cmd_sema, 10000);
			if (err != kNoErr)
			{
				goto error;
			}
			else
			{
				if (ql_wifi_cmd_status == 1)
				{
					msg = WIFI_EVT_GOT_IP;
					os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
					rtos_deinit_semaphore(&ql_wifi_cmd_sema);
					ql_wifi_cmd_status = 0;
					return;
				}
				else
				{
					goto error;
				}
			}
		}
	} else {
		CLI_LOGI("not buf for utf8\r\n");
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	if (ql_wifi_cmd_sema != NULL)
		rtos_deinit_semaphore(&ql_wifi_cmd_sema);
	return;
}

void ql_cli_wifi_stop_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	int ret = 0;
	char *msg = NULL;

	if (argc == 2) {
		if (os_strcmp(argv[1], "sta") == 0) {
#if CONFIG_ENABLE_WIFI_DEFAULT_CONNECT
			bk_event_unregister_cb(EVENT_MOD_WIFI, EVENT_WIFI_STA_CONNECTED,
									fast_connect_cb);
#endif
			ret = bk_wifi_sta_stop();
		} else if (os_strcmp(argv[1], "softap") == 0)
			ret = bk_wifi_ap_stop();
		else {
			CLI_LOGI("unknown WiFi interface\n");
			goto error;
		}
	} else {
		CLI_LOGI("bad parameters\r\n");
		goto error;
	}

	if (!ret) {
		msg = WIFI_CMD_RSP_SUCCEED;
		os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
		return;
	}
	else {
		goto error;
	}

error:
	msg = WIFI_CMD_RSP_ERROR;
	os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
	return;
}


static void cli_gpio_int_isr(gpio_id_t id)
{
	CLI_LOGI("gpio isr index:%d\n",id);
}
void ql_cli_gpio_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t id;
	gpio_config_t cfg;
	ql_gpio_level_e level = QL_GPIO_LEVEL_HIGH;

	if (argc < 2) {
		cli_gpio_help();
		return;
	}

	if(os_strcmp("direct", argv[1]) == 0){
		id = os_strtoul(argv[2], NULL, 10);
		gpio_dev_unmap(id);
		if(os_strcmp("1", argv[3]) == 0){ //output mode
			if(os_strcmp("0", argv[4]) == 0){
				level = QL_GPIO_LEVEL_LOW;
			}else if(os_strcmp("1",argv[4]) == 0){
				level = QL_GPIO_LEVEL_HIGH;
			}
			ql_gpio_init(id, QL_GMODE_OUTPUT, level);
		}else if(os_strcmp("0", argv[3]) == 0){ //Input mode
			cfg.io_mode =GPIO_INPUT_ENABLE;
			if(os_strcmp("0", argv[4]) == 0){
				cfg.pull_mode = GPIO_PULL_DOWN_EN;
			}else{
				cfg.pull_mode = GPIO_PULL_UP_EN;
			}
			bk_gpio_set_config(id, &cfg);
		}
	}else if(os_strcmp("intr", argv[1]) == 0){
		id = os_strtoul(argv[2], NULL, 10);
		ql_gpio_interrupt_init(id, os_strtoul(argv[3], NULL, 10), cli_gpio_int_isr);
		ql_gpio_interrupt_enable(id, true);
	}else {
		cli_gpio_help();
		return;
	}
}


void ql_cli_adc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t adc_chan;
	uint16_t adc_value	= 0;
	if (argc < 2) {
		cli_adc_help();
		return;
	}
	adc_chan = os_strtoul(argv[2], NULL, 10);
	if (os_strcmp(argv[1], "init") == 0) {
		uint32_t sample_rate = os_strtoul(argv[3], NULL, 10);
		uint8_t adc_mode = os_strtoul(argv[4], NULL, 10);
		ql_adc_init(adc_chan, sample_rate, adc_mode);
		CLI_LOGI("adc init:\n adc_chan=%d, sample_rate=%d, adc_mode=%d\r\n", adc_chan, sample_rate, adc_mode);
	}
	else if(os_strcmp(argv[1], "deinit") == 0){
		ql_adc_deinit(adc_chan);
		CLI_LOGI("adc deinit\r\n");
	}
	else if(os_strcmp(argv[1], "start") == 0){
		ql_adc_start(adc_chan);
		CLI_LOGI("adc start\r\n");
	}
	else if(os_strcmp(argv[1], "stop") == 0){
		ql_adc_stop(adc_chan);
		CLI_LOGI("adc stop\r\n");
	}
	else if(os_strcmp(argv[1], "read") == 0){
		adc_value = 0;
		ql_adc_read(adc_chan, &adc_value, 500);
        CLI_LOGI("adc,chan=%d, val=%d\n", adc_value);
	}
	else{
		cli_adc_help();
		return;
	}
}

void ql_cli_uart_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t uart_id;

	if (argc < 2) {
		cli_uart_help();
		return;
	}

	uart_id = os_strtoul(argv[2], NULL, 10);

	if (os_strcmp(argv[1], "set") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		ql_uart_config_s config = {0};
		os_memset(&config, 0, sizeof(ql_uart_config_s));
		config.baudrate = os_strtoul(argv[3], NULL, 10);
		config.data_bit = os_strtoul(argv[4], NULL, 10);
		config.parity_bit = os_strtoul(argv[5], NULL, 10);
		config.stop_bit = os_strtoul(argv[6], NULL, 10);
		if (argc > 7) {
			config.flow_ctrl = os_strtoul(argv[7], NULL, 10);
		}
		ql_uart_set_dcbconfig(uart_id, &config);
		ql_uart_open(uart_id);
		CLI_LOGI("ql uart init, uart_id=%d\n", uart_id);
	}else if (os_strcmp(argv[1], "get") == 0) {
		ql_uart_config_s uart_get_cfg = {0};
		os_memset(&uart_get_cfg, 0, sizeof(ql_uart_config_s));
		ql_uart_get_dcbconfig(uart_id, &uart_get_cfg);
		CLI_LOGI("ql uart, get config:\r\n");
		CLI_LOGI("ql uart, baudrate:%d\r\n",     uart_get_cfg.baudrate);
		CLI_LOGI("ql uart, data_bit:%d\r\n",     uart_get_cfg.data_bit);
		CLI_LOGI("ql uart, stop_bit:%d\r\n",     uart_get_cfg.stop_bit);
		CLI_LOGI("ql uart, parity_bit:%d\r\n",   uart_get_cfg.parity_bit);
		CLI_LOGI("ql uart, flow_ctrl:%d\r\n",    uart_get_cfg.flow_ctrl);
		CLI_LOGI("\r\n");
	} else if (os_strcmp(argv[1], "recv") == 0) {
		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *recv_data = (uint8_t *)os_malloc(buf_len);
		if (recv_data == NULL) {
			CLI_LOGE("recv buffer malloc failed\r\n");
			return;
		}
		int time_out = os_strtoul(argv[4], NULL, 10);
		if (time_out < 0) {
			time_out = BEKEN_WAIT_FOREVER;
		}
		int data_len = bk_uart_read_bytes(uart_id, recv_data, buf_len, time_out);
		if (data_len < 0) {
			CLI_LOGE("ql uart read failed, ret:-0x%x\r\n", -data_len);
			goto exit;
		}
		CLI_LOGI("ql uart read, ql uart_id=%d, time_out:%x data_len:%d\n", uart_id, time_out, data_len);
		for (int i = 0; i < data_len; i++) {
			CLI_LOGI("recv_buffer[%d]=0x%x\n", i, recv_data[i]);
		}
exit:
		if (recv_data) {
			os_free(recv_data);
		}
		recv_data = NULL;
	} else if (os_strcmp(argv[1], "write") == 0) {
		BK_LOG_ON_ERR(bk_uart_write_bytes(uart_id, argv[2], os_strlen(argv[2])));
		CLI_LOGI("ql uart write string, uart_id=%d, data_len:%d\n", uart_id, os_strlen(argv[2]));
	}
	else {
		cli_uart_help();
		return;
	}
}

void ql_cli_pwm_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	uint32_t chan;

	if (argc < 2) {
		cli_pwm_help();
		return;
	}

	chan = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		CLI_RET_ON_INVALID_ARGC(argc, 5);
		pwm_init_config_t config = {0};

		config.period_cycle = os_strtoul(argv[3], NULL, 10);
		config.duty_cycle = os_strtoul(argv[4], NULL, 10);
		if (argc > 5)
			config.duty2_cycle = os_strtoul(argv[5], NULL, 10);
		if (argc > 6)
			config.duty3_cycle = os_strtoul(argv[6], NULL, 10);

		BK_LOG_ON_ERR(bk_pwm_init(chan, &config));
		CLI_LOGI("pwm init, chan=%d period=%x duty=%x\n", chan, config.period_cycle, config.duty_cycle);
	} else if (os_strcmp(argv[2], "start") == 0) {
		BK_LOG_ON_ERR(bk_pwm_start(chan));
		CLI_LOGI("pwm start, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "stop") == 0) {
		BK_LOG_ON_ERR(bk_pwm_stop(chan));
		CLI_LOGI("pwm stop, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(bk_pwm_deinit(chan));
		CLI_LOGI("pwm deinit, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "signal") == 0) {
		if (argc != 4) {
			cli_pwm_help();
			return;
		}

		if (os_strcmp(argv[3], "low") == 0)
			BK_LOG_ON_ERR(bk_pwm_set_init_signal_low(chan));
		else
			BK_LOG_ON_ERR(bk_pwm_set_init_signal_high(chan));
		CLI_LOGI("pwm set signal, chan=%d\n", chan);
	} else if (os_strcmp(argv[2], "duty") == 0) {
		pwm_period_duty_config_t config = {0};
		if (argc < 5) {
			cli_pwm_help();
			return;
		}

		config.period_cycle = os_strtoul(argv[3], NULL, 10);
		config.duty_cycle = os_strtoul(argv[4], NULL, 10);
		if (argc > 5)
			config.duty2_cycle = os_strtoul(argv[5], NULL, 10);
		if (argc > 6)
			config.duty3_cycle = os_strtoul(argv[6], NULL, 10);
		BK_LOG_ON_ERR(bk_pwm_set_period_duty(chan, &config));
		CLI_LOGI("pwm duty, chan=%d period=%x t1=%x t2=%x t3=%x\n", chan, config.period_cycle,
				 config.duty_cycle, config.duty2_cycle, config.duty3_cycle);
	} else {
		cli_pwm_help();
		return;
	}
}

void ql_cli_i2c_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
	if (argc < 2) {
		cli_i2c_help();
		return;
	}

	uint32_t i2c_id = os_strtoul(argv[1], NULL, 10);

	if (os_strcmp(argv[2], "init") == 0) {
		if(argc < 5){
			CLI_LOGI("please params error!!!!\r\n");
			return;
		}

		uint8_t speed_mode = os_strtoul(argv[3], NULL, 10);
		uint8_t addr_mode =  os_strtoul(argv[4], NULL, 10);
		uint16_t dev_addr =  (uint16_t)os_strtoul(argv[5], NULL, 16);
		ql_i2c_init(i2c_id, speed_mode, addr_mode, dev_addr);
		CLI_LOGI("i2c(%d) init:\n speed_mode=%d, addr_mode=%d, dev_addr=%04X\r\n", i2c_id, speed_mode, addr_mode, dev_addr);

	} else if (os_strcmp(argv[2], "deinit") == 0) {
		BK_LOG_ON_ERR(ql_i2c_deinit(i2c_id));
		CLI_LOGI("i2c(%d) deinit\n", i2c_id);
	} else if (os_strcmp(argv[2], "master_write") == 0) {
		uint8_t data_buf[10] = {0};
		for (uint32_t i = 0; i < 10; i++) {
			data_buf[i] = i & 0xff;
		}
		BK_LOG_ON_ERR(bk_i2c_master_write(i2c_id, EEPROM_DEV_ADDR, data_buf, 10, I2C_WRITE_WAIT_MAX_MS));
	} else if (os_strcmp(argv[2], "memory_write") == 0) {

		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_malloc(buf_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("malloc fail\r\n");
			return;
		}

		for (uint32_t i = 0; i < buf_len; i++) {
			data_buf[i] = (i + 1) & 0xff;
		}

		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		uint32_t mem_addr = os_strtoul(argv[5], NULL, 16);
		uint8_t mem_size =  os_strtoul(argv[6], NULL, 10);

		BK_LOG_ON_ERR(ql_i2c_master_mem_write(i2c_id, dev_addr, mem_addr, mem_size, data_buf, buf_len));
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}

		CLI_LOGI("i2c(%d) memory_write buf_len:%d\r\n", i2c_id, buf_len);
	} else if (os_strcmp(argv[2], "memory_read") == 0) {

		uint32_t buf_len = os_strtoul(argv[3], NULL, 10);
		uint8_t *data_buf = os_zalloc(buf_len);
		if(NULL == data_buf)
		{
			CLI_LOGE("os_zalloc fail\r\n");
			return;
		}

		uint32_t dev_addr = os_strtoul(argv[4], NULL, 16);
		uint32_t mem_addr = os_strtoul(argv[5], NULL, 16);
		uint8_t mem_size =  os_strtoul(argv[6], NULL, 10);

		BK_LOG_ON_ERR(ql_i2c_master_mem_read(i2c_id, dev_addr, mem_addr, mem_size, data_buf, buf_len));
		for (uint32_t i = 0; i < buf_len; i++) {
			CLI_LOGI("i2c_read_buf[%d]=%x\r\n", i, data_buf[i]);
		}
		if (data_buf) {
			os_free(data_buf);
			data_buf = NULL;
		}
		CLI_LOGI("i2c(%d) memory_read buf_len:%d\r\n", i2c_id, buf_len);
	} else {
		cli_i2c_help();
		return;
	}
}


static void cli_timer_isr(timer_id_t chan)
{
    CLI_LOGI("[TIMER][ISR] chan:%d\r\n", chan);
}

void ql_cli_timer_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    int ret = 0;
    uint8_t channel = 0;
    uint32_t time_ms = 0, read_cnt = 0;

    channel = os_strtoul(argv[1], NULL, 10);

    if (os_strcmp(argv[2], "start") == 0) {
        time_ms = os_strtoul(argv[3], NULL, 10);
        CLI_LOGI("[TIMER][START] channel:%d, time_ms:%d\r\n", channel, time_ms);
        ret = bk_timer_start(channel, time_ms, cli_timer_isr);
        if (ret != BK_OK) {
            CLI_LOGW("[TIMER][START] start failed, error code:%x\n", ret);
        }
    } else if (os_strcmp(argv[2], "stop") == 0) {
        CLI_LOGI("[TIMER][STOP] channel:%d\r\n", channel);
        bk_timer_stop(channel);
    } else if (os_strcmp(argv[2], "read") == 0) {
        read_cnt = bk_timer_get_cnt(channel);
        CLI_LOGI("[TIMER][READ] read cnt:%x\r\n", read_cnt);
    } else if (os_strcmp(argv[2], "enable") == 0) {
        bk_timer_enable(channel);
        CLI_LOGI("[TIMER][ENABLE] channel:%x\r\n", channel);
    } else if (os_strcmp(argv[2], "disable") == 0) {
        bk_timer_disable(channel);
        CLI_LOGI("[TIMER][DISABLE] channel:%x\r\n", channel);
    } else if (os_strcmp(argv[2], "get_period") == 0) {
        read_cnt = bk_timer_get_period(channel);
        CLI_LOGI("[TIMER][GET][PERIOD] period value:%x\r\n", read_cnt);
    } else {
        CLI_LOGW("ql_timer {timer_id} {start|stop}  [timer_period]\r\n");
    }
}

#define QL_CLI_CMD_CNT (sizeof(s_ql_demo_commands) / sizeof(struct cli_command))
static const struct cli_command s_ql_demo_commands[] = {
    {"ql_wifi_scan",    "ql_wifi_scan [ssid]", ql_cli_wifi_scan_cmd},
    {"ql_wifi_softap",  "ql_wifi_softap {-s|-ssid} [-p|-password] [-n|-channel[1:14]]", ql_cli_wifi_ap_cmd},
    {"ql_wifi_sta",     "ql_wifi_sta ssid [password] [bssid] [channel]", ql_cli_wifi_sta_cmd},
    {"ql_wifi_stop",    "ql_wifi_stop {sta | softap}", ql_cli_wifi_stop_cmd},
    {"ql_gpio",         "ql_gpio {direct|intr} gpio_num gpio_mode [gpio_value]", ql_cli_gpio_cmd},
    {"ql_adc",          "ql_adc {init|deinit|start|stop|read} chan [sample_rate|raw] [adc_mode|size]", ql_cli_adc_cmd},
    {"ql_uart",         "ql_uart {set|recv|get} uart_id baudrate data_bit parity_bit stop_bit flow_ctrl", ql_cli_uart_cmd},
    {"ql_pwm",          "ql_pwm chan {start|stop|deinit|signal|duty} [low|high|period_v] [duty_v] [duty2_v] [duty3_v]", ql_cli_pwm_cmd},
    {"ql_i2c",          "ql_i2c i2c_id {init|deinit|memory_write|memory_read}", ql_cli_i2c_cmd},
    {"ql_timer",        "ql_timer timer_id {start|stop} [timer_period]", ql_cli_timer_cmd},
};

int ql_demo_cli_init(void)
{
#if CFG_ENABLE_QUECTEL_DEMO_CLIS
    return cli_register_commands(s_ql_demo_commands, QL_CLI_CMD_CNT);
#endif
}
