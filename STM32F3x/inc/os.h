/*
 * os.h
 *
 *  Created on: May 5, 2013
 *      Author: aghosh01
 */

#ifndef OS_H_
#define OS_H_

#include <stdint.h>

#include "common_headers.h"

#define MAX_NUM_PROCESSES 10
#define DT_OS_TICK 1			// In milliseconds
#define NEXT_PROCESS_ANY -1

void timer_tick_ISR_init(void);

#define yield_time() asm volatile ("b SysTick_Handler\n\t")

static uint32_t *stack;

typedef struct process_block{
	int task_id;
	void *(*taskPointer)(void*);
	uint32_t process_stack_pointer;
	uint32_t process_pc;
	uint32_t task_state[32];
	int delayMS;
};

void OS_init();

uint32_t read_current_msp_val(void);

void processTable_init(process_block *pBlock);

int create_task(void *(*tp)(void*));
void delete_task(int task_id);

void os_delay_ms(uint32_t millis);

#endif /* OS_H_ */
