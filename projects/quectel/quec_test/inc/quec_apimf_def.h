/////////////////////////////////////////////////////////

#ifdef ENUM_INFO
#define PARAM_INFO(mode, str, funcname) mode,
#endif

#ifdef MODESTR_INFO
#define PARAM_INFO(mode, str, funcname) str,
#endif

#ifdef FUNCDECLARE_INFO
#define PARAM_INFO(mode, str, funcname) void funcname(int, char *param_str[PARAM_SIZE]);
#endif

#ifdef FUNCNAME_INFO
#define PARAM_INFO(mode, str, funcname) funcname,
#endif

#define BLE
#define WIFI
#define GPIO
#define I2C
#define PWM
#define SPI
#define WDG
#define ADC
#define FLASH
#define UART
#define TIMER

#ifdef PARAM_INFO
/**************************************************************************************/

#ifdef BLE
PARAM_INFO(_QLST_BLE_INIT, "qlst_ble_init", qlst_ble_init)
PARAM_INFO(_QLST_BLE_DEINIT, "qlst_ble_deinit", qlst_ble_deinit)
PARAM_INFO(_QLST_BLE_CREATE_DB, "qlst_ble_create_db", qlst_ble_create_db)
PARAM_INFO(_QLST_GET_IDLE_ACTV_IDX_HANDLE, "qlst_ble_get_idle_actv_idx_handle", qlst_ble_get_idle_actv_idx_handle)
PARAM_INFO(_QLST_GET_IDLE_CONN_IDX_HANDLE, "qlst_ble_get_idle_conn_idx_handle", qlst_ble_get_idle_conn_idx_handle)
PARAM_INFO(_QLST_BLE_CREATE_ADVERTISING, "qlst_ble_create_advertising", qlst_ble_create_advertising)
PARAM_INFO(_QLST_BLE_START_ADVERTISING, "qlst_ble_start_advertising", qlst_ble_start_advertising)
PARAM_INFO(_QLST_BLE_STOP_ADVERTISING, "qlst_ble_stop_advertising", qlst_ble_stop_advertising)
PARAM_INFO(_QLST_BLE_DELETE_ADVERTISING, "qlst_ble_delete_advertising", qlst_ble_delete_advertising)
PARAM_INFO(_QLST_SET_ADV_DATA, "qlst_ble_set_adv_data", qlst_ble_set_adv_data)
PARAM_INFO(_QLST_BLE_ADV_START, "qlst_ble_adv_start", qlst_ble_adv_start)
PARAM_INFO(_QLST_BLE_ADV_REPORT_DEAL, "app_demo_ble_adv_report_deal", app_demo_ble_adv_report_deal)
PARAM_INFO(_QLST_BLE_CREATE_SCANNING, "qlst_ble_create_scaning", qlst_ble_create_scaning)
PARAM_INFO(_QLST_BLE_START_SCANNING, "qlst_ble_start_scaning", qlst_ble_start_scaning)
PARAM_INFO(_QLST_BLE_STOP_SCANNING, "qlst_ble_stop_scaning", qlst_ble_stop_scaning)
PARAM_INFO(_QLST_BLE_DELETE_SCANNING, "qlst_ble_delete_scaning", qlst_ble_delete_scaning)
PARAM_INFO(_QLST_BLE_UPDATA_PARAM, "qlst_ble_update_param", qlst_ble_update_param)
PARAM_INFO(_QLST_BLE_CREATE_CONN, "qlst_ble_create_conn", qlst_ble_create_conn)
PARAM_INFO(_QLST_BLE_SET_CONNECT_DEV_ADDR, "qlst_ble_set_connect_dev_addr", qlst_ble_set_connect_dev_addr)
PARAM_INFO(_QLST_BLE_START_CONN, "qlst_ble_start_conn", qlst_ble_start_conn)
PARAM_INFO(_QLST_BLE_STOP_CONN, "qlst_ble_stop_conn", qlst_ble_stop_conn)
PARAM_INFO(_QLST_BLE_GATTS_DISCONNECT, "qlst_ble_gatts_disconnect", qlst_ble_gatts_disconnect)
PARAM_INFO(_QLST_BLE_GATTS_SEND_NTF_VALUE, "qlst_ble_gatts_send_ntf_value", qlst_ble_gatts_send_ntf_value)
PARAM_INFO(_QLST_BLE_GATTS_SEND_IND_VALUE, "qlst_ble_gatts_send_ind_value", qlst_ble_gatts_send_ind_value)

#endif

