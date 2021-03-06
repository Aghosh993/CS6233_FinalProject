/*
 * common_headers.h
 *
 *  Created on: Feb 19, 2013
 *      Author: root
 */

#ifndef COMMON_HEADERS_H_
#define COMMON_HEADERS_H_

#define DT_IMU 				40

#define K_ULTRASONIC 		0.001130f
#define UV_NUMSAMPLES 		300
#define UV_THRESHOLD 		540

#define FIREFIGHT_TIMEOUT	10.0f // 10 seconds

#define ST_READY 			-1
#define ST_WANDER 			0
#define ST_HOMING 			1
#define ST_FIREFIGHT 		2
#define ST_CANDLE_BLOWOUT 	3
#define ST_DONE 			4

#include "stm32f30x.h"
#include "stm32f30x_adc.h"
#include "stm32f30x_gpio.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_misc.h"
#include "stm32f30x_tim.h"
#include "stm32f30x_usart.h"

#include "os.h"

#endif /* COMMON_HEADERS_H_ */
