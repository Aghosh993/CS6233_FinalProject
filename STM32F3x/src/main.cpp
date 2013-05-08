#include "stm32f30x_adc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_dma.h"

#include "stm32f3_discovery_l3gd20.h"

#include "encoder.h"
#include "pwm.h"
#include "debug.h"

#include <stdio.h>
#include <stdint.h>
#include <math.h>

/*
    This program (program referring to all source and header files in this repository (repository
    referring to https://github.com/Aghosh993/Trinity2013/tree) not containing code covered
    under the terms of STMicroelectronic's Liberty v2 License) is free software: you can redistribute
    it and/or modify it ("it" refers to all sections of this code as covered in the aforementioned way)
    under the terms of the GNU General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program (please refer to the file COPYING in the root of
    the repository).  If not, see <http://www.gnu.org/licenses/>.

    (c) Abhimanyu Ghosh, Polytechnic Institute of NYU (2013-2014)
 */

/*
 * All other code is covered by the following terms:
 *
 *   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
 */

/*
 *
 * Purpose: CS6233 Final Project (Implementation of a simple round-robin scheduler and task switcher
 * 			using the STM32 SysTick interrupt)
 *
 */

volatile int led_iter;
volatile int led_matrix[7] = {GPIO_Pin_9, GPIO_Pin_8, GPIO_Pin_15, GPIO_Pin_13,
										GPIO_Pin_12, GPIO_Pin_11, GPIO_Pin_10};

int current_process;
process_block process_list[MAX_NUM_PROCESSES];
int num_processes_active, active_task, nextProc;

int task1_iter, task2_iter, task3_iter, pc;

uint32_t addrVal;

void task1(void)
{
	while(1)
	{
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_SET);
		while(task1_iter < 1000000)
		{
			++task1_iter;
		}
		task1_iter = 0;
		GPIO_WriteBit(GPIOE, GPIO_Pin_8, Bit_RESET);
		yield_time();
	}
}

void task2(void)
{
	while(1)
	{
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_SET);
		while(task2_iter < 1000000)
		{
			++task2_iter;
		}
		task2_iter = 0;
		GPIO_WriteBit(GPIOE, GPIO_Pin_9, Bit_RESET);
		yield_time();
	}
}

void task3(void)
{
	while(1)
	{
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_SET);
		while(task3_iter < 1000000)
		{
			++task3_iter;
		}
		task3_iter = 0;
//		asm volatile ("mov %[outval], PC" : [outval] "=r" (process_list[active_task].process_pc));
//		process_list[active_task].process_pc += sizeof(os_delay_ms);
//		os_delay_ms(200);
//		while(task3_iter < 1000)
//		{
//			++task3_iter;
//		}
//		task3_iter = 0;
		GPIO_WriteBit(GPIOE, GPIO_Pin_15, Bit_RESET);
		yield_time();
	}
}

void GPIO_init(void);

int main(void)
{
	SystemInit(); // Set up clocks/PLL/et. al

	UART1_init(); // Debug bridge
	GPIO_init();

	task1_iter = 0;
	task2_iter = 0;

//	process_list[0].taskPointer = task1;
//	process_list[1].taskPointer = task2;

	OS_init();
	create_task(task1);
	create_task(task2);
	create_task(task3);

	yield_time(); // Kick us into the first task

	while(true);

	return 0; // We should never manage to get here...
}

void timer_tick_ISR_init(void)
{
	NVIC_InitTypeDef nv;
	TIM_TimeBaseInitTypeDef TIM17_init;

	nv.NVIC_IRQChannel = TIM1_TRG_COM_TIM17_IRQn;
	nv.NVIC_IRQChannelPreemptionPriority = 0;
	nv.NVIC_IRQChannelSubPriority = 0;
	nv.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nv);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

	/*
	 * UPDATE_FREQUENCY = TIM_CLK/[(PRESCALER+1)(ARR+1)(REPCOUNTER+1)]
	 * 	  Prescaler -> PRESCALER
	 * 	  Period -> ARR
	 * 	  RepetitionCounter -> REPCOUNTER
	 */

	TIM17_init.TIM_Period = (10*DT_OS_TICK)-1;
	TIM17_init.TIM_Prescaler = 7199;
	TIM17_init.TIM_RepetitionCounter = 0;
	TIM17_init.TIM_ClockDivision = 0;
	TIM17_init.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM17, &TIM17_init);

	TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM17, ENABLE);
}

void GPIO_init(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);

	GPIO_InitTypeDef a;

	a.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
	a.GPIO_Mode = GPIO_Mode_OUT;
	a.GPIO_PuPd = GPIO_PuPd_NOPULL;
	a.GPIO_OType = GPIO_OType_PP;
	a.GPIO_Speed = GPIO_Speed_Level_2;

	GPIO_Init(GPIOE, &a);
}