#ifdef WIFI
PARAM_INFO(_QLST_WLAN_START, "qlst_wlan_start", qlst_wlan_start)
PARAM_INFO(_QLST_WLAN_STOP, "qlst_wlan_stop", qlst_wlan_stop)
PARAM_INFO(_QLST_WLAN_START_SCAN, "qlst_wlan_start_scan", qlst_wlan_start_scan)
PARAM_INFO(_QLST_WLAN_STA_SCAN_RESULT, "qlst_wlan_sta_scan_result", qlst_wlan_sta_scan_result)
PARAM_INFO(_QLST_WLAN_START_MONITOR, "qlst_wlan_start_monitor", qlst_wlan_start_monitor)
PARAM_INFO(_QLST_WLAN_STOP_MONITOR, "qlst_wlan_stop_monitor", qlst_wlan_stop_monitor)
PARAM_INFO(_QLST_WLAN_AP_PARA_INFO_GET, "qlst_wlan_ap_para_info_get", qlst_wlan_ap_para_info_get)
PARAM_INFO(_QLST_WLAN_GET_IP_STATUS, "qlst_wlan_get_ip_status", qlst_wlan_get_ip_status)
PARAM_INFO(_QLST_WLAN_GET_LINK_STATUS, "qlst_wlan_get_link_status", qlst_wlan_get_link_status)
PARAM_INFO(_QLST_WLAN_GET_CHANNEL, "qlst_wlan_get_channel", qlst_wlan_get_channel)
PARAM_INFO(_QLST_WLAN_SET_CHANNEL, "qlst_wlan_set_channel", qlst_wlan_set_channel)
PARAM_INFO(_QLST_WLAN_GET_CHANNEL_SYNC, "qlst_wlan_set_channel_sync", qlst_wlan_set_channel_sync)
PARAM_INFO(_QLST_WLAN_OTA_DOWNLOAD, "qlst_wlan_ota_download", qlst_wlan_ota_download)
PARAM_INFO(_QLST_STA_IP_IS_START, "qlst_sta_ip_is_start", qlst_sta_ip_is_start)
#endif

#ifdef GPIO 
PARAM_INFO(_QLST_GPIO_DEINIT, "qlst_gpio_deinit", qlst_gpio_deinit)
PARAM_INFO(_QLST_GPIO_INIT, "qlst_gpio_init", qlst_gpio_init)
PARAM_INFO(_QLST_GPIO_INTERRUPT_INIT, "qlst_gpio_interrupt_init", qlst_gpio_interrupt_init)
PARAM_INFO(_QLST_GPIO_INTERRUPT_ENABLE, "qlst_gpio_interrupt_enable", qlst_gpio_interrupt_enable) 
PARAM_INFO(_QLST_GPIO_GET_LEVEL, "qlst_gpio_get_level", qlst_gpio_get_level) 
PARAM_INFO(_QLST_GPIO_TOGGLE_LEVEL, "qlst_gpio_toggle_level", qlst_gpio_toggle_level)
PARAM_INFO(_QLST_GPIO_SET_LEVEL, "qlst_gpio_set_level", qlst_gpio_set_level)
#endif

#ifdef I2C
PARAM_INFO(_QLST_I2C_INIT, "qlst_i2c_init", qlst_i2c_init)
PARAM_INFO(_QLST_I2C_DEINIT, "qlst_i2c_deinit", qlst_i2c_deinit)
PARAM_INFO(_QLST_I2C_MASTER_WRITE, "qlst_i2c_master_write", qlst_i2c_master_write)
PARAM_INFO(_QLST_I2C_MASTER_READ, "qlst_i2c_master_read", qlst_i2c_master_read)
PARAM_INFO(_QLST_I2C_MASTER_MEM_WRITE, "qlst_i2c_master_mem_write", qlst_i2c_master_mem_write)
PARAM_INFO(_QLST_I2C_MASTER_MEM_READ, "qlst_i2c_master_mem_read", qlst_i2c_master_mem_read)
PARAM_INFO(_QLST_I2C_SLAVE_WRITE, "qlst_i2c_slave_write", qlst_i2c_slave_write)
PARAM_INFO(_QLST_I2C_SLAVE_READ, "qlst_i2c_slave_read", qlst_i2c_slave_read)

#endif

#ifdef PWM
PARAM_INFO(_QLST_PWM_INIT, "qlst_pwm_init", qlst_pwm_init)
PARAM_INFO(_QLST_PWM_DEINIT, "qlst_pwm_deinit", qlst_pwm_deinit)
PARAM_INFO(_QLST_PWM_ENABLE, "qlst_pwm_enable", qlst_pwm_enable)
PARAM_INFO(_QLST_PWM_DISABLE, "qlst_pwm_disable", qlst_pwm_disable)
PARAM_INFO(_QLST_PWM_UPDATE_PARAM, "qlst_pwm_update_param", qlst_pwm_update_param)
#endif

