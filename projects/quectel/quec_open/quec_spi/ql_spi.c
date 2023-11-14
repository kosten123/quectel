/**
 * @file ql_spi_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * SPI PORT NUMBER:
 * SPI0_SCK         44
 * SPI0_CSN         45
 * SPI0_MOSI        46
 * SPI0_MISO        47
 * 
 * SPI1_SCK         2
 * SPI1_CSN         3
 * SPI1_MOSI        4
 * SPI1_MISO        5
 * 
 * SPI2_SCK         14
 * SPI2_CSN         15
 * SPI2_MOSI        16
 * SPI2_MISO        17
 * 
 */

#include<quec_spi/ql_spi.h>
#include<quec_common/ql_defines.h>

#if (CONFIG_SPI_DMA)
static dma_id_t spi_tx_dma_chan;  /**< SPI tx dma channel */
static dma_id_t spi_rx_dma_chan;  /**< SPI rx dma channel */
#endif
/**
 * @brief 
 * 
 * @param spi_cfg 
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_init(ql_spi_id_e spi_id, ql_spi_config_s *spi_cfg)
{
    if(spi_id >= QL_SPI_ID_MAX){
        return QL_SPI_INVALID_PARAM_ERR;
    }
    spi_config_t cfg;
    cfg.baud_rate = spi_cfg->spiclk;
    cfg.bit_order = spi_cfg->bit_order;
    cfg.bit_width = spi_cfg->bit_width;
    cfg.phase     = spi_cfg->cpha;
    cfg.polarity  = spi_cfg->cpol;
    cfg.role      = spi_cfg->role;
    cfg.wire_mode = spi_cfg->wire_mode;

    if((cfg.bit_width<0||cfg.bit_width>1)
        ||(cfg.bit_order<0||cfg.bit_order>1)
        ||(cfg.phase<0||cfg.phase>1)
        ||(cfg.polarity<0||cfg.polarity>1)
        ||(cfg.role<0||cfg.role>1)
        ||(cfg.wire_mode<0||cfg.wire_mode>1)){
            return QL_SPI_NOT_INIT_ERR;
        }

#if (CONFIG_SPI_DMA)
    cfg.dma_mode            = SPI_DMA_MODE_ENABLE;
    spi_tx_dma_chan         = bk_dma_alloc(DMA_DEV_DTCM);
    spi_rx_dma_chan         = bk_dma_alloc(DMA_DEV_DTCM);
    cfg.spi_tx_dma_chan     = spi_tx_dma_chan;
    cfg.spi_rx_dma_chan     = spi_rx_dma_chan;
#endif
    return bk_spi_init(spi_id, &cfg);  
}

/**
 * @brief 
 * 
 * @param spi_id 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_deinit(ql_spi_id_e spi_id)
{
    if(spi_id >= QL_SPI_ID_MAX){
        return QL_SPI_INVALID_PARAM_ERR;
    }
#if (CONFIG_SPI_DMA)
    bk_dma_free(DMA_DEV_DTCM, spi_tx_dma_chan);
    bk_dma_free(DMA_DEV_DTCM, spi_rx_dma_chan);
#endif
    return bk_spi_deinit(spi_id);
}

/**
 * @brief 
 * 
 * @param spi_id
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_transfer(ql_spi_id_e spi_id, ql_spi_message_s *spi_msg)
{
    if(spi_id >= QL_SPI_ID_MAX){
        return QL_SPI_INVALID_PARAM_ERR;
    }
    if(NULL == spi_msg)
    {
        return QL_SPI_INVALID_PARAM_ERR;
    }

    if(NULL == spi_msg->recv_buf || NULL == spi_msg->send_buf || 0 == spi_msg->recv_len || 0 == spi_msg->send_len)
    {
        return QL_SPI_INVALID_PARAM_ERR;
    }
#if (CONFIG_SPI_DMA)
    return bk_spi_dma_transmit(spi_id, spi_msg->send_buf, spi_msg->send_len, spi_msg->recv_buf, spi_msg->recv_len);
#else
    return bk_spi_transmit(spi_id, spi_msg->send_buf, spi_msg->send_len, spi_msg->recv_buf, spi_msg->recv_len);
#endif
}

/**
 * @brief 
 * 
 * @param spi_id 
 * @param data 
 * @param size 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_write_bytes(ql_spi_id_e spi_id, void *data, uint32_t size)
{
    if(spi_id >= QL_SPI_ID_MAX){
        return QL_SPI_INVALID_PARAM_ERR;
    }
    if(data == NULL)
    {
        return QL_SPI_INVALID_PARAM_ERR;
    }
    return bk_spi_write_bytes(spi_id, data, size);
}

/**
 * @brief 
 * 
 * @param spi_id 
 * @param data 
 * @param size 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e ql_spi_read_bytes(ql_spi_id_e spi_id, void *data, uint32_t size)
{
    if(spi_id >= QL_SPI_ID_MAX){
        return QL_SPI_INVALID_PARAM_ERR;
    }
    if(data == NULL)
    {
        return QL_SPI_INVALID_PARAM_ERR;
    }
   return  bk_spi_read_bytes(spi_id, data, size);
}

/**
 * @brief 
 * 
 * @param spi_id
 * @param spi_msg 
 * @return ql_spi_errcode_e 
 */
ql_spi_errcode_e bk_spi_master_xfer(ql_spi_id_e id,ql_spi_message_s *msg)
{
	uint32_t buf_len = msg->send_len + msg->recv_len;

	uint8_t *send_data = (uint8_t *)os_zalloc(buf_len);
	if (send_data == NULL) {
		return BK_FAIL;
	}
	uint8_t *recv_data = (uint8_t *)os_zalloc(buf_len);
	if (recv_data == NULL) {
		if (send_data) {
			os_free(send_data);
			send_data = NULL;
		}
		return BK_FAIL;
	}

	for (int i = 0; i < msg->send_len; i++) {
		send_data[i] = *((char*)msg->send_buf + i);
	}

	bk_spi_dma_duplex_init(id);
	BK_LOG_ON_ERR(bk_spi_dma_duplex_xfer(id, send_data, buf_len, recv_data, buf_len));
	bk_spi_dma_duplex_deinit(id);

	for (int i = 0; i < msg->recv_len; i++) {
		msg->recv_buf[i] =*(recv_data + msg->send_len + i);
	}

	if (recv_data) {
		os_free(recv_data);
		recv_data = NULL;
	}

	if (send_data) {
		os_free(send_data);
		send_data = NULL;
	}

	return BK_OK;
}