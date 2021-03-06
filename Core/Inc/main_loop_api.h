/*
 * main_loop_api.h
 *
 *  Created on: 16 lip 2022
 *      Author: marci
 */

#ifndef INC_MAIN_LOOP_API_H_
#define INC_MAIN_LOOP_API_H_

#include "stm32f4xx_hal.h"

#define MAX_ADC_CH  16

extern TIM_HandleTypeDef* pTimerHtim;  //pointer to htim object instance used by Timer class
extern ADC_HandleTypeDef* pHadc;    //pointer to ADC object
extern uint16_t adcConvBuffer[MAX_ADC_CH]; //buffer for ADC conversion results

#ifdef __cplusplus
extern "C"
{
#endif

void mainLoop();

#ifdef __cplusplus
}
#endif

#endif /* INC_MAIN_LOOP_API_H_ */
