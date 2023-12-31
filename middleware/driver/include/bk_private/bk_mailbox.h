// Copyright 2020-2021 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define MAILBOX_DEV_NAME                ("mailbox")

#define MAILBOX_SUCCESS                 (0)
#define MAILBOX_UNKNOW_CMD              (1)
#define MAILBOX_NOT_READY               (2)

#define MAILBOX_CMD_NEED_RSP_FLAG	0x80000000
#define MAILBOX_CMD_IS_RSP_FLAG  	0x40000000
#define MAILBOX_CMD_FAST_RSP_FLAG	0x20000000

typedef enum
{
	MAILBOX_CMD_COMMON_SUBSET = 0x0000,
	MAILBOX_CMD_REG_READ,
	MAILBOX_CMD_REG_WRITE,
	MAILBOX_CMD_CYCLE_COUNTER_RESET,
	MAILBOX_CMD_CYCLE_COUNTER_GET,

	MAILBOX_CMD_AUDIO_COMMON_SUBSET = 0x0100,
	MAILBOX_CMD_AUDIO_INIT,
	MAILBOX_CMD_AUDIO_DEINIT,
	MAILBOX_CMD_AUDIO_FIFO_CONFIG,
	MAILBOX_CMD_AUDIO_FIFO_STATUS,

	MAILBOX_CMD_AUDIO_WRITE_THRED_SET,

	MAILBOX_CMD_AUDIO_ADC_SUBSET = 0x0200,
	MAILBOX_CMD_AUDIO_ADC_INIT,
	MAILBOX_CMD_AUDIO_ADC_DEINIT,
	MAILBOX_CMD_AUDIO_ADC_SAMPLE_RATE_SET, //= 0x0203,
	MAILBOX_CMD_AUDIO_ADC_SAMPLE_RATE_GET,
	MAILBOX_CMD_AUDIO_ADC_ANALOG_VOLUME_SET,
	MAILBOX_CMD_AUDIO_ADC_ANALOG_VOLUME_GET,
	MAILBOX_CMD_AUDIO_ADC_DIGITAL_VOLUME_SET,
	MAILBOX_CMD_AUDIO_ADC_DIGITAL_VOLUME_GET, //= 0x0208,
	MAILBOX_CMD_AUDIO_ADC_VOLUME_SET,
	MAILBOX_CMD_AUDIO_ADC_VOLUME_GET,
	MAILBOX_CMD_AUDIO_ADC_ENABLE, //= 0x020b,
	MAILBOX_CMD_AUDIO_ADC_PCM_CHECK, //Check fill space for read
	MAILBOX_CMD_AUDIO_ADC_PCM_READ,

	MAILBOX_CMD_AUDIO_ADC_PCM_READ_DONE,
	MAILBOX_CMD_AUDIO_ADC_INT_ENABLE,
	MAILBOX_CMD_AUDIO_ADC_HPF2_BYPASS_SET, //= 0x0210,
	MAILBOX_CMD_AUDIO_ADC_GAIN_SET,
	MAILBOX_CMD_AUDIO_ADC_RECORD,

	MAILBOX_CMD_AUDIO_DAC_SUBSET = 0x0300,
	MAILBOX_CMD_AUDIO_DAC_INIT,
	MAILBOX_CMD_AUDIO_DAC_DEINIT,
	MAILBOX_CMD_AUDIO_DAC_SAMPLE_RATE_SET,
	MAILBOX_CMD_AUDIO_DAC_SAMPLE_RATE_GET,
	MAILBOX_CMD_AUDIO_DAC_ANALOG_VOLUME_SET,
	MAILBOX_CMD_AUDIO_DAC_ANALOG_VOLUME_GET,
	MAILBOX_CMD_AUDIO_DAC_DIGITAL_VOLUME_SET,
	MAILBOX_CMD_AUDIO_DAC_DIGITAL_VOLUME_GET, // = 0x0308,
	MAILBOX_CMD_AUDIO_DAC_VOLUME_SET,
	MAILBOX_CMD_AUDIO_DAC_VOLUME_GET,
	MAILBOX_CMD_AUDIO_DAC_ENABLE, // = 0x030b,
	MAILBOX_CMD_AUDIO_DAC_PCM_CHECK, //Check free space for write
	MAILBOX_CMD_AUDIO_DAC_PCM_WRITE,
	MAILBOX_CMD_AUDIO_DAC_FRAC_COEF,

	MAILBOX_CMD_AUDIO_DAC_PCM_WRITE_DONE,
	MAILBOX_CMD_AUDIO_DAC_ADC_LOOPBACK_SET, // = 0x0310,
	MAILBOX_CMD_AUDIO_DAC_HPF1_BYPASS_SET,
	MAILBOX_CMD_AUDIO_DAC_HPF2_BYPASS_SET,
	MAILBOX_CMD_AUDIO_DAC_INT_ENABLE,
	MAILBOX_CMD_AUDIO_DAC_BPS_SET,
	MAILBOX_CMD_AUDIO_DAC_BPS_GET,

	MAILBOX_CMD_FUNC_SUBSET = 0x0400,

	MAILBOX_CMD_AUDIO_AUX_SUBSET = 0x0500,
	MAILBOX_CMD_AUDIO_AUX_INIT,
	MAILBOX_CMD_AUDIO_AUX_DEINIT,
	MAILBOX_CMD_AUDIO_AUX_ENABLE,

	MAILBOX_CMD_AUDIO_IDLE_SUBSET = 0x0600,
	MAILBOX_CMD_AUDIO_BT_IDLE,
	MAILBOX_CMD_AUDIO_BT_WORKING,
	MAILBOX_CMD_AUDIO_WIFI_SLEEP,
	MAILBOX_CMD_AUDIO_WIFI_WAKEUP,

	MAILBOX_CMD_DEBUG_SUBSET = 0x0700,

	MAILBOX_CMD_HCI_SEND = 0x0800,

	MAILBOX_CMD_BLE_SET_ADDR = 0x0900,
	MAILBOX_CMD_BLE_START_ADV,
	MAILBOX_CMD_BLE_START_ADV_CMP,
	MAILBOX_CMD_BLE_STOP_ADV,
	MAILBOX_CMD_BLE_STOP_ADV_CMP,
	MAILBOX_CMD_BLE_READ_CMP,
	MAILBOX_CMD_BLE_WRITE_CMP,
	MAILBOX_CMD_BLE_SEND_NTF,
	MAILBOX_CMD_BLE_SEND_NTF_CMP,
	MAILBOX_CMD_BLE_SEND_IND,
	MAILBOX_CMD_BLE_SEND_IND_CMP,
	MAILBOX_CMD_BLE_CONN_CMP,
	MAILBOX_CMD_BLE_DISC,
	MAILBOX_CMD_BLE_DISC_CMP,
	MAILBOX_CMD_BLE_START_SCAN,
	MAILBOX_CMD_BLE_STOP_SCAN,
	MAILBOX_CMD_BLE_SCAN_DECODER,
	MAILBOX_CMD_BLE_GET_RSSI,
	MAILBOX_CMD_BLE_GET_RSSI_CMP,
	MAILBOX_CMD_BLE_GET_STATUS,
	MAILBOX_CMD_BLE_GET_STATUS_CMP,

	MAILBOX_CMD_TEST_SUBSET = 0xFF00,
	MAILBOX_CMD_AUDIO_ADC_TEST,
	MAILBOX_CMD_AUDIO_DAC_TEST,
	MAILBOX_CMD_AUDIO_ADC_STATS,
	MAILBOX_CMD_AUDIO_DAC_STATS,
	MAILBOX_CMD_AUDIO_VOL_ADD,
	MAILBOX_CMD_AUDIO_VOL_SUB,
	MAILBOX_CMD_FLOAT_POINT_TEST,
	MAILBOX_CMD_SMEM_TEST,

	MAILBOX_CMD_SET_MASK = 0xFFFF
} mailbox_msgtype_t;

void mailbox_driver_init(void);
void mailbox_driver_deinit(void);

#ifdef __cplusplus
}
#endif
