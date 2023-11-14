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

#include "cli.h"


extern void cli_aud_intf_record_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_play_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_sd_voc_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_set_voc_param_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_set_aec_param_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_get_aec_param_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_set_samp_rate_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_doorbell_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_aud_debug_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_loop_test_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
extern void cli_aud_intf_play_const_data_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);

#if CONFIG_AUD_TRAS_DAC_DEBUG
extern void cli_aud_intf_debug_cmd(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv);
#endif

#define AUD_INTF_CMD_CNT (sizeof(s_aud_intf_commands) / sizeof(struct cli_command))
static const struct cli_command s_aud_intf_commands[] = {
	{"aud_intf_record_test", "aud_intf_record_test {init|start|pause|stop|set_chl|deinit xxx.pcm|mic1|dual}", cli_aud_intf_record_cmd},
	{"aud_intf_play_test", "{init|start|pause|stop|set_chl|deinit xxx.pcm|left|dual}", cli_aud_intf_play_cmd},
	{"aud_intf_sd_voc_test", "aud_intf_sd_voc_test {start|stop xx.pcm, xx.pcm}", cli_aud_intf_sd_voc_cmd},
	{"aud_intf_set_voc_param_test", "aud_intf_set_voc_param_test {param value}", cli_aud_intf_set_voc_param_cmd},
	{"aud_intf_set_aec_param_test", "aud_intf_set_aec_param_test {param value}", cli_aud_intf_set_aec_param_cmd},
	{"aud_intf_get_aec_param_test", "aud_intf_get_aec_param_test", cli_aud_intf_get_aec_param_cmd},
	{"aud_intf_set_samp_rate_test", "aud_intf_set_samp_rate_test {param value}", cli_aud_intf_set_samp_rate_cmd},
	{"aud_intf_doorbell_test", "aud_intf_doorbell_test {start|stop}", cli_aud_intf_doorbell_cmd},
	/* debug cmd */
#if CONFIG_AUD_TRAS_DAC_DEBUG
	{"aud_intf_debug_test", "aud_intf_debug_test {on|off}", cli_aud_intf_debug_cmd},
#endif
	{"aud_intf_aud_debug_test", "aud_intf_aud_debug_test {aud_debug|tx|rx|aec on|off}", cli_aud_intf_aud_debug_cmd},
	{"aud_intf_loop_test", "aud_intf_loop_test {start|stop}", cli_aud_intf_loop_test_cmd},
	{"aud_intf_play_const_data_test", "aud_intf_play_const_data_test {start|stop}", cli_aud_intf_play_const_data_cmd},
};

int cli_aud_intf_init(void)
{
	return cli_register_commands(s_aud_intf_commands, AUD_INTF_CMD_CNT);
}

