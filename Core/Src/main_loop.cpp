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
//#include "pc_link.h"
//#include "convert.h"
//#include "constant.h"
#include "logger.h"
//#include "monitor.h"
#include "HX711.h"
#include <iostream>

ADC_HandleTypeDef* pHadc;    //pointer to ADC object
uint16_t adcConvBuffer[MAX_ADC_CH]; //buffer for ADC conversion results


#ifdef MONITOR
//int16_t monitor_elevTrim;
#endif

void mainLoop()
{
    constexpr uint32_t HeartbeatPeriod = 500000;
    Timer statusLedTimer;
//    Timer gameCtrlTimer;

    HX711 forceSensorLeft(HX711_1_clk_GPIO_Port, HX711_1_clk_Pin, HX711_1_data_GPIO_Port, HX711_1_data_Pin, HX711Mode::A128);

    LOG_ALWAYS("Deadstick v0.1");

    //assign system LEDs
    GPIO_TypeDef* heartbeatLedPort = LD1_GPIO_Port; //green LED
    uint16_t heartbeatLedPin = LD1_Pin;

//    GameController gameController;  //USB link-to-PC object (class custom HID - joystick)

    Timer::start(pTimerHtim);

    float forceValue{0};

    /* main forever loop */
    while(true)
    {
        if(statusLedTimer.hasElapsed(HeartbeatPeriod))
        {
            HAL_GPIO_TogglePin(heartbeatLedPort, heartbeatLedPin);
            statusLedTimer.reset();
            std::cout << "\r" << forceValue << "     ";
        }

        if(forceSensorLeft.isDataReady())   //XXX test
        {
            forceValue = forceSensorLeft.getValue();
        }

//        if(gameCtrlTimer.hasElapsed(GameController::ReportInterval))
//        {
//            gameController.sendReport();
//            gameCtrlTimer.reset();
//        }
    }
}



