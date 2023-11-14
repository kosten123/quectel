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

#include<quec_sys/ql_sys.h>

#define ql_printf bk_printf
/* **************NOTE*******************************************
* Use last 32 Bytes of DTCM to save sysinfo while system reset
* Only the BSP memeber can  change this area!!!
***************************************************************/
/*1. SAVE_JUMPAPP_ADDR For bootloader jump to app when occur nmi interrupt*/
#define SAVE_JUMPAPP_ADDR      (0x20007FE0)

/*2. START_TYPE_ADDR For CPU0-APP save reset reson*/
// #define START_TYPE_ADDR        (0x44000000 + 0x3 * 4)    //aon_pmu reg03
#define START_TYPE_ADDR        (0x20007FE4)

/*3. 0x20007FE8 Reserved now*/

/*4. 0x20007FEC Reserved now*/

/*5. RTC_TIME_SEC_ADDR For CPU0-APP save rtc seconds while system reset*/
#define RTC_TIME_SEC_ADDR      (0x20007FF0)

/*6. RTC_TIME_USEC_ADDR For CPU0-APP save rtc u seconds while system reset*/
#define RTC_TIME_USEC_ADDR     (0x20007FF4)

/*7. REBOOT_TAG_ADDR For CPU0-APP set reset tag in soft reboot*/
#define REBOOT_TAG_ADDR        (0x20007FF8)

/*8. PERSIST_MEMORY_ADDR For CPU0-APP save sysinfo while system reset*/
#define PERSIST_MEMORY_ADDR    (0x20007FFC)

#define REBOOT_TAG_REQ         (0xAA55AA55)   //4 bytes
#define MCAUSE_CAUSE_WATCHDOG  (0x1)

#define     CRASH_ILLEGAL_JUMP_VALUE      0xbedead00
#define     CRASH_UNDEFINED_VALUE         0xbedead01
#define     CRASH_PREFETCH_ABORT_VALUE    0xbedead02
#define     CRASH_DATA_ABORT_VALUE        0xbedead03
#define     CRASH_UNUSED_VALUE            0xbedead04

/***************************************************************************************************/
static uint32_t ql_s_start_type;
static uint32_t ql_s_misc_value_save;
static uint32_t ql_s_mem_value_save;

static void ql_persist_memory_init(void)
{
	*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)) = (uint32_t)CRASH_ILLEGAL_JUMP_VALUE;
}

static uint32_t ql_persist_memory_get(void)
{
	return (*((volatile uint32_t *)(PERSIST_MEMORY_ADDR)));
}

/**
 * @brief
 * 
 * @param start_type
 * @return ql_misc_get_start_type_str 
 */
static char *ql_misc_get_start_type_str(uint32_t start_type)
{
	switch (start_type) {
	case QL_RESET_SOURCE_POWERON:
		return "power on";

	case QL_RESET_SOURCE_REBOOT:
		return "software reboot";

	case QL_RESET_SOURCE_WATCHDOG:
		return "interrupt watchdog";

	case QL_RESET_SOURCE_DEEPPS_GPIO:
		return "deep sleep gpio";

	case QL_RESET_SOURCE_DEEPPS_RTC:
		return "deep sleep rtc";

	case QL_RESET_SOURCE_DEEPPS_TOUCH:
		return "deep sleep touch";

	case QL_RESET_SOURCE_CRASH_ILLEGAL_JUMP:
		return "illegal jump";

	case QL_RESET_SOURCE_CRASH_UNDEFINED:
		return "undefined";

	case QL_RESET_SOURCE_CRASH_PREFETCH_ABORT:
		return "prefetch abort";

	case QL_RESET_SOURCE_CRASH_DATA_ABORT:
		return "data abort";

	case QL_RESET_SOURCE_CRASH_UNUSED:
		return "unused";

	case QL_RESET_SOURCE_CRASH_ILLEGAL_INSTRUCTION:
		return "illegal instruction";

	case QL_RESET_SOURCE_CRASH_MISALIGNED:
		return "misaligned";

	case QL_RESET_SOURCE_CRASH_ASSERT:
		return "assert";

	case QL_RESET_SOURCE_DEEPPS_USB:
		return "deep sleep usb";

	case QL_RESET_SOURCE_UNKNOWN:
	default:
		// Chip power on the value of start address may not always be 0
		// return "unknown";
		return "power on";
	}
}

/**
 * @brief
 * 
 * @return ql_reset_reason_init 
 */
static uint32_t ql_reset_reason_init(void)
{
	#if (!CONFIG_SLAVE_CORE)
	uint32_t misc_value = REG_READ(START_TYPE_ADDR) >> 1;
	ql_s_start_type = misc_value;

	ql_s_misc_value_save = misc_value;
	ql_s_mem_value_save = ql_persist_memory_get();
	ql_persist_memory_init();

#endif
	return ql_s_start_type;
}
/*****************************************************************************************/

/**
 * @brief 
 * 
 * @return ql_sys_reboot 
 */
ql_sys_errcode_e ql_sys_reboot(void)
{
    bk_reboot();
    return QL_SYS_EXECUTE_ERR;
}

/**
 * @brief 
 * 
 * @return QL_RESET_SOURCE_STATUS 
 */
QL_RESET_SOURCE_STATUS ql_misc_get_reset_reason(void)
{
    ql_printf("restart reason: %s\r\n",ql_misc_get_start_type_str(ql_reset_reason_init()));
	#if CONFIG_DEEP_PS
	if(RESET_SOURCE_DEEPPS_GPIO == ql_s_start_type)
	{
		ql_printf("by gpio - %d\r\n", bk_misc_wakeup_get_gpio_num());
	}
	#endif
	ql_printf("regs - %x, %x, %x\r\n", ql_s_start_type, ql_s_misc_value_save,ql_s_mem_value_save);

	return ql_reset_reason_init();
}
