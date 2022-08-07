/*
 * main_loop.cpp
 *
 *  Created on: 16 lip 2022
 *      Author: marci
 */


#include "main.h"
#include "main_loop_api.h"
#include "main_loop.h"
#include "timer.h"
#include "pc_link.h"
#include "convert.h"
#include "constant.h"
#include "logger.h"
#include "monitor.h"
#include "HX711.h"
#include <iostream>

ADC_HandleTypeDef* pHadc;    //pointer to ADC object
uint16_t adcConvBuffer[MAX_ADC_CH]; //buffer for ADC conversion results

#ifdef MONITOR
int16_t monitor_forceXL;
int16_t monitor_forceXR;
int16_t monitor_forceX;
#endif

void mainLoop()
{
    constexpr uint32_t HeartbeatPeriod = 500000;
    Timer statusLedTimer;
    Timer joyCtrlTimer;

    HX711 forceSensorLeft(HX711_1_clk_GPIO_Port, HX711_1_clk_Pin, HX711_1_data_GPIO_Port, HX711_1_data_Pin, HX711Mode::A64);
    HX711 forceSensorRight(HX711_2_clk_GPIO_Port, HX711_2_clk_Pin, HX711_2_data_GPIO_Port, HX711_2_data_Pin, HX711Mode::A64);

    LOG_ALWAYS("Deadstick v0.1");

    //assign system LEDs
    GPIO_TypeDef* heartbeatLedPort = LD1_GPIO_Port; //green LED
    uint16_t heartbeatLedPin = LD1_Pin;

    JoystickController joystickController;  //USB link-to-PC object (class custom HID - joystick)

    Timer::start(pTimerHtim);

    float forceLeftValue{0};
    float forceRightValue{0};
    float forceValue{0};
    float offset{0};

    /* main forever loop */
    while(true)
    {
        if(statusLedTimer.hasElapsed(HeartbeatPeriod))
        {
            HAL_GPIO_TogglePin(heartbeatLedPort, heartbeatLedPin);
            statusLedTimer.reset();
            //std::cout << "\r" << forceLeftValue << " , " << forceRightValue << "     ";
        }

        bool dataUpdated = false;

        if(forceSensorLeft.isDataReady())   //XXX test
        {
            forceLeftValue = forceSensorLeft.getValue();
            dataUpdated = true;
        }
        if(forceSensorRight.isDataReady())   //XXX test
        {
            forceRightValue = forceSensorRight.getValue();
            dataUpdated = true;
        }

        if(dataUpdated)
        {
            forceValue = 2.0F * (forceLeftValue - forceRightValue);
        }

        if(HAL_GPIO_ReadPin(USER_Btn_GPIO_Port, USER_Btn_Pin) == 1)
        {
            offset = forceValue;
        }

        if(joyCtrlTimer.hasElapsed(JoystickController::ReportInterval))
        {
            joystickController.data.Y = scale<float, int16_t>(-1.0F, 1.0F, forceValue - offset, -Max15bit, Max15bit);

            joystickController.sendReport();
            joyCtrlTimer.reset();
        }

#ifdef MONITOR
        monitor_forceXL = static_cast<int16_t>(forceLeftValue * 1000);
        monitor_forceXR = static_cast<int16_t>(forceRightValue * 1000);
        monitor_forceX = static_cast<int16_t>((forceValue - offset) * 1000);
#endif
    }
}



