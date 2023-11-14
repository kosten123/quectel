/*================================================================
  Copyright (c) 2022, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#include "quec_apimf_includes.h"
#include "quec_apimf.h"


#define QL_I2C_MASTER_WRITE_TIMOUT_MS   (100)
#define QL_I2C_MASTER_READ_TIMOUT_MS    (100)
#define QL_I2C_SLAVE_WRITE_TIMOUT_MS   (500)
#define QL_I2C_SLAVE_READ_TIMOUT_MS    (500)
/**
 * @brief 
 * 
 * @param iic_id 
 * @param speed_mode 
 * @param addr_mode 
 * @param slave_addr 
 * @return ql_i2c_errcode_e 
 */

void qlst_i2c_init(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
        
		int ret, iic_id;
		//for ( i = 0; i < 3; i++);
		iic_id = atoi(param_str[PARAM_NUM_1]);
		uint16_t speed_mode = atoi(param_str[PARAM_NUM_2]);
        uint16_t addr_mode = atoi(param_str[PARAM_NUM_3]);
        uint16_t slave_addr ={0x50};
			ret = ql_i2c_init(iic_id, speed_mode , addr_mode, slave_addr);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);		
			quecst_printf(SYNC_LOG, ":re=%d iic_id=%d speed_mode=%d addr_mode=%d slave_addr=%d\r\n", ret,iic_id , speed_mode , addr_mode,slave_addr);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_INIT_ERR, param_num);
		}
	}
/**
 * @brief 
 * 
 * @param iic_id 
 * @return ql_i2c_errcode_e 
 */
void qlst_i2c_deinit(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_1)){
		int ret;
		int iic_id ;
		//for ( iic_id = 0; iic_id < 3; iic_id++);
		iic_id  = atoi(param_str[PARAM_NUM_1]);
		
			ret = ql_i2c_deinit(iic_id);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE);
			quecst_printf(SYNC_LOG, "re:%d iic_id:%d\r\n", ret, iic_id);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_ID_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_ID_INIT_ERR, param_num);
		}
			
	} 


/**
 * @brief 
 * 
 * @param iic_id 
 * @param dev_addr 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */

void qlst_i2c_master_write(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
		int ret;
		int iic_id;
		iic_id  = atoi(param_str[PARAM_NUM_1]);
        uint32_t dev_addr= atoi(param_str[PARAM_NUM_2]);
        uint8_t data[256]={0};
       // data = param_str[PARAM_NUM_2];
        uint32_t size = atoi(param_str[PARAM_NUM_3]);
		
			ret = ql_i2c_master_write(iic_id,dev_addr,data,size);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QL_I2C_WRITE_ERR,QL_I2C_MASTER_WRITE_TIMOUT_MS);
			quecst_printf(SYNC_LOG, "re:%d iic_id:%d dev_addr:%d  data:%d size:%d\r\n", ret, iic_id,dev_addr, data,size);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_ID_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_ID_INIT_ERR, param_num);
		}
			
	} 


/**
 * @brief 
 * 
 * @param iic_id 
 * @param dev_addr 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */

void qlst_i2c_master_read(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_4)){
		int ret;
		int iic_id;
         uint8_t data[256]={0};
		iic_id  = atoi(param_str[PARAM_NUM_1]);
        uint32_t dev_addr= atoi(param_str[PARAM_NUM_2]);
        
        //data =param_str[PARAM_NUM_3];
        uint32_t size = atoi(param_str[PARAM_NUM_4]);
		
			ret = ql_i2c_master_read(iic_id,dev_addr,data,size);
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QL_I2C_READ_ERR,QL_I2C_MASTER_WRITE_TIMOUT_MS);
			quecst_printf(SYNC_LOG, "re:%d iic_id:%d dev_addr:%d  data:%d size:%d\r\n", ret, iic_id,dev_addr, data,size);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_ID_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_ID_INIT_ERR, param_num);
		}
		
	} 

