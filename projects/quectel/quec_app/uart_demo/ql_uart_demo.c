/**
 * @file ql_uart_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include<quec_osi/ql_osi.h>
#include<quec_uart/ql_uart.h>

#define UART_TEST_DEBUG
#ifdef UART_TEST_DEBUG
#define ql_uart_log os_printf

#else
#endif
#define QL_WAIT_FOROVER 0xFFFFFFFF
/***************uart test******************/
#define UART_TEST_LEN 20
#define UART_TEST_PORT QL_UART_PORT_2
ql_sem_t uart_rx_test_sema = NULL;
ql_sem_t uart_tx_test_sema = NULL;


static void uart_example_rx_isr(int port, void *param)
{
	ql_uart_log("uart(%d) enter uart_example_rx_isr\r\n", port);
    uint8_t rev_buf[20];
    uint16_t rev_len = 20;
    ql_uart_read(port, rev_buf, &rev_len);
    for(int i = 0; i < rev_len; i++){
        ql_uart_log("%c ",rev_buf[i]);
    }
    ql_uart_set_rx_int(port, QL_DEV_ENABLE);
}

void ql_uart_demo_thread(void *param)
{
	int i = 0, ret = 0; // stop_cmd;set 1 enable , set 0 disable
	UINT8 *send_buf;
	ql_uart_config_s uart_test_cfg;
	ql_uart_config_s uart_get_cfg;

	ql_rtos_task_sleep_ms(3000);

	send_buf = os_malloc(UART_TEST_LEN * sizeof(unsigned char));

	if (send_buf == NULL)
	{
		ql_uart_log("buf malloc failed\r\n");
		return;
	}

	memset(send_buf, 0, UART_TEST_LEN);
	uart_test_cfg.baudrate      = QL_UART_BAUD_115200;
	uart_test_cfg.data_bit      = QL_UART_DATABIT_8;
	uart_test_cfg.parity_bit    = QL_UART_PARITY_NONE;
	uart_test_cfg.stop_bit      = QL_UART_STOP_1;
	uart_test_cfg.flow_ctrl     = QL_UART_FC_NONE;
	ql_uart_set_dcbconfig(UART_TEST_PORT, &uart_test_cfg);
	ret = ql_uart_open(UART_TEST_PORT);
	for (i = 0; i < UART_TEST_LEN; i++)
    {
        send_buf[i] = 'a' + i;
    }	
	ret = ql_uart_write(UART_TEST_PORT, send_buf, 20);
	if (ret == 0)
    {
        ql_uart_log("uart send success !\r\n");
    }
	ql_uart_log("uart rx test, please enter %d bytes using uart3: \r\n", UART_TEST_LEN);
    ql_uart_set_rx_cb(UART_TEST_PORT, uart_example_rx_isr);
    ql_uart_set_rx_int(UART_TEST_PORT, QL_DEV_ENABLE);
	ql_uart_get_dcbconfig(UART_TEST_PORT, &uart_get_cfg);
	ql_uart_log("uart get config:\r\n");
	ql_uart_log("uart baudrate:%d\r\n",     uart_get_cfg.baudrate);
	ql_uart_log("uart data_bit:%d\r\n",     uart_get_cfg.data_bit);
	ql_uart_log("uart stop_bit:%d\r\n",     uart_get_cfg.stop_bit);
	ql_uart_log("uart parity_bit:%d\r\n",   uart_get_cfg.parity_bit);
	ql_uart_log("uart flow_ctrl:%d\r\n",    uart_get_cfg.flow_ctrl);
	ql_uart_log("\r\n");
	ql_uart_log("uart  test over !!\r\n");
	while (1)
	{
		ql_rtos_task_sleep_ms(1000);
	}
}

ql_task_t uart_test_thread_handle = NULL;
void ql_uart_demo_thread_create(void)
{
	int ret;
	ret = ql_rtos_task_create(&uart_test_thread_handle,
							  (unsigned short)2048,
							  BEKEN_APPLICATION_PRIORITY,
							  "uart_test",
							  ql_uart_demo_thread,
							  0);

	if (ret != kNoErr)
	{
		os_printf("Error: Failed to create uart test thread: %d\r\n", ret);
		goto init_err;
	}

	return;

init_err:
	if (uart_test_thread_handle != NULL)
	{
		ql_rtos_task_delete(uart_test_thread_handle);
	}
}
