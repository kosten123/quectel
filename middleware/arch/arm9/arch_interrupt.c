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

#include "arch_interrupt.h"


static int_group_isr_t s_fiq_isr_fn = 0;
static int_group_isr_t s_irq_isr_fn = 0;


void arch_register_group_isr(uint32_t group, int_group_isr_t isr)
{
	if(group == ARM_INT_GROUP_FIQ)
		s_fiq_isr_fn = isr;
	else
		s_irq_isr_fn = isr;
}


void intc_irq(void)
{
	s_irq_isr_fn();
}

void intc_fiq(void)
{
	s_fiq_isr_fn();
}


bk_err_t arch_isr_entry_init(void)
{
	*((volatile uint32_t *)0x400000) = (uint32_t)&do_irq;
	*((volatile uint32_t *)0x400004) = (uint32_t)&do_fiq;
	*((volatile uint32_t *)0x400008) = (uint32_t)&do_swi;
	*((volatile uint32_t *)0x40000c) = (uint32_t)&do_undefined;
	*((volatile uint32_t *)0x400010) = (uint32_t)&do_pabort;
	*((volatile uint32_t *)0x400014) = (uint32_t)&do_dabort;
	*((volatile uint32_t *)0x400018) = (uint32_t)&do_reserved;

	return BK_OK;
}


