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

#include <common/bk_include.h>
#include "sys_driver.h"
#include "clock_driver.h"
#include <os/os.h>
#include <os/mem.h>
#include <driver/int.h>
#include <modules/pm.h>
#include "bk_usb.h"
//#include <modules/usb.h>
#include "bk_misc.h"

#define AUD_UAC_DRV_TAG "aud_uac_drv"

#define LOGI(...) BK_LOGI(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGW(...) BK_LOGW(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGE(...) BK_LOGE(AUD_UAC_DRV_TAG, ##__VA_ARGS__)
#define LOGD(...) BK_LOGD(AUD_UAC_DRV_TAG, ##__VA_ARGS__)


bk_err_t bk_aud_uac_driver_init(void)
{
#if CONFIG_USB_UAC
	uint8_t wait_count = 0;
	bk_usb_open(0);
	usb_device_set_using_status(1, USB_UAC_MIC_DEVICE);
	usb_device_set_using_status(1, USB_UAC_SPEAKER_DEVICE);
	while(MGC_DevGetConnectStatus() != 1) {
		if(wait_count > 100) {
			os_printf("wait usb connect timeout 1s!\r\n");
			return BK_FAIL;
			break;
		}
		rtos_delay_milliseconds(10);
		wait_count++;
	}
#endif

	return BK_OK;
}

bk_err_t bk_aud_uac_driver_deinit(void)
{
#if CONFIG_USB_UAC
	usb_device_set_using_status(0, USB_UAC_MIC_DEVICE);
	usb_device_set_using_status(0, USB_UAC_SPEAKER_DEVICE);
	return bk_usb_close();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_start_mic(void)
{
#if CONFIG_USB_UAC
	return bk_uac_start_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_mic(void)
{
#if CONFIG_USB_UAC
	return bk_uac_stop_mic();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_start_spk(void)
{
#if CONFIG_USB_UAC
	return bk_uac_start_speaker();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_stop_spk(void)
{
#if CONFIG_USB_UAC
	return bk_uac_stop_speaker();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_mic_callback(void *cb)
{
#if CONFIG_USB_UAC
	return bk_uac_register_micrxed_packet_callback(cb);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_unregister_mic_callback(void)
{
#if CONFIG_USB_UAC
	return bk_uac_unregister_micrxed_packet_callback();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_spk_buff_ptr(void *buff, uint32_t size)
{
#if CONFIG_USB_UAC
	return bk_uac_register_tx_speakerstream_buffptr(buff, size);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_spk_callback(void *cb)
{
#if CONFIG_USB_UAC
	return bk_uac_register_speakerstream_txed_callback(cb);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_unregister_spk_callback(void)
{
#if CONFIG_USB_UAC
	return bk_uac_unregister_speakerstream_txed_callback();
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_set_spk_samp_rate(uint32_t value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_set_hz(USB_UAC_SPEAKER_DEVICE, value);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_set_mic_samp_rate(uint32_t value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_set_hz(USB_UAC_MIC_DEVICE, value);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_set_spk_gain(uint32_t value)
{
#if CONFIG_USB_UAC
	bk_err_t ret = BK_OK;
	uint32_t vol_min = 0;
	uint32_t vol_max = 0;
	uint32_t vol_set = 0;

	ret = bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MIN, UAC_ATTRIBUTE_VOLUME, &vol_min);
	if (ret != BK_OK) {
		LOGE("get uac volume min value fail \n");
		return ret;
	}

	ret = bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_MAX, UAC_ATTRIBUTE_VOLUME, &vol_max);
	if (ret != BK_OK) {
		LOGE("get uac volume max value fail \n");
		return ret;
	}

	vol_set = (vol_max - vol_min) / 100 * value + vol_min;
	LOGI("vol_max: %04x, vol_min: %04x, value: %04x, vol_set: %04x\n", vol_max, vol_min, value, vol_set);

	return bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_VOLUME, &vol_set);
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_get_spk_gain(uint32_t *value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_attribute_op(USB_ATTRIBUTE_GET_CUR, UAC_ATTRIBUTE_VOLUME, value);
	//LOGW(" The uac speaker not support volume configuration \n");
#else
	return BK_OK;
#endif
}

bk_err_t bk_aud_uac_register_disconnect_cb(void *callback)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_register_disconnect_callback(callback);
#endif
	return BK_OK;
}

bk_err_t bk_aud_uac_register_connect_cb(void *callback)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_register_connect_callback(callback);
#endif
	return BK_OK;
}

bk_err_t bk_aud_uac_check_spk_gain_cfg(void)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_check_support_attribute(UAC_ATTRIBUTE_VOLUME);
#endif
	return BK_OK;
}

bk_err_t bk_aud_uac_ctrl_spk_mute(uint32_t value)
{
#if CONFIG_USB_UAC
	return bk_usb_uac_attribute_op(USB_ATTRIBUTE_SET_CUR, UAC_ATTRIBUTE_MUTE, &value);
#endif
	return BK_OK;
}

