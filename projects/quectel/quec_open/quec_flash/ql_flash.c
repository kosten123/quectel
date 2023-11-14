/**
 * @file ql_flash_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include<quec_flash/ql_flash.h>
#include<quec_common/ql_defines.h>

static ql_logic_partition_info_t s_partition_info = {0};

/**
 * @brief 
 * 
 * @param type 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_security(ql_flash_protect_type_e type)
{
    if(type<0||type>3){
        return QL_FLASH_PARAM_ERR;
    }
    return bk_flash_set_protect_type(type);
}

/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_write_bytes(const uint8_t *data, uint32_t addr, uint32_t size)
{
    bk_flash_write_enable();
    if(bk_flash_write_bytes(addr, data, size)!=BK_OK){
        return QL_FLASH_EXECUTE_ERR;
    }
    return bk_flash_write_disable();
}

/**
 * @brief 
 * 
 * @param data 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_read_bytes(uint8_t *data, uint32_t addr, uint32_t size)
{
    return bk_flash_read_bytes(addr, data, size);
}

/**
 * @brief 
 * 
 * @param addr 
 * @param len 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_erase(uint32_t addr, uint32_t size)
{
    //TODO:
    return QL_FLASH_EXECUTE_ERR; //bk_flash_erase(addr, size);
}

/**
 * @brief 
 * 
 * @param inPartition 
 * @param off_set 
 * @param size 
 * @return ql_flash_errcode_e 
 */
ql_flash_errcode_e ql_flash_partition_erase(ql_partition_e inPartition, uint32_t off_set, uint32_t size)
{
    if(inPartition<0||inPartition>9){
        return QL_FLASH_PARAM_ERR;
    }
    return bk_flash_partition_erase(inPartition, off_set, size);
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
ql_flash_errcode_e ql_flash_partition_write( ql_partition_e inPartition, volatile uint32_t off_set, uint8_t *inBuffer , uint32_t inBufferLength)
{
    if(inPartition<0||inPartition>9){
        return QL_FLASH_PARAM_ERR;
    }
    return bk_flash_partition_write(inPartition, inBuffer, off_set, inBufferLength);
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
ql_flash_errcode_e ql_flash_partition_read(ql_partition_e inPartition, volatile uint32_t off_set, uint8_t *outBuffer, uint32_t outBufferLength)
{
    if(inPartition<0||inPartition>9){
        return QL_FLASH_PARAM_ERR;
    }
    return bk_flash_partition_read(inPartition, outBuffer, off_set, outBufferLength);
}

/**
 * @brief 
 * 
 * @param partition 
 * @return ql_logic_partition_info_t* 
 */
ql_logic_partition_info_t *ql_flash_partition_get_info(ql_partition_e partition)
{
    if(partition<0||partition>9){
        return NULL;
    }
    bk_logic_partition_t* partition_prt = NULL;
    partition_prt = bk_flash_partition_get_info(partition);
    if(NULL != partition_prt)
    {
        s_partition_info.partition_description = partition_prt->partition_description;
        s_partition_info.partition_owner       = partition_prt->partition_owner;
        s_partition_info.partition_start_addr  = partition_prt->partition_start_addr;
        s_partition_info.partition_length      = partition_prt->partition_length; 
        s_partition_info.partition_options     = partition_prt->partition_options; 
        return &s_partition_info;
    }else{
        return NULL;
    }
}
