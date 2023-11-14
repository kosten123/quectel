/**
 * @file ql_i2c.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_I2C_H
#define _QL_I2C_H

#include <quec_common/ql_platform.h>

typedef enum
{
    QL_I2C0 = 0,
    QL_I2C1 = 1,
    QL_I2C2 = 2,
}ql_i2c_id_e;

typedef enum
{
    QL_I2C_SPEED_STANDARD   = 0,
    QL_I2C_SPEED_FAST,
    QL_I2C_SPEED_FAST_PLUS,
    QL_I2C_SPEED_HIGH_SPEED,
}ql_i2c_speed_mode_e;

typedef enum
{
    QL_I2C_ADDR_SIZE_7BIT   = 0,
    QL_I2C_ADDR_SIZE_10BIT,
}ql_i2c_addr_mode_e;

typedef enum
{
    QL_I2C_MEM_ADDR_SIZE_8BIT   = 0,
    QL_I2C_MEM_ADDR_SIZE_16BIT,
}ql_i2c_mem_addr_size_e;

typedef enum
{
    QL_I2C_SUCCESS          = 0,
    QL_I2C_INIT_ERR,
    QL_I2C_ID_INIT_ERR,
    QL_I2C_SM_BUS_ERR,
    QL_I2C_ACK_TIMEOUT_ERR,
    QL_I2C_WRITE_ERR,
    QL_I2C_READ_ERR,
}ql_i2c_errcode_e;


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
                            uint16_t slave_addr);

/**
 * @brief 
 * 
 * @param iic_id 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_deinit(ql_i2c_id_e iic_id);


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
                                    uint32_t size);

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
                                    uint32_t size);


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
                                    uint32_t size);

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
                                    uint32_t size);


/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_slave_write(ql_i2c_id_e iic_id, const uint8_t *data, uint32_t size);


/**
 * @brief 
 * 
 * @param iic_id 
 * @param data 
 * @param size 
 * @return ql_i2c_errcode_e 
 */
ql_i2c_errcode_e ql_i2c_slave_read(ql_i2c_id_e iic_id, uint8_t *data, uint32_t size);

#endif
