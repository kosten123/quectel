/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#include "quec_apimf_includes.h"
#include "quec_apimf.h"
#include <string.h>

/**
 * @brief 
 * 
 * @param type 
 * @return ql_flash_errcode_e 
 */

void qlst_flash_security(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int type ;
		type = atoi(param_str[PARAM_NUM_1]);
			ret =  ql_flash_security(type);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d  type:%d\r\n", ret, type);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_FLASH_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_FLASH_PARAM_ERR,param_num); 
		}
	} 

 
/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
void qlst_flash_write_bytes (int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
        int ret;
		// const uint8_t data[]={0x00,0X01,0X02,0x03,0X04,0X05,0x06,0X07,0X08,0x09,0X0a,0X0b,0x0c,0X0d,0X0e,0x0f,0X10,0X11,
		//  0X12,0x13,0X14,0X15,0x16,0X17,0X18,0Xff,0x0c,0X0d,0X0e,0x0f,0X10,0X11};
		char data[32]={0};
         uint32_t addr;
         uint32_t size;
        // size =(sizeof(data));
		// data = atoi(param_str[PARAM_NUM_1]);
		//  data = param_str[PARAM_NUM_1];
		 strcpy(data,param_str[PARAM_NUM_1]);
         addr = atoi(param_str[PARAM_NUM_2]);
         size = atoi(param_str[PARAM_NUM_3]);
		ret =  ql_flash_write_bytes((const uint8_t *)data,addr,size);
        quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		quecst_printf(SYNC_LOG, "re:%d  data:%s addr:%d size:%d\r\n", ret,data,addr, size);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
void qlst_flash_read_bytes(int param_num, char (*param_str)[PARAM_SIZE])         //重启可能是数据内存溢出所导致的      不死机了，但读写存在疑问
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		 uint8_t data[64] = {0};
         uint32_t addr;
         uint32_t size = 0;
         size =(sizeof(data));
		// data = atoi(param_str[PARAM_NUM_1]);
         addr = atoi(param_str[PARAM_NUM_1]);
         size = atoi(param_str[PARAM_NUM_2]);
		 ret =  ql_flash_read_bytes(data,addr,size);
          
        quecst_send_upper_data("%s=%d,%s\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,data);
		quecst_printf(SYNC_LOG, "re:%d  addr:%d data:%s size:%d\r\n", ret,addr,data, size);
	}
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
void  qlst_flash_erase (int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
         int ret;
		 uint32_t addr;
         uint32_t size;
         addr = atoi(param_str[PARAM_NUM_1]);
         size = atoi(param_str[PARAM_NUM_2]);
		 ret =  ql_flash_erase(addr,size);

        quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
		quecst_printf(SYNC_LOG, "re:%d  addr:%d size:%d\r\n", ret,addr, size);
	} 
    else{
		quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param size 
 * @return ql_flash_errcode_e 
 */
void  qlst_flash_partition_erase(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
        int ret,size;
		ql_partition_e inPartition=atoi(param_str[PARAM_NUM_1]);
        uint32_t off_set = atoi(param_str[PARAM_NUM_2]);
         size = atoi(param_str[PARAM_NUM_3]);
		
			ret =  ql_flash_partition_erase(inPartition,off_set,size);
        	quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d inPartition:%d  off_set:%d size:%d\r\n", ret,inPartition,off_set, size);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_FLASH_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_FLASH_PARAM_ERR,param_num); 
		}
	} 

/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param inBuffer 
 * @param inBufferLength 
 * @return ql_flash_errcode_e 
 */
void  qlst_flash_partition_write(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
        int ret,inBufferLength ;
		volatile uint32_t off_set;
		uint8_t inBuffer=0;
		ql_partition_e inPartition=atoi(param_str[PARAM_NUM_1]);
         off_set = atoi(param_str[PARAM_NUM_2]);
         inBuffer = atoi(param_str[PARAM_NUM_3]);
		 inBufferLength = atoi(param_str[PARAM_NUM_4]);
	
			ret =  ql_flash_partition_write(inPartition,off_set,&inBuffer,inBufferLength);
        	quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d inPartition:%d off_set:%d inBuffer :%d inBufferLength:%d\r\n", ret,inPartition,off_set, inBuffer ,inBufferLength);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_FLASH_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_FLASH_PARAM_ERR,param_num); 
		}
		 
	} 

/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param outBuffer 
 * @param outBufferLength 
 * @return ql_flash_errcode_e 
 */
void  qlst_flash_partition_read(int param_num, char (*param_str)[PARAM_SIZE])
{
    if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
        int ret,outBufferLength ;
		volatile uint32_t off_set;
		uint8_t outBuffer=0;
		ql_partition_e inPartition=atoi(param_str[PARAM_NUM_1]);
         off_set = atoi(param_str[PARAM_NUM_2]);
        // inBuffer = atoi(param_str[PARAM_NUM_3]);
		 outBufferLength = atoi(param_str[PARAM_NUM_3]);
	
			ret =  ql_flash_partition_read(inPartition, off_set, &outBuffer, outBufferLength);
        	quecst_send_upper_data("%s=%d,%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,outBuffer);
			quecst_printf(SYNC_LOG, "re:%d inPartition:%d  outBuffer:%d  off_set:%d outBufferLength:%d\r\n", ret,inPartition,outBuffer, off_set ,outBufferLength);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_FLASH_PARAM_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_FLASH_PARAM_ERR,param_num); 
		}
		 
	} 
