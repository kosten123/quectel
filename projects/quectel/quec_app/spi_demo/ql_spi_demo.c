/**
 * @file ql_spi_demo.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_osi/ql_osi.h>
#include<quec_spi/ql_spi.h>

#define QL_SPI_TEST_DEBUG
#ifdef QL_SPI_TEST_DEBUG
#define ql_spi_log  os_printf
#else
#endif

#define SPI_TEST_LEN        64
#define SPI_TEST_PORT       QL_SPI_ID0
#define SPI_MASTER_CLK      (26000000)

static void ql_spi_demo_thread(void *param)
{
    ql_spi_errcode_e ret;
    ql_spi_config_s cfg;
    ql_spi_message_s msg;
    uint8_t tx_buf[SPI_TEST_LEN] = {0xAB};
    uint8_t rx_buf[SPI_TEST_LEN] = {0xFF};
    msg.recv_buf = rx_buf;
    msg.send_buf = tx_buf;
    msg.recv_len = SPI_TEST_LEN;
    msg.send_len = SPI_TEST_LEN;
    
    cfg.bit_order   = QL_SPI_MSB_FIRST;
    cfg.bit_width   = QL_SPI_BIT_WIDTH_8BITS;
    cfg.cpha        = QL_SPI_CPHA_1ST_EDGE;
    cfg.cpol        = QL_SPI_CPOL_LOW;
    cfg.role        = QL_SPI_ROLE_MASTER;
    cfg.wire_mode   = QL_SPI_4WIRE_MODE;
    cfg.spiclk      = SPI_MASTER_CLK;

    ql_rtos_task_sleep_ms(1000);

    ret = ql_spi_init(SPI_TEST_PORT, &cfg);

    if(QL_SPI_SUCCESS == ret){
        ql_spi_log("success: success spi init: %d\r\n", ret);
    }else{
        ql_spi_log("Error: Failed spi init: %d\r\n", ret);
    }
    ql_spi_log("SPI SEND DATA:\r\n");
    ql_spi_transfer(SPI_TEST_PORT, &msg);
    ql_spi_log("SPI SEND DATA OVER !!\r\n");
    while(1){
        
        ql_rtos_task_sleep_ms(2000);
    }
}

ql_task_t spi_test_thread_handle = NULL;
void ql_spi_demo_thread_create(void)
{
	int ret;
	ret = ql_rtos_task_create(&spi_test_thread_handle,
							  (unsigned short)2048,
							  BEKEN_APPLICATION_PRIORITY,
							  "uart_test",
							  ql_spi_demo_thread,
							  0);

	if (ret != kNoErr)
	{
		os_printf("Error: Failed to create SPI test thread: %d\r\n", ret);
		goto init_err;
	}

	return;

init_err:
	if (spi_test_thread_handle != NULL)
	{
		ql_rtos_task_delete(spi_test_thread_handle);
	}
}


