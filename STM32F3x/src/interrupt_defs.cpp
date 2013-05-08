/*
 * interrupt_defs.cpp
 *
 *  Created on: Feb 19, 2013
 *      Author: root
 */

#include "common_headers.h"

#include "os.h"

// ISR definitions need to go here, to avoid C++ name-mangling

extern "C"
{
extern uint32_t msp_res;
extern int current_process;
extern process_block process_list[MAX_NUM_PROCESSES];
extern void nextTask();

extern uint32_t addrVal;

extern void *v;
extern void *(*p)(void*);

	void SVC_Handler(void)
	{
		++current_process;
		if(current_process >= MAX_NUM_PROCESSES)
		{
			current_process = 0;
		}

		addrVal = (uint32_t)(process_list[current_process].taskPointer);//0x8001841;
		asm volatile ("mov R10, %[something]\n\t" : [something] "=r" (addrVal));

		asm volatile ("bx R10\n\t");
	}

	void PendSV_Handler(void)
	{

		SCB->ICSR |= (1<<27);
		asm volatile ("svc 1");	// To kick us into SVC handler
	}

	void SysTick_Handler(void)
	{
		++current_process;
		if(current_process >= MAX_NUM_PROCESSES)
		{
			current_process = 0;
		}

		addrVal = (uint32_t)(process_list[current_process].taskPointer);//0x8001841;
		asm volatile ("mov R10, %[something]\n\t" : [something] "=r" (addrVal));

		asm volatile ("bx R10\n\t");
	}

	// Primarily for debug purposes:

	void HardFault_Handler(void)
	{
		while(1);
	}
	void MemManage_Handler(void)
	{
		while(1);
	}
	void BusFault_Handler(void)
	{
		while(1);
	}
	void UsageFault_Handler(void)
	{
		while(1);
	}
}


