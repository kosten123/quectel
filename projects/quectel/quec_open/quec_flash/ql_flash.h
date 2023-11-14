/**
 * @file ql_flash.h
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _QL_FLASH_PUB_H
#define _QL_FLASH_PUB_H

#include<quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    QL_FLASH_SUCCESS    = 0,
    QL_FLASH_EXECUTE_ERR,
    QL_FLASH_PARAM_ERR,
}ql_flash_errcode_e;

typedef enum
{
    QL_FLASH_PROTECT_NONE = 0,
    QL_FLASH_PROTECT_ALL,
    QL_FLASH_PROTECT_HALF,
    QL_FLASH_UNPROTECT_LAST_BLOCK,
} ql_flash_protect_type_e;


typedef enum
{
    QL_PARTITION_BOOTLOADER = 0,
	QL_PARTITION_APPLICATION,
	QL_PARTITION_OTA,
	QL_PARTITION_APPLICATION1,
	QL_PARTITION_MATTER_FLASH,
	QL_PARTITION_RF_FIRMWARE,
	QL_PARTITION_NET_PARAM,
	QL_PARTITION_USR_CONFIG,
	QL_PARTITION_OTA_FINA_EXECUTIVE,
	QL_PARTITION_MAX,
} ql_partition_e;

typedef enum{
    QL_FLASH_EMBEDDED = 0,
    QL_FLASH_SPI,
    QL_FLASH_MAX,
    QL_FLASH_NONE
}ql_flash_type_e;

typedef struct{
    ql_flash_type_e partition_owner;
    const char* partition_description;
    uint32_t partition_start_addr;
    uint32_t partition_length;
    uint32_t partition_options;
}ql_logic_partition_info_t;


/**
 * @brief 
 * 
 * @param type 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_security(ql_flash_protect_type_e type);

/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_write_bytes(const uint8_t *data, uint32_t addr, uint32_t size);

/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_read_bytes(uint8_t *data, uint32_t addr, uint32_t size);

/**
 * @brief 
 * 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_erase(uint32_t addr, uint32_t size);


/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param size 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_partition_erase(ql_partition_e inPartition, uint32_t off_set, uint32_t size);

/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param inBuffer 
 * @param inBufferLength 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_partition_write( ql_partition_e inPartition, volatile uint32_t off_set, uint8_t *inBuffer , uint32_t inBufferLength);

/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param outBuffer 
 * @param outBufferLength 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_partition_read( ql_partition_e inPartition, volatile uint32_t off_set, uint8_t *outBuffer, uint32_t outBufferLength);

/**
 * @brief 
 * 
 * @param partition 
 * @return ql_logic_partition_info_t* 
 */
ql_logic_partition_info_t *ql_flash_partition_get_info(ql_partition_e partition);

#ifdef __cplusplus
}
#endif
#endif
