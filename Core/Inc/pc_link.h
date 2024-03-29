/*
 * pc_link.h
 *
 *  Created on: Aug 7, 2022
 *      Author: marci
 */

#ifndef INC_PC_LINK_H_
#define INC_PC_LINK_H_


#include "stm32f4xx_hal.h"
#include "usbd_custom_hid_if.h"

//#define GAME_CONTROLLER_TEST


struct JoysticDataType
{
    int16_t X;
    int16_t Y;
    int16_t Z;
    int16_t Rz;
    uint16_t Rx;
    uint16_t Ry;
    uint16_t slider;
    uint16_t dial;
    uint8_t hat;
    uint32_t buttons;
};

class JoystickController
{
public:
    void sendReport();
    JoysticDataType data;
    static constexpr uint32_t ReportInterval = 10000U;    //joystick report sending interval
private:
    void setTestData();
    uint8_t _testCounter{0};
};


#endif /* INC_PC_LINK_H_ */
