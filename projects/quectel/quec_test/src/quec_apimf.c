/**
 * @file quec_apimf.c
 * @author Jay Dong (Jay.Dong@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "quec_apimf_includes.h"
#include "quec_apimf_def.h"
#include "quec_apimf.h"

const char* const MODESTR[_MAX_MODE] = {
#define MODESTR_INFO
#include "quec_apimf_def.h"
};

#define FUNCDECLARE_INFO
#include "quec_apimf_def.h"

static const pQUECSTFUNC qlstFunction[_MAX_MODE] = {
#define FUNCNAME_INFO
#include "quec_apimf_def.h"
};

#define MAX_DATA_SEND 0x400
#define UART_TEST_PORT UART_ID_1

static ql_queue_t  quecst_receive_queue = NULL;
static ql_queue_t  quecst_send_queue = NULL;
volatile int function_id =0;//back function id to upper
char funcid_str[48];

void quecst_uart_write(char *para);

unsigned int quecst_pointer_str_to_address_num(char* str_pointer)
{
	int i = 0, j = 0;
	unsigned int address_num = 0;
	i = strlen(str_pointer);
	j = i;
	for (; i >= 0; i--)
	{
		int temp_num = 0;
		if (str_pointer[i] >= 'A' && str_pointer[i] <= 'Z')
			temp_num = (str_pointer[i] - 'A' + 10)<<(4*(j-i));
		else if (str_pointer[i] >= 'a' && str_pointer[i] <= 'z')
			temp_num = (str_pointer[i] - 'a' + 10)<<(4*(j-i));
		else 
			temp_num = (str_pointer[i] - '0')<<(4*(j-i));

		address_num = temp_num;
	}

	return address_num;
}

int quecst_special_data_process(char* msg)
{
	if (strstr(msg,"qlst_fota_firmware_download"))
		return 1;
	return 0;
}

void quecst_send_upper_data(const char *fmt, ...) // fmt:"%d=%d,%s,%s,..."
{
	int len = 0;
	int len_pos = 0, len_remain = 0;
	char *pbuf = NULL;
	char *s_pbuf = NULL;

	pbuf = malloc(MAX_DATA_SEND);
	memset (pbuf, 0, MAX_DATA_SEND);
	s_pbuf = malloc(MSG_MAX_SIZE);
	memset (s_pbuf,  0, MSG_MAX_SIZE);

	va_list args;
	va_start(args, fmt);
	len = vsnprintf(pbuf, MAX_DATA_SEND, fmt, args);
	va_end(args);

	if (quecst_special_data_process(pbuf)){
		quecst_uart_write(pbuf);
		return;
	}

	if (len <= 0)
		quecst_printf(SYNC_LOG,  "quecst_send_upper_data failed!\r\n");
	else{
		len_remain = len;
		while (len_remain > MSG_MAX_SIZE)
		{			
			memcpy(s_pbuf, pbuf+len_pos, MSG_MAX_SIZE);			
			ql_rtos_queue_release(quecst_send_queue, MSG_MAX_SIZE, (u8 *)s_pbuf, QL_NO_WAIT);

			len_pos += MSG_MAX_SIZE;
			len_remain -= MSG_MAX_SIZE;
			memset(s_pbuf, 0, MSG_MAX_SIZE);
		}

		memcpy(s_pbuf, pbuf+(len-len_remain), len_remain);
		ql_rtos_queue_release(quecst_send_queue, len_remain, (u8 *)s_pbuf, QL_NO_WAIT);
	}

	free(pbuf);
	free(s_pbuf);
	return;
}

void quecst_uart_write (char *para)
{
	if (para != NULL && strlen(para) > 0)
		ql_uart_write(UART_TEST_PORT,(unsigned char*)para,strlen(para));
	return;
}

void quecst_send_uart_data_task(void * argv)
{
	char msg[MSG_MAX_SIZE] = {0};
	quecst_printf(SYNC_LOG, "enter quecst_send_uart_data_task!\r\n");
	quecst_send_upper_data("enter quecst_send_uart_data_task! !");
	while (1){
		memset(msg, 0, sizeof(msg));
		ql_rtos_queue_wait(quecst_send_queue,(uint8_t)sizeof(msg),(u8*)msg ,0xFFFFFFFF);  // wait for the message from main task
		if (strlen(msg) > 0)
			quecst_uart_write(msg);
		else
			quecst_uart_write("send msg is NULL!");
	}	

	return;
}

int quecst_data_process (char* msg, char (*out_str)[PARAM_SIZE], char* funcid)
{
	char* pbuf = NULL;
	char* pbuf_bk = NULL;
	int str_len = 0;
	int param_num = 0;
	
	pbuf = strstr(msg, "=");
	if(pbuf != NULL){
		str_len = strlen(msg) - strlen(pbuf);
		memcpy(funcid, msg, str_len);
		
		//quecst_printf(SYNC_LOG, "****** funcid:%s ******\r\n", funcid);
		if (pbuf != NULL){
			for (; param_num < PARAM_COUNT; param_num++)
			{
				if(!strcmp(funcid, "75") && param_num == 1)//解决socket send 发送数据中带有',' 误认为参数进行分割的问题
				{
					strncpy(out_str[param_num], pbuf+1, strlen(pbuf)-1);
					param_num++;
					break;
				}
				
				pbuf_bk = pbuf+1;

				//quecst_printf(SYNC_LOG, "****** 1 pbuf_bk:%s pbuf:%s,%d %d len:%d ******\r\n", pbuf_bk, pbuf, *pbuf_bk, *(pbuf_bk+1),strlen(pbuf_bk));
				pbuf = strstr(pbuf_bk, ",");
				if (pbuf != NULL){
					str_len = strlen(pbuf_bk) - strlen(pbuf);
					strncpy(out_str[param_num], pbuf_bk, str_len);
					//quecst_printf(SYNC_LOG, "****** 1 param_str:%s ******\r\n", out_str[param_num]);
				}
				else if (strlen(pbuf_bk) > 0 && *pbuf_bk != CH_RC){
					if (*(pbuf_bk+strlen(pbuf_bk)-2) == CH_RC && *(pbuf_bk+strlen(pbuf_bk)-1) ==CH_LF)
						strncpy(out_str[param_num], pbuf_bk, strlen(pbuf_bk)-2);
					else
						strncpy(out_str[param_num], pbuf_bk, strlen(pbuf_bk));
					
					//quecst_printf(SYNC_LOG, "****** 2 param_str:%s ******\r\n", out_str[param_num]);
					param_num++;
					break;
				}
				else if (strstr(msg, ","))
				{
					param_num++;
					break;
				}
				else
				    break;
			}
		}
	}
	return param_num;
}

/**
 * @brief 
 * 
 * @param port 
 * @param para 
 */
