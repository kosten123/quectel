/*
 * Copyright (c) 2021-2022 Beken Corporation
 * All rights reserved.
 *
 */

#include "core_v5.h"

#define MTIMER                     (0xE6000000)
#define MTIMERCMP                  (0xE6000008)

.global  arch_int_disable
.global  arch_disable_irq
.global  arch_int_lock

.global  arch_int_enable
.global  arch_enable_irq
.global  arch_int_unlock

.global  arch_int_restore
.global  arch_fence
.global  arch_atomic_clear
.global  arch_atomic_set

.global  riscv_get_mtimer
.global  riscv_set_mtimercmp
.global  riscv_get_cycle
.global  riscv_get_instruct_cnt
.global  arch_get_int_status

//.section .text
.section .itcm_sec_code, "ax"


/*
 * u32 arch_int_unlock(void);
 */
arch_int_unlock:
    csrr    a0, mstatus           // return value
	csrsi  mstatus, MSTATUS_MIE
    ret

/*
 * u32 arch_int_disable(void);
 */
arch_int_lock:
arch_disable_irq:
arch_int_disable:
	csrrci	a0, mstatus, MSTATUS_MIE
    ret

/*
 * void arch_int_enable(void);
 */
arch_enable_irq:
arch_int_enable:
    csrsi  mstatus, MSTATUS_MIE
    ret

/*
 * void arch_int_restore(u32 int_flag);
 */
arch_int_restore:
    csrw mstatus, a0
    ret

/*
 * u32 arch_get_int_status(void);
 */
arch_get_int_status:
    csrr    a0, mip           // return value
    ret


/*
 * void arch_fence(void);
 */
arch_fence:
	fence iorw, iorw
	ret

#if 0
/*
 * void arch_atomic_clear(u32 * lock_addr);
 */
arch_atomic_clear:
	amoswap.w.rl x0, x0, (a0)
	ret

/*
 * u32 arch_atomic_set(u32 * lock_addr);
 */
arch_atomic_set:
	addi t0, x0, 1
//swap_again: 
	amoswap.w.aq t0, t0, (a0)
	c.mv  a0, t0
//	bnez t0, swap_again
	ret

#endif

/*
 * u64 riscv_get_mtimercmp(void);
 */
riscv_get_mtimercmp:
	li t0, MTIMERCMP
	lw a0, 0(t0)
	lw a1, 4(t0)
	ret

/*
 * u64 riscv_get_mtimer(void);
 */
riscv_get_mtimer:
	li t0, MTIMER
read_mtimer: 
	lw a1, 4(t0)
	lw a0, 0(t0)
	lw t1, 4(t0)
	bne a1, t1, read_mtimer
	ret

/*
 * void riscv_set_mtimercmp(u64 new_time);
 */
riscv_set_mtimercmp:
	li t0, MTIMERCMP
	li t1, -1
	sw t1, 4(t0)
	fence
	sw a0, 0(t0)
	sw a1, 4(t0)
	ret

/*
 * u64 riscv_get_cycle(void);
 */
riscv_get_cycle:
read_cycle: 
	csrrs a1, mcycleh, x0
	csrrs a0, mcycle, x0
	csrrs t0, mcycleh, x0
	bne a1, t0, read_cycle
	ret

/*
 * u64 riscv_get_instruct_cnt(void);
 */
riscv_get_instruct_cnt:
read_instr_cnt: 
	csrrs a1, minstreth, x0
	csrrs a0, minstret, x0
	csrrs t0, minstreth, x0
	bne a1, t0, read_instr_cnt
	ret

