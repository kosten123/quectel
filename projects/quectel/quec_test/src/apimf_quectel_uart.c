/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
#include "quec_apimf_includes.h"
#include "quec_apimf.h"
         
		 
/**
 * @brief 
 * 
 * @param port 
 * @param dcb 
 * @return ql_uart_errcode_e 
 */
static ql_uart_config_s dcb[3];
void  qlst_uart_set_dcbconfig (int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_6)){
        int ret,port=0;
		    
        port = atoi(param_str[PARAM_NUM_1]);
			  dcb[port].baudrate   = QL_UART_BAUD_115200;
        dcb[port].data_bit   = QL_UART_DATABIT_8;
        dcb[port].flow_ctrl  =  QL_UART_FC_NONE ;
        dcb[port].parity_bit = QL_UART_PARITY_NONE;
        dcb[port].stop_bit   =  QL_UART_STOP_1;	 
		    
        ret = ql_uart_set_dcbconfig(port,&dcb[port]);

        quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		    quecst_printf(SYNC_LOG, "re:%d   port:%d\r\n", ret,port);
	} 
  else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
void  qlst_uart_get_dcbconfig (int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_6)){
        int ret,port=0;
		    
        port = atoi(param_str[PARAM_NUM_1]);
			  dcb[port].baudrate   = QL_UART_BAUD_115200;
        dcb[port].data_bit   = QL_UART_DATABIT_8;
        dcb[port].flow_ctrl  =  QL_UART_FC_NONE ;
        dcb[port].parity_bit = QL_UART_PARITY_NONE;
        dcb[port].stop_bit   =  QL_UART_STOP_1;	 
		    
        ret = ql_uart_set_dcbconfig(port,&dcb[port]);

        quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		    quecst_printf(SYNC_LOG, "re:%d   port:%d\r\n", ret,port);
	} 
  else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
 void qlst_uart_open(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
           int ret;
		       uint32_t port;
		       port = atoi(param_str[PARAM_NUM_1]);
          
         ret = ql_uart_open(port);
         quecst_send_upper_data("%s=%d\r\n",funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d   port:%d\r\n", ret,port);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
void qlst_uart_close(int param_num, char (*param_str)[PARAM_SIZE]) 
{
  if(param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
    int ret;
    uint32_t port;
    port = atoi(param_str[PARAM_NUM_1]);
    
    ret = ql_uart_close(port);
    quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
    quecst_printf(SYNC_LOG, "re:%d   port:%d\r\n", ret,port);
  } 
  else{
    quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
    quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
  }
}
/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
 void qlst_uart_write(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
         int ret,data_len;
		     uint32_t port;
         char data[32]={0};
        // unsigned char data[]={0x00,0X01,0X02,0x03,0X04,0X05,0x06,0X07,0X08,0x09,0X0a,0X0b,0x0c,0X0d,0X0e,0x0f,0X10,0X11,
		    // 0X12,0x13,0X14,0X15,0x16,0X17,0X18,0Xff,0x0c,0X0d,0X0e,0x0f,0X10,0X11};
        // uint16_t data_len=sizeof(data);
		     port = atoi(param_str[PARAM_NUM_1]); 
         strcpy(data,param_str[PARAM_NUM_2]);         
         data_len = atoi(param_str[PARAM_NUM_3]);  
         ret = ql_uart_write(port,(unsigned char *)data,data_len);
         quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d  data:%s data_len:%d\r\n", ret,port,data,data_len);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
 void qlst_uart_read(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		     uint32_t port;
         unsigned char data[64]={0};
         uint16_t data_len=0;
		     port = atoi(param_str[PARAM_NUM_1]); 
         data_len = atoi(param_str[PARAM_NUM_2]);  
         ret = ql_uart_read(port,data,&data_len);
         quecst_send_upper_data("%s=%d,%s\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,data);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d  data:%s data_len:%d\r\n", ret,port,data,data_len);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
 void qlst_uart_read_timeout(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
         int ret;
		     uint32_t port;
         unsigned char data=0;
        uint16_t data_len=0;
          uint32_t timeout;
         port = atoi(param_str[PARAM_NUM_1]);
         data_len = atoi(param_str[PARAM_NUM_2]);
          timeout=atoi(param_str[PARAM_NUM_3]);

         ret = ql_uart_read_timeout(port,&data,&data_len,timeout);
         quecst_send_upper_data("%s=%d,%s\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,data);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d  data:%s data_len:%d\r\n", ret,port,data,data_len,timeout);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param port 
 * @param uart_cb 
 * @return ql_uart_errcode_e 
 */
 typedef void(*ql_uart_callback)(int port, void *param);
 static void uart_example_rx_isr(int port, void *param)
{ 
	 quecst_printf(SYNC_LOG,"uart(%d) enter uart_example_rx_isr\r\n",port);
    uint8_t rev_buf[20];
    uint16_t rev_len = 20;
    ql_uart_read(port, rev_buf, &rev_len);
    for(int i = 0; i < rev_len; i++){
         quecst_printf(SYNC_LOG,"%c ",rev_buf[i]);
    }
    ql_uart_set_rx_int(port, QL_DEV_ENABLE);
}

void qlst_uart_set_rx_cb(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		     uint32_t port;        
		     port = atoi(param_str[PARAM_NUM_1]);
        if(port>2||port<0){
        quecst_printf(SYNC_LOG, ":ERROR=%x param_num:%d\r\n", PARAM_ERROR,QL_UART_INVALID_PARAM_ERR);
        return ;
        }                  
         ret = ql_uart_set_rx_cb(port,uart_example_rx_isr);
         quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d  data:%d uart_example_rx_isr:%d\r\n", ret,port,uart_example_rx_isr);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param port 
 * @param set 
 * @return ql_uart_errcode_e 
 */
 typedef void(*ql_uart_callback)(int port, void *param);
static void uart_example_tx_isr(int port, void *param)
{ 
	 quecst_printf(SYNC_LOG,"uart(%d) enter uart_example_rx_isr\r\n",port);
    uint8_t rev_buf[20];
    uint16_t rev_len = 20;
    ql_uart_read(port, rev_buf, &rev_len);
    for(int i = 0; i < rev_len; i++){
         quecst_printf(SYNC_LOG,"%c ",rev_buf[i]);
    }
    ql_uart_set_tx_int(port, QL_DEV_ENABLE);
}
void qlst_uart_set_tx_cb(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		     uint32_t port;        
		     port = atoi(param_str[PARAM_NUM_1]);
         if(port>2||port<0){
        quecst_printf(SYNC_LOG, ":ERROR=%x param_num:%d\r\n", PARAM_ERROR,QL_UART_INVALID_PARAM_ERR);
        return ;
        }                  
         ret = ql_uart_set_tx_cb(port,uart_example_tx_isr);
         quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d  data:%d \r\n", ret,port);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
 }

void qlst_uart_set_rx_int(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		     uint32_t port;
         unsigned int set=0;
         if (set>2)
         {
           quecst_printf(SYNC_LOG, ":ERROR=%x param_num:%d\r\n", PARAM_ERROR,QL_UART_INVALID_PARAM_ERR);
         return ;
         }
		     port = atoi(param_str[PARAM_NUM_1]);
         if(port>2||port<0){
        quecst_printf(SYNC_LOG, ":ERROR=%x param_num:%d\r\n", PARAM_ERROR,QL_UART_INVALID_PARAM_ERR);
        return ;
        }  
          set = atoi(param_str[PARAM_NUM_2]);
          
         ret = ql_uart_set_rx_int(port,set);
         quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d set:%d\r\n", ret,port,set);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param port 
 * @param set 
 * @return ql_uart_errcode_e 
 */
void qlst_uart_set_tx_int(int param_num, char (*param_str)[PARAM_SIZE]) 
 {
 if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		     uint32_t port;
         unsigned int set;
		     port = atoi(param_str[PARAM_NUM_1]);
         if(port>2||port<0){
        quecst_printf(SYNC_LOG, ":ERROR=%x param_num:%d\r\n", PARAM_ERROR,QL_UART_INVALID_PARAM_ERR);
        return ;
        }  
         set = atoi(param_str[PARAM_NUM_2]);  
         ret = ql_uart_set_tx_int(port,set);
         quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		     quecst_printf(SYNC_LOG, "re:%d  port:%d set:%d\r\n", ret,port,set);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}


