/**
 * @file ql_sys_pub.h
 * @author Song Huanwei (terrin.song@quectel.com)
 * @brief
 * @version 0.1
 * @date 2023-06-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef _QL_SYS_PUB_H
#define _QL_SYS_PUB_H

#include <quec_common/ql_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    QL_RESET_SOURCE_POWERON                   = 0x0,
    QL_RESET_SOURCE_REBOOT                    = 0x1,
    QL_RESET_SOURCE_WATCHDOG                  = 0x2,
    QL_RESET_SOURCE_DEEPPS_GPIO               = 0x3,
    QL_RESET_SOURCE_DEEPPS_RTC                = 0x4,
    QL_RESET_SOURCE_DEEPPS_USB                = 0x5,
    QL_RESET_SOURCE_DEEPPS_TOUCH              = 0x6,
    QL_RESET_SOURCE_CRASH_ILLEGAL_JUMP        = 0x7,
    QL_RESET_SOURCE_CRASH_UNDEFINED           = 0x8,
    QL_RESET_SOURCE_CRASH_PREFETCH_ABORT      = 0x9,
    QL_RESET_SOURCE_CRASH_DATA_ABORT          = 0xa,
    QL_RESET_SOURCE_CRASH_UNUSED              = 0xb,
    QL_RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION = 0xc,
    QL_RESET_SOURCE_CRASH_MISALIGNED          = 0xd,
    QL_RESET_SOURCE_CRASH_ASSERT              = 0xe,
    QL_RESET_SOURCE_UNKNOWN                   = 0xff,
} ql_reset_source_status_e;

typedef enum {
    QL_SYS_SUCCESS = 0,
    QL_SYS_EXECUTE_ERR,
    QL_SYS_INVALID_PARAM_ERR,
} ql_sys_errcode_e;

/**
 * @brief
 *
 * @return ql_sys_reboot
 */
ql_sys_errcode_e ql_sys_reboot(void);

/**
 * @brief
 *
 * @return ql_reset_source_status_e
 */
ql_reset_source_status_e ql_get_reset_reason(void);

#ifdef __cplusplus
}
#endif
#endif
