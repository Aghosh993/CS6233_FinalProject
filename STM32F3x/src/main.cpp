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

// Initialize all encoder data structures to zero:

int current_process;
process_block process_list[MAX_NUM_PROCESSES];
process_block pList;

int task1_iter, task2_iter;

uint32_t addrVal;

void task1(void)
{
	while(1)
	{
		++task1_iter;
		yield_time();
	}
}

void task2(void)
{
	while(1)
	{
		++task2_iter;
		yield_time();
	}
}

int main(void)
{
	SystemInit(); // Set up clocks/PLL/et. al

	UART1_init(); // Debug bridge

	task1_iter = 0;
	task2_iter = 0;

	process_list[0].taskPointer = task1;
	process_list[1].taskPointer = task2;

	OS_init(&pList);

	while(true);

	return 0; // We should never manage to get here...
}
