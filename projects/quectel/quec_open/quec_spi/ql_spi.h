/**
 * @file ql_spi_pub.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_SPI_PUB_H
#define _QL_SPI_PUB_H

#include<quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    QL_SPI_ID0         = 0,
    QL_SPI_ID1,
    QL_SPI_ID_MAX,
}ql_spi_id_e;

typedef enum
{
    QL_SPI_CPOL_LOW     = 0,    /**<POLARITY: SCK is low at idle mode */
    QL_SPI_CPOL_HIGH,           /**<POLARITY: SCK is high at idle mode */
}ql_spi_cpol_pol_e;

typedef enum
{
    QL_SPI_CPHA_1ST_EDGE    = 0,      /**< PHASE: Sample data at SCK first edge */ 
    QL_SPI_CPHA_2ND_Edge    = 1,      /**< PHASE: Sample data at SCK second edge */
}ql_spi_cpha_pol_e;

typedef enum
{
    QL_SPI_MSB_FIRST      = 0,
    QL_SPI_LSB_FIRST,
}ql_spi_bit_order_e;

typedef enum
{
    QL_SPI_ROLE_SLAVE    = 0,       /**< SPI as Master */ 
    QL_SPI_ROLE_MASTER,             /**<SPI ad SLAVE */
}ql_spi_role_e;

typedef enum
{
    QL_SPI_BIT_WIDTH_8BITS  = 0,
    QL_SPI_BIT_WIDTH_16BITS,
}ql_spi_bit_width_e;

typedef enum
{
    QL_SPI_4WIRE_MODE   = 0,
    QL_SPI_3WIRE_MODE,
}ql_spi_wire_mode_e;

typedef enum
{
    QL_SPI_POL_MODE0    = 0,    /**< CPOL_L, CPHA_L */
    QL_SPI_POL_MODE1,           /**< CPOL_L, CPHA_H */
    QL_SPI_POL_MODE2,           /**< CPOL_H, CPHA_L */
    QL_SPI_POL_MODE3,           /**< CPOL_H, CPHA_H */
}ql_spi_mode_e;

typedef struct 
{
   const void *send_buf;    /**< the data need to send */
   uint32_t send_len;       /**< the data length to send */

   uint8_t *recv_buf;          /**< the data need to receive */
   uint32_t recv_len;       /**< the data length need to receive */
}ql_spi_message_s;

typedef enum
{
    QL_SPI_SUCCESS          = 0,
    QL_SPI_EXECUTE_ERR,
    QL_SPI_INVALID_PARAM_ERR,
    QL_SPI_NOT_INIT_ERR,
}ql_spi_errcode_e;

typedef struct 
{
    uint32_t                    spiclk;
    ql_spi_cpol_pol_e           cpol;
    ql_spi_cpha_pol_e           cpha;
    ql_spi_bit_order_e          bit_order;
    ql_spi_role_e               role;
    ql_spi_wire_mode_e          wire_mode;
    ql_spi_bit_width_e          bit_width;
}ql_spi_config_s;


/**
 * @brief 
 * 
 * @param spi_cfg 
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_init(ql_spi_id_e spi_id, ql_spi_config_s *spi_cfg);

/**
 * @brief 
 * 
 * @param spi_id 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_deinit(ql_spi_id_e spi_id);

/**
 * @brief 
 * 
 * @param spi_cfg 
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_transfer(ql_spi_id_e spi_id, ql_spi_message_s *spi_msg);

/**
 * @brief 
 * 
 * @param spi_id 
 * @param data 
 * @param size 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_write_bytes(ql_spi_id_e spi_id, void *data, uint32_t size);

/**
 * @brief 
 * 
 * @param spi_id 
 * @param data 
 * @param size 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_read_bytes(ql_spi_id_e spi_id, void *data, uint32_t size);

/**
 * @brief 
 * 
 * @param spi_id
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e bk_spi_master_xfer(ql_spi_id_e id,ql_spi_message_s *msg);

#ifdef __cplusplus
}
#endif
#endif