#ifdef WDG
PARAM_INFO(_QLST_WDG_INIT, "qlst_wdg_init", qlst_wdg_init)
PARAM_INFO(_QLST_WDG_RELOAD, "qlst_wdg_reload", qlst_wdg_reload)
PARAM_INFO(_QLST_WDG_FINALIZE, "qlst_wdg_finalize", qlst_wdg_finalize)
#endif

#ifdef ADC
PARAM_INFO(_QLST_ADC_INIT, "qlst_adc_init", qlst_adc_init)
PARAM_INFO(_QLST_ADC_DEINIT, "qlst_adc_deinit", qlst_adc_deinit)
PARAM_INFO(_QLST_ADC_START, "qlst_adc_start", qlst_adc_start)
PARAM_INFO(_QLST_ADC_STOP, "qlst_adc_stop", qlst_adc_stop)
PARAM_INFO(_QLST_ADC_READ, "qlst_adc_read", qlst_adc_read)
PARAM_INFO(_QLST_ADC_READ_RAW, "qlst_adc_read_raw", qlst_adc_read_raw)
#endif


#ifdef SPI
PARAM_INFO(_QLST_SPI_INIT, "qlst_spi_init", qlst_spi_init)
PARAM_INFO(_QLST_SPI_DEINIT, "qlst_spi_deinit", qlst_spi_deinit)
PARAM_INFO(_QLST_SPI_TRANSFER, "qlst_spi_transfer", qlst_spi_transfer)
#endif
#ifdef FLASH
PARAM_INFO(_QLST_FLASH_SECURITY, "qlst_flash_security",  qlst_flash_security)
PARAM_INFO(_QLST_FLASH_WRITE_BYTES, "qlst_flash_write_bytes", qlst_flash_write_bytes)
PARAM_INFO(_QLST_FLASH_READ_BYTES, "qlst_flash_read_bytes", qlst_flash_read_bytes)
PARAM_INFO(_QLST_FLASH_EARSE, "qlst_flash_erase", qlst_flash_erase)
PARAM_INFO(_QLST_FLASH_PARTITION_EARSE, "qlst_flash_partition_erase", qlst_flash_partition_erase)
PARAM_INFO(_QLST_FLASH_PARTITION_WRITE, "qlst_flash_partition_write", qlst_flash_partition_write)
PARAM_INFO(_QLST_FLASH_PARTITION_READ, "qlst_flash_partition_read", qlst_flash_partition_read)


#endif

#ifdef UART
PARAM_INFO(_QLST_UART_SET_DCBCONFIG, "qlst_uart_set_dcbconfig", qlst_uart_set_dcbconfig)
PARAM_INFO(_QLST_UART_GET_DCBCONFIG, "qlst_uart_get_dcbconfig", qlst_uart_get_dcbconfig)
PARAM_INFO(_QLST_UART_OPEN, "qlst_uart_open", qlst_uart_open)
PARAM_INFO(_QLST_UART_CLOSE, "qlst_uart_close", qlst_uart_close)
PARAM_INFO(_QLST_UART_WRITE, "qlst_uart_write", qlst_uart_write)
PARAM_INFO(_QLST_UART_READ, "qlst_uart_read", qlst_uart_read)
PARAM_INFO(_QLST_UART_READ_TIMEOUT, "qlst_uart_read_timeout", qlst_uart_read_timeout)
PARAM_INFO(_QLST_UART_SET_RX_CB, "qlst_uart_set_rx_cb", qlst_uart_set_rx_cb)
PARAM_INFO(_QLST_UART_SET_TX_CB, "qlst_uart_set_tx_cb", qlst_uart_set_tx_cb)
PARAM_INFO(_QLST_UART_SET_RX_INT, "qlst_uart_set_rx_int", qlst_uart_set_rx_int)
#endif

#ifdef TIMER
PARAM_INFO(_QLST_TIMER_INIT, "qlst_timer_init", qlst_timer_init)
PARAM_INFO(_QLST_TIMER_INIT_US, "qlst_timer_init_us",qlst_timer_init_us)
PARAM_INFO(_QLST_TIMER_GET_CNT, "qlst_timer_get_cnt",qlst_timer_get_cnt)
PARAM_INFO(_QLST_TIMER_STOP, "qlst_timer_stop", qlst_timer_stop)
PARAM_INFO(_QLST_TIMER_DEINIT, "qlst_timer_deinit", qlst_timer_deinit)
PARAM_INFO(_QLST_TIMER_START, "qlst_timer_start", qlst_timer_start)
#endif

#undef PARAM_INFO
#endif

#ifdef ENUM_INFO
#undef ENUM_INFO
#endif
#ifdef MODESTR_INFO
#undef MODESTR_INFO
#endif
#ifdef FUNCDECLARE_INFO
#undef FUNCDECLARE_INFO
#endif
#ifdef FUNCNAME_INFO
#undef FUNCNAME_INFO
#endif