void quecst_uart_rx_isr (int  port, void *para)
{
    uint16_t read_len = MSG_MAX_SIZE;
    unsigned char r_data[MSG_MAX_SIZE] = {0};

	ql_uart_read(port, r_data, &read_len);
	ql_rtos_queue_release(quecst_receive_queue, read_len, (u8 *)r_data, QL_NO_WAIT);
	ql_uart_set_rx_int(port, QL_DEV_ENABLE);
 }

/**
 * @brief 
 * 
 * @param argv 
 */
void quecst_receive_uart_data_task(void * argv)
{
	//char funcid_str[48];
	int param_num = 0;
	int funcid = 0;
	char param_str[PARAM_COUNT][PARAM_SIZE];
	char msg[MSG_MAX_SIZE] = {0};
	quecst_printf(SYNC_LOG, "enter quecst_receive_uart_data_task!\r\n");
	while (1){
		funcid = 0;
		memset(funcid_str,0,sizeof(funcid_str));
		memset(param_str,0,sizeof(param_str));
		//quecst_printf(SYNC_LOG, "****** 1 wait main uart data ******");
		ql_rtos_queue_wait(quecst_receive_queue, (uint8_t)sizeof(msg),(uint8*)msg, 0xFFFFFFFF);  // wait for the message from main task
		//quecst_printf(SYNC_LOG, "****** 2 receive uart data ******");
		param_num = quecst_data_process(msg, param_str, funcid_str);

		//quecst_printf(SYNC_LOG, "****** 3 param_num:%d ******\r\n", param_num);
		if (param_num == PARAM_COUNT)
			continue;
			
#if (FUNCTION_CALL_TYPE)
		funcid = atoi(funcid_str)-1;//数组下标从0开始	
		//quecst_printf(SYNC_LOG, "****** 4 funcid:%d ******\r\n", funcid); 	
		if((funcid >= 0) && (funcid < (_MAX_MODE-1)))
		{
			function_id = funcid+1;
			qlstFunction[funcid](param_num,(char**)param_str);
		}
#else
		for(;funcid < _MAX_MODE-1; funcid++)
		{
			if (!strcmp(MODESTR[funcid], funcid_str))
			{
				//quecst_printf(SYNC_LOG, "****** 5 param_num:%d ******\r\n", param_num);
				qlstFunction[funcid](param_num, (char**)param_str);
				break;
			}
		}

		if (funcid == _MAX_MODE){
			quecst_printf(SYNC_LOG, "********** The function interface does not exist ! **********");
			quecst_send_upper_data("The function interface does not exist !");
		}
#endif
		
		
	}	

}

