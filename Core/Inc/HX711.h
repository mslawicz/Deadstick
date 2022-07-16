/*
 * HX711.h
 *
 *  Created on: 16 lip 2022
 *      Author: marci
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "stm32f4xx_hal.h"

enum class HX711Mode : uint8_t
{
    A128 = 25,
    B32 = 26,
    A64 = 27
};

class HX711
{
public:
    HX711(GPIO_TypeDef* sckPort, uint16_t sckPin, GPIO_TypeDef* dataPort, uint16_t dataPin, HX711Mode mode);
    bool isDataReady() { return HAL_GPIO_ReadPin(_dataPort, _dataPin) == GPIO_PIN_RESET; }
    float getValue();
private:
    GPIO_TypeDef* _sckPort;
    uint16_t _sckPin;
    GPIO_TypeDef* _dataPort;
    uint16_t _dataPin;
    HX711Mode _mode;
    float _value{0.0F};       //last read value <-1,1>
};


#endif /* INC_HX711_H_ */
