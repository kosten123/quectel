/**
 * @file ql_i2c_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * I2C0_SCL     20
 * I2C0_SDA     21
 * I2C1_SCL     42
 * I2C1_SDA     43
 * I2C2_SCL     0
 * I2C2_SDA     1
 * 
 */

#include <quec_i2c/ql_i2c.h>
#include<quec_common/ql_defines.h>

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
ql_i2c_errcode_e ql_i2c_init(ql_i2c_id_e iic_id,
                            ql_i2c_speed_mode_e speed_mode,
                            ql_i2c_addr_mode_e addr_mode,
                            uint16_t slave_addr)
{
    if((iic_id<0||iic_id>2)||(speed_mode<0||speed_mode>3)||(addr_mode<0||addr_mode>1)){
        return QL_I2C_INIT_ERR;
    }
    i2c_config_t cfg = {0};
    if( QL_I2C_SPEED_STANDARD == speed_mode){
        cfg.baud_rate = 100000; /* 100KHz */
    }else if (QL_I2C_SPEED_FAST == speed_mode){
        cfg.baud_rate = 400000; /* 400KHz */
    }else if(QL_I2C_SPEED_FAST_PLUS == speed_mode){
        cfg.baud_rate = 1000000; /*1MHz*/
    }else if(QL_I2C_SPEED_HIGH_SPEED == speed_mode){
        cfg.baud_rate = 3400000; /* 3.4MHz */
    }else{
        cfg.baud_rate = 100000; /* 100KHz */
    }

    cfg.addr_mode = addr_mode;
    cfg.slave_addr = slave_addr;

    return bk_i2c_init(iic_id, &cfg);
}

/**
 * @brief 
 * 
 * @param iic_id 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_deinit(ql_i2c_id_e iic_id)
{
    if(iic_id<0||iic_id>2){
        return QL_I2C_ID_INIT_ERR;
    }
    return bk_i2c_deinit(iic_id);
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
ql_i2c_errcode_e ql_i2c_master_write(ql_i2c_id_e iic_id,
                                    uint32_t dev_addr,
                                    const uint8_t* data,
                                    uint32_t size)
{
    if(iic_id<0||iic_id>2){
        return QL_I2C_ID_INIT_ERR;
    }
    return bk_i2c_master_write(iic_id, dev_addr, data, size, QL_I2C_MASTER_WRITE_TIMOUT_MS);
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
ql_i2c_errcode_e ql_i2c_master_read(ql_i2c_id_e iic_id,
                                    uint32_t dev_addr,
                                    uint8_t *data,
                                    uint32_t size)
{
    if(iic_id<0||iic_id>2){
        return QL_I2C_ID_INIT_ERR;
    }
    return bk_i2c_master_read(iic_id,dev_addr, data, size, QL_I2C_MASTER_READ_TIMOUT_MS);

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
ql_i2c_errcode_e ql_i2c_master_mem_write(ql_i2c_id_e iic_id,
                                    uint32_t dev_addr,
                                    uint32_t mem_addr,
                                    ql_i2c_mem_addr_size_e mem_size,
                                    uint8_t* data,
                                    uint32_t size)
{
    if((iic_id<0||iic_id>2)||(mem_size<0||mem_size>1)){
        return QL_I2C_SM_BUS_ERR;
    }
    i2c_mem_param_t mem_param = {0};
    mem_param.data          = data;
    mem_param.data_size     = size;
    mem_param.dev_addr      = dev_addr;
    mem_param.mem_addr      = mem_addr;
    mem_param.mem_addr_size = mem_size;
    mem_param.timeout_ms    = QL_I2C_MASTER_WRITE_TIMOUT_MS;

    return bk_i2c_memory_write(iic_id, &mem_param);

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
ql_i2c_errcode_e ql_i2c_master_mem_read(ql_i2c_id_e iic_id,
                                    uint32_t dev_addr,
                                    uint32_t mem_addr,
                                    ql_i2c_mem_addr_size_e mem_size,
                                    uint8_t *data,
                                    uint32_t size)
{
    if((iic_id<0||iic_id>2)||(mem_size<0||mem_size>1)){
        return QL_I2C_SM_BUS_ERR;
    }
    i2c_mem_param_t mem_param = {0};
    mem_param.data          = data;
    mem_param.data_size     = size;
    mem_param.dev_addr      = dev_addr;
    mem_param.mem_addr      = mem_addr;
    mem_param.mem_addr_size = mem_size;
    mem_param.timeout_ms    = QL_I2C_MASTER_READ_TIMOUT_MS;

    return bk_i2c_memory_read(iic_id, &mem_param);
}

/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_slave_write(ql_i2c_id_e iic_id, const uint8_t *data, uint32_t size)
{
    if(iic_id<0||iic_id>2){
        return QL_I2C_ID_INIT_ERR;
    }
    return bk_i2c_slave_write(iic_id, data, size, QL_I2C_SLAVE_WRITE_TIMOUT_MS);
}

/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_slave_read(ql_i2c_id_e iic_id, uint8_t *data, uint32_t size)
{
    if(iic_id<0||iic_id>2){
        return QL_I2C_ID_INIT_ERR;
    }
    return bk_i2c_slave_read(iic_id, data, size, QL_I2C_SLAVE_READ_TIMOUT_MS);
}