void quecst_apimf_start(void)
{
	quecst_printf(SYNC_LOG, "********** apimf main process start! ************\r\n");
	int ret;
	ql_uart_config_s uart_test_cfg;

	uart_test_cfg.baudrate      = QL_UART_BAUD_115200;
	uart_test_cfg.data_bit      = QL_UART_DATABIT_8;
	uart_test_cfg.parity_bit    = QL_UART_PARITY_NONE;
	uart_test_cfg.stop_bit      = QL_UART_STOP_1;
	uart_test_cfg.flow_ctrl     = QL_UART_FC_NONE;
	ql_uart_set_dcbconfig(UART_TEST_PORT, &uart_test_cfg);
	ql_uart_open(UART_TEST_PORT);
 	ql_uart_set_rx_cb(UART_TEST_PORT, quecst_uart_rx_isr);
    ql_uart_set_rx_int(UART_TEST_PORT, QL_DEV_ENABLE);

	ql_rtos_queue_create(&quecst_send_queue,MSG_MAX_SIZE, MSG_MAX_NUM);
	ql_rtos_queue_create(&quecst_receive_queue,MSG_MAX_SIZE, MSG_MAX_NUM);

	ql_task_t send_uart_data_task_ref = NULL;
	ql_task_t receive_uart_data_task_ref = NULL;
	ret =ql_rtos_task_create(&send_uart_data_task_ref,
							 (unsigned short)S_TASK_STACK_SIZE,
							TASK_PRIORITY,
							"uart_data_send_task", 
							quecst_send_uart_data_task,
							 NULL);
	if (ret != kNoErr)
	{
		quecst_printf(SYNC_LOG, "Error: Failed to create send_uart_data_task_ref thread: %d\r\n", ret);
		send_uart_data_task_ref =NULL;
		return;
	}

	ret =ql_rtos_task_create(&receive_uart_data_task_ref,
							(unsigned short)R_TASK_STACK_SIZE,
							8,
							"uart_data_receive_task",
							quecst_receive_uart_data_task,
							NULL);	
	if (ret != kNoErr)
	{
		quecst_printf(SYNC_LOG, "Error: Failed to create receive_uart_data_task_ref thread: %d\r\n", ret);
		receive_uart_data_task_ref =NULL;
		return;
	}
	//ql_rtos_task_delete(NULL); 
}