/**
 * @brief 
 * 
 * @param iic_id 
 * @param dev_addr 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
void qlst_i2c_master_mem_write(int param_num, char (*param_str)[PARAM_SIZE])           // !!!
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_6)){
		 int ret;
		 int iic_id, mem_addr,mem_size,size;
         uint8_t data[]={0};
		 iic_id  = atoi(param_str[PARAM_NUM_1]);
         uint32_t dev_addr= atoi(param_str[PARAM_NUM_2]);        
         mem_addr =atoi(param_str[PARAM_NUM_3]);
         mem_size = atoi(param_str[PARAM_NUM_4]);
		 size = atoi(param_str[PARAM_NUM_5]);

				ret =ql_i2c_master_mem_write(iic_id,dev_addr,mem_addr,mem_size,data,size);
         		quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QL_I2C_WRITE_ERR,QL_I2C_MASTER_WRITE_TIMOUT_MS);
		 		quecst_printf(SYNC_LOG, "re:%d iic_id:%d dev_addr:%d mem_addr:%d mem_size:%d data:%d size:%d\r\n", ret,iic_id,dev_addr,mem_addr,mem_size, data,size);
			} 
    else{
		 quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		 quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}
/**
 * @brief 
 * 
 * @param iic_id 
 * @param dev_addr 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
void qlst_i2c_master_mem_read(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_6)){
		 int ret;
		 int iic_id,mem_size,size;
        uint8_t data[]={0};
		 uint32_t dev_addr={0x50};
		 iic_id  = atoi(param_str[PARAM_NUM_1]);
       //  uint32_t dev_addr= atoi(param_str[PARAM_NUM_2]);        
         uint32_t mem_addr =atoi(param_str[PARAM_NUM_2]);
         mem_size = atoi(param_str[PARAM_NUM_3]);
		 size = atoi(param_str[PARAM_NUM_4]);
		 size=(sizeof(data));
		
				ret =ql_i2c_master_mem_read(iic_id,dev_addr,mem_addr,mem_size,data,size);

         		quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QL_I2C_READ_ERR,QL_I2C_MASTER_WRITE_TIMOUT_MS);
		 		quecst_printf(SYNC_LOG, "re:%d iic_id:%d dev_addr:%d mem_addr:%d mem_size:%d data:%d size:%d\r\n", ret,iic_id,dev_addr,mem_addr,mem_size, data,size);
			}
    else{
		 quecst_send_upper_data("%s=%x\r\n", funcid_str,PARAM_ERROR);
		 quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", PARAM_ERROR,param_num);
	}
}

/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
void qlst_i2c_slave_write(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_3)){
		 int ret;
		 int iic_id, size;
         uint8_t data[64]={0};
		 iic_id  = atoi(param_str[PARAM_NUM_1]);     
		 size = atoi(param_str[PARAM_NUM_2]);
		// memcpy(data,size);

			ret =ql_i2c_slave_write(iic_id,data,size);
	switch(ret)
   {
    case  QL_I2C_SUCCESS:
        quecst_printf(SYNC_LOG," QL_I2C_SUCCESS\r\n");
        break;
    case  QL_I2C_INIT_ERR:
         quecst_printf(SYNC_LOG,"QL_I2C_INIT_ERR\r\n");
        break;
    case  QL_I2C_ID_INIT_ERR:
         quecst_printf(SYNC_LOG,"  QL_I2C_ID_INIT_ERR\r\n");
        break;
    case QL_I2C_SM_BUS_ERR:
         quecst_printf(SYNC_LOG,"  QL_I2C_SM_BUS_ERR\r\n");
        break;
    case QL_I2C_ACK_TIMEOUT_ERR:
         quecst_printf(SYNC_LOG," QL_I2C_ACK_TIMEOUT_ERR\r\n");
    case QL_I2C_WRITE_ERR:
         quecst_printf(SYNC_LOG," QL_I2C_WRITE_ERR\r\n");
	case QL_I2C_READ_ERR:
         quecst_printf(SYNC_LOG,"QL_I2C_READ_ERR\r\n");	 
        break;       
    default:
            break;
    }
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QLST_FAILURE,QL_I2C_SLAVE_WRITE_TIMOUT_MS);
			quecst_printf(SYNC_LOG, "re:%d iic_id:%d data:%d size:%d\r\n", ret, iic_id, data,size);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_ID_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_ID_INIT_ERR, param_num);
		}
			
	} 

/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
void qlst_i2c_slave_read(int param_num, char (*param_str)[PARAM_SIZE])
{
	if (param_num ==  PARAM_NUM_COUNT(PARAM_NUM_2)){
		 int ret;
		 int iic_id, size;
         uint8_t data[256]={0};
		 iic_id  = atoi(param_str[PARAM_NUM_1]);     
		 size = atoi(param_str[PARAM_NUM_2]);
		// memcpy(data,size);
		
			ret =ql_i2c_slave_read(iic_id,data,size);
	
			quecst_send_upper_data("%s=%d\r\n", funcid_str,ret==0?QLST_SUCCESS:QL_I2C_READ_ERR,QL_I2C_SLAVE_READ_TIMOUT_MS);
			quecst_printf(SYNC_LOG, "re:%d iic_id:%d data:%d size:%d\r\n", ret,iic_id,data,size);
		}
		else{
			quecst_send_upper_data("%s=%x\r\n", funcid_str,QL_I2C_ID_INIT_ERR);
			quecst_printf(SYNC_LOG, ":re=%x param_num:%d\r\n", QL_I2C_ID_INIT_ERR, param_num);
		}
			
	} 
