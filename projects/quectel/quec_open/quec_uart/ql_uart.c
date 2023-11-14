/**
 * @file ql_uart_bk7256.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 *  UART Port Number
 *  Capability      BK7256      BK7256_cp1
 *  Port Number        3            3
 *  
 * UART1:
 *  UART0_TX    11
 *  UART0_RX    10
 * 
 * UART2:
 *  UART1_TX    0
 *  UART1_RX    1
 * 
 *UART3:
 *  UART2_TX    41
 *  UART2_RX    40
 * 
**/


#include<quec_uart/ql_uart.h>
#include<quec_common/ql_defines.h>

static ql_uart_config_s uart_config[3];

/**
 * @brief 
 * 
 * @param port 
 * @param dcb 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_set_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb)
{
    if(port == QL_UART_PORT_0)
    {
        uart_config[0].baudrate   = dcb->baudrate;
        uart_config[0].data_bit   = dcb->data_bit;
        uart_config[0].flow_ctrl  = dcb->flow_ctrl;
        uart_config[0].parity_bit = dcb->parity_bit;
        uart_config[0].stop_bit   = dcb->stop_bit;

    }
    else if(port == QL_UART_PORT_1)
    {
        uart_config[1].baudrate   = dcb->baudrate;
        uart_config[1].data_bit   = dcb->data_bit;
        uart_config[1].flow_ctrl  = dcb->flow_ctrl;
        uart_config[1].parity_bit = dcb->parity_bit;
        uart_config[1].stop_bit   = dcb->stop_bit;
    }
    else if(port == QL_UART_PORT_2)
    {
        uart_config[2].baudrate   = dcb->baudrate;
        uart_config[2].data_bit   = dcb->data_bit;
        uart_config[2].flow_ctrl  = dcb->flow_ctrl;
        uart_config[2].parity_bit = dcb->parity_bit;
        uart_config[2].stop_bit   = dcb->stop_bit;
    }
    else
    {
        return QL_UART_INVALID_PARAM_ERR;
    }
    return QL_UART_SUCCESS;

}


/**
 * @brief 
 * 
 * @param port 
 * @param dcb 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_get_dcbconfig(ql_uart_port_number_e port, ql_uart_config_s *dcb)
{
	if(port == QL_UART_PORT_0)
	{
     dcb->baudrate      = uart_config[0].baudrate;
	 dcb->data_bit      = uart_config[0].data_bit; 
	 dcb->parity_bit    = uart_config[0].parity_bit;
	 dcb->stop_bit      = uart_config[0].stop_bit;
	 dcb->flow_ctrl     = uart_config[0].flow_ctrl;
	}
	else if(port == QL_UART_PORT_1)
	{
	  dcb->baudrate     = uart_config[1].baudrate;
	  dcb->data_bit     = uart_config[1].data_bit;
	  dcb->parity_bit   = uart_config[1].parity_bit;
	  dcb->stop_bit     = uart_config[1].stop_bit;
	  dcb->flow_ctrl    = uart_config[1].flow_ctrl;
	}
    else if(port == QL_UART_PORT_2)
	{
	  dcb->baudrate     = uart_config[2].baudrate;
	  dcb->data_bit     = uart_config[2].data_bit;
	  dcb->parity_bit   = uart_config[2].parity_bit;
	  dcb->stop_bit     = uart_config[2].stop_bit;
	  dcb->flow_ctrl    = uart_config[2].flow_ctrl;
	}
	else
    {
        return QL_UART_INVALID_PARAM_ERR;
    }
    return QL_UART_SUCCESS;    
}

/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_open(ql_uart_port_number_e port)
{
	uart_config_t uart_cfg;

	if(port == QL_UART_PORT_0)
	{
		uart_cfg.baud_rate  = (uint32_t)uart_config[0].baudrate;
		uart_cfg.data_bits  = uart_config[0].data_bit;
		uart_cfg.parity     = uart_config[0].parity_bit;
		uart_cfg.stop_bits  = uart_config[0].stop_bit;
		uart_cfg.flow_ctrl  = uart_config[0].flow_ctrl;
		uart_cfg.src_clk    = UART_CLOCK;

	}
	else if(port == QL_UART_PORT_1)
	{
		uart_cfg.baud_rate  = (uint32_t)uart_config[1].baudrate;
		uart_cfg.data_bits  = uart_config[1].data_bit;
		uart_cfg.parity     = uart_config[1].parity_bit;
		uart_cfg.stop_bits  = uart_config[1].stop_bit;
		uart_cfg.flow_ctrl  = uart_config[1].flow_ctrl;
		uart_cfg.src_clk    = UART_CLOCK;
	}
    else if(port == QL_UART_PORT_2)
	{
		uart_cfg.baud_rate  = (uint32_t)uart_config[2].baudrate;
		uart_cfg.data_bits  = uart_config[2].data_bit;
		uart_cfg.parity     = uart_config[2].parity_bit;
		uart_cfg.stop_bits  = uart_config[2].stop_bit;
		uart_cfg.flow_ctrl  = uart_config[2].flow_ctrl;
		uart_cfg.src_clk    = UART_CLOCK;
	}
	else
	{
		return QL_UART_INVALID_PARAM_ERR;
	}

	QL_LOG_ON_ERR(bk_uart_init(port, &uart_cfg));

   	return QL_UART_SUCCESS;   
}

/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_close(ql_uart_port_number_e port)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
	QL_LOG_ON_ERR(bk_uart_register_rx_isr(port, NULL, NULL));
	QL_LOG_ON_ERR(bk_uart_register_tx_isr(port, NULL, NULL));
	QL_LOG_ON_ERR(bk_uart_deinit(port));
	return QL_UART_SUCCESS;   
}

/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_write(ql_uart_port_number_e port, unsigned char *data, uint16_t data_len)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
    if(!(data&&data_len))
  	    return QL_UART_MEM_ADDR_NULL_ERR;
    if((port!=QL_UART_PORT_1)&&(port!=QL_UART_PORT_2) && (port != QL_UART_PORT_0))
  	    return QL_UART_INVALID_PARAM_ERR;

	QL_LOG_ON_ERR(bk_uart_write_bytes(port, data, data_len));
	return QL_UART_SUCCESS;
}

/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_read(ql_uart_port_number_e port, unsigned char *data, uint16_t *data_len)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
	int ret;
	if(!(data&&data_len))
		return QL_UART_MEM_ADDR_NULL_ERR;
  	if((port!=QL_UART_PORT_1)&&(port!=QL_UART_PORT_2)&&(port != QL_UART_PORT_0))
		return QL_UART_INVALID_PARAM_ERR;

	ret = bk_uart_read_bytes( port, data, *data_len, BEKEN_NO_WAIT);

	if(ret >= BK_OK)
	{
	    *data_len = ret;
		return QL_UART_SUCCESS;
	}
	else
	{
		return QL_UART_EXECUTE_ERR;
	}   
}

/**
 * @brief 
 * 
 * @param port 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_read_timeout(ql_uart_port_number_e port, unsigned char *data, uint16_t *data_len, uint32_t timeout)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
	int ret;
	if(!(data&&data_len))
		return QL_UART_MEM_ADDR_NULL_ERR;
  	if((port!=QL_UART_PORT_1)&&(port!=QL_UART_PORT_2) && (port != QL_UART_PORT_0))
		return QL_UART_INVALID_PARAM_ERR;

    bk_uart_set_enable_rx(port, true);
	ret = bk_uart_read_bytes( port, data, *data_len, timeout);

	if(ret >= BK_OK)
	{
	    *data_len = ret;
		return QL_UART_SUCCESS;
	}
	else
	{
		return QL_UART_EXECUTE_ERR;
	}     
}

/**
 * @brief 
 * 
 * @param port 
 * @param uart_cb 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_set_rx_cb(ql_uart_port_number_e port, ql_uart_callback uart_cb)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
	if (NULL == uart_cb)
	{
		return QL_UART_INVALID_PARAM_ERR;
	}
	QL_LOG_ON_ERR(bk_uart_register_rx_isr(port, (uart_isr_t)uart_cb, NULL));
	return QL_UART_SUCCESS;    
}

/**
 * @brief 
 * 
 * @param port 
 * @param uart_cb 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_set_tx_cb(ql_uart_port_number_e port, ql_uart_callback uart_cb)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
    if (NULL == uart_cb)
	{
		return QL_UART_INVALID_PARAM_ERR;
	}

	QL_LOG_ON_ERR(bk_uart_register_tx_isr(port, (uart_isr_t)uart_cb, NULL));
	return QL_UART_SUCCESS;
}

/**
 * @brief 
 * 
 * @param port 
 * @param set 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_set_rx_int(ql_uart_port_number_e port, unsigned int set)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }

	if((port!=QL_UART_PORT_1)&&(port!=QL_UART_PORT_2) && (port != QL_UART_PORT_0))
	{
		return QL_UART_INVALID_PARAM_ERR;
	}

    if (QL_DEV_DISABLE == set)
	{
		bk_uart_enable_sw_fifo(port);
		QL_LOG_ON_ERR(bk_uart_disable_rx_interrupt(port));
	}
	else if (QL_DEV_ENABLE == set)
	{
		bk_uart_disable_sw_fifo(port);
		QL_LOG_ON_ERR(bk_uart_enable_rx_interrupt(port));
	}

	return QL_UART_SUCCESS;    
}

/**
 * @brief 
 * 
 * @param port 
 * @param set 
 * @return ql_uart_errcode_e 
 */
ql_uart_errcode_e ql_uart_set_tx_int(ql_uart_port_number_e port, unsigned int set)
{
	if(port<0||port>2){
        return QL_UART_INVALID_PARAM_ERR;
    }
	if((port!=QL_UART_PORT_1)&&(port!=QL_UART_PORT_2) && (port != QL_UART_PORT_0))
	{
		return QL_UART_INVALID_PARAM_ERR;
	}

    if (QL_DEV_DISABLE == set)
	{
		QL_LOG_ON_ERR(bk_uart_disable_tx_interrupt(port));
	}
	else if (QL_DEV_ENABLE == set)
	{
		QL_LOG_ON_ERR(bk_uart_enable_tx_interrupt(port));
	}

	return QL_UART_SUCCESS;    
}

