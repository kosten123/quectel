/**
 * @file cli_quectel_test.c
 * @author Zhang Yuanyuan (yuan.zhang@quectel.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-15
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "cli.h"
#include "cli_config.h"
#include <string.h>
#include <os/os.h>
#include <components/log.h>

#define QUECTEL_V_STR           "v0.1"
#define QUECTEL_CMD_RSP_SUCCEED "QUECTEL: SUCCESS\r\n"
#define QUECTEL_CMD_RSP_ERR     "QUECTEL: ERROR\r\n"

static void cli_factory_version(char *pcWriteBuffer, int xWriteBufferLen, int argc, char **argv)
{
    char *msg = QUECTEL_CMD_RSP_SUCCEED;
    if(1 == argc){
        os_printf("quectel test version: %s \r\n", QUECTEL_V_STR);
    }
    if(0 == os_strcmp(argv[1], "version")){
        os_printf("quectel test version: %s \r\n", QUECTEL_V_STR);
    }else if(0 == os_strcmp(argv[1], "help")){
        os_printf("quectel test help. \r\n");
    }else if(0 == os_strcmp(argv[1], "log")){
        os_printf("quectel test log. \r\n");
    }else{
        msg = QUECTEL_CMD_RSP_ERR;
    }

    if(2 < argc){
        msg = QUECTEL_CMD_RSP_ERR;
    }
    os_memcpy(pcWriteBuffer, msg, os_strlen(msg));
}

#define QUECTEL_TEST_CMD_CNT    (sizeof(s_quectel_commands) / sizeof(struct cli_command))

static const struct cli_command s_quectel_commands[] = {
    {"quectel", "quectel version|help|log", cli_quectel_info},
}

int cli_quectel_test_init(void)
{
    return cli_register_commands(s_quectel_commands, QUECTEL_TEST_CMD_CNT);
}

