/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"


#define SPI_TEST_LEN        64
#define SPI_TEST_PORT       QL_SPI_ID0
#define SPI_MASTER_CLK      (26000000)

/**
 * @brief 
 * 
 * @param spi_cfg 
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
void qlst_spi_init(int param_num, char (*param_str)[PARAM_SIZE])
{
	if(param_num==2){
    int ret ,spi_id;	
    ql_spi_config_s cfg;
    cfg.bit_order   = QL_SPI_MSB_FIRST;
    cfg.bit_width   = QL_SPI_BIT_WIDTH_8BITS;
    cfg.cpha        = QL_SPI_CPHA_1ST_EDGE;
    cfg.cpol        = QL_SPI_CPOL_LOW;
    cfg.role        = QL_SPI_ROLE_MASTER;
    cfg.wire_mode   = QL_SPI_4WIRE_MODE;
    cfg.spiclk      = SPI_MASTER_CLK;
    spi_id  = atoi(param_str[PARAM_NUM_1]);
      ret = ql_spi_init(spi_id,&cfg);  
      quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
      quecst_printf(SYNC_LOG, ":re=%d spi_id=%d\r\n", ret,spi_id);
    }
    else{
      quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_SPI_INVALID_PARAM_ERR);
      quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_SPI_INVALID_PARAM_ERR, param_num);
    }
  }

/**
 * @brief 
 * 
 * @param spi_id 
 * @return ql_spi_errcode_e 
 */
void qlst_spi_deinit(int param_num, char (*param_str)[PARAM_SIZE])
{       
	if(param_num == PARAM_NUM_COUNT(PARAM_NUM_1)){
    int ret ,spi_id ;	        
    spi_id  = atoi(param_str[PARAM_NUM_1]);
    
      ret = ql_spi_deinit(spi_id);  
      quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
      quecst_printf(SYNC_LOG, ":re=%d spi_id=%d\r\n", ret,spi_id);
    }
    else{
      quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_SPI_INVALID_PARAM_ERR);
      quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_SPI_INVALID_PARAM_ERR, param_num);
    }
  }

/**
 * @brief 
 * 
 * @param spi_cfg 
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
void qlst_spi_transfer(int param_num, char (*param_str)[PARAM_SIZE])
{       
	if (param_num==2){
    int ret ,spi_id;	
    // ql_spi_config_s cfg;
    ql_spi_message_s msg;
    uint8_t tx_buf[SPI_TEST_LEN] = {0xAB};
    uint8_t rx_buf[SPI_TEST_LEN] = {0xFF};
    msg.recv_buf = rx_buf;
    msg.send_buf = tx_buf;
    msg.recv_len = 1;
    msg.send_len = 1;
    spi_id  = atoi(param_str[PARAM_NUM_1]);
    
      ret = ql_spi_transfer(spi_id,&msg);  
      // quecst_send_upper_data("%s=%d,%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,tx_buf,rx_buf);		
      // quecst_printf(SYNC_LOG, ":re=%d spi_id=%d \r\n", ret,spi_id);
      quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
      quecst_printf(SYNC_LOG, ":re=%d spi_id=%d \r\n", ret,spi_id);
    }
    else{
      quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_SPI_INVALID_PARAM_ERR);
      quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_SPI_INVALID_PARAM_ERR, param_num);
    }
  }
