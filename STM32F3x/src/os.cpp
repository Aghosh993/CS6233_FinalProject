/*
 * os.c
 *
 *  Created on: May 5, 2013
 *      Author: aghosh01
 */

#include "os.h"

extern int current_process;
extern int num_processes_active, active_task;
extern process_block process_list[MAX_NUM_PROCESSES];

uint32_t read_current_msp_val(void)
{
	uint32_t val;
	asm volatile ("mrs r0, MSP\n\t");
	asm ("mov %[output], r0" : [output] "=r" (val));
	return val;
}

void context_push(void){
  uint32_t tmp;
  asm volatile ("mrs %[temp], msp\n\t"
      "push {r4-r11}\n\t"
      "MSR msp, %[temp]\n\t"  : [temp] "=r" (tmp) );
}

void context_pop(void){
  uint32_t tmp;
  asm volatile ("mrs %[temp], msp\n\t"
      "pop {r4-r11}\n\t"
      "MSR msp, %[temp]\n\t"  : [temp] "=r" (tmp) );
}

void processTable_init(process_block *pBlock)
{
	pBlock->process_stack_pointer = 0;
	pBlock->taskPointer = 0;
	pBlock->task_id = 1;
	int i = 0;
	for(i = 0; i < 32; ++i)
	{
		pBlock->task_state[i] = 0;
	}
	pBlock->delayMS = 0;
}

int create_task(void *(*tp)(void*))
{
	if(num_processes_active < MAX_NUM_PROCESSES)
	{
		processTable_init(&process_list[num_processes_active]);
		process_list[num_processes_active].taskPointer = tp;
		process_list[num_processes_active].task_id = num_processes_active+1;
		num_processes_active += 1;
		return process_list[num_processes_active - 1].task_id;
	}
	return -1;
}

void OS_init()
{
	current_process = 0;
	num_processes_active = 0;
	active_task = -1;
	*((uint32_t volatile *)0xE000EF34) = (1<<30); // Disable Cortex-M4F lazy stacking, so we don't save registers 2x
	timer_tick_ISR_init();
	__enable_irq();
}

void os_delay_ms(uint32_t millis)
{
	__disable_irq();
	process_list[active_task].delayMS = millis;
//	asm volatile ("mov %[outval], PC" : [outval] "=r" (process_list[active_task].process_pc));
//	process_list[active_task].process_pc += 0x04;
	yield_time();
	__enable_irq();
}
