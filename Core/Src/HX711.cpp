/*
 * HX711.cpp
 *
 *  Created on: 16 lip 2022
 *      Author: marci
 */


#include "HX711.h"
#include "convert.h"
#include "constant.h"

HX711::HX711(GPIO_TypeDef* sckPort, uint16_t sckPin, GPIO_TypeDef* dataPort, uint16_t dataPin, HX711Mode mode) :
    _sckPort(sckPort),
    _sckPin(sckPin),
    _dataPort(dataPort),
    _dataPin(dataPin),
    _mode(mode)
{

}

//get the last data from HX711 sensor
//if new data is ready - read it
//otherwise return the previously read data
float HX711::getValue()
{
    constexpr uint8_t Bit24 = 24U;
    constexpr uint8_t Bit8 = 8U;
    constexpr uint8_t HalfByteValue = 0x80U;
    uint8_t pulseCounter{0};
    uint32_t dataBuffer{0};
    uint32_t newData{0};

    if(!isDataReady())
    {
        //new data in not available - return the last value
        return _value;
    }

    do
    {
        {
            //this section must not be interrupted
            __disable_irq();
            HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PinState::GPIO_PIN_SET);
            if(HAL_GPIO_ReadPin(_dataPort, _dataPin) == GPIO_PinState::GPIO_PIN_SET)
            {
                dataBuffer |= 0x01U;
            }
            HAL_GPIO_WritePin(_sckPort, _sckPin, GPIO_PinState::GPIO_PIN_RESET);
            __enable_irq();
        }

        if(++pulseCounter == Bit24)
        {
            //all bit have been read
            newData = (dataBuffer << Bit8) | HalfByteValue;
        }
        else
        {
            dataBuffer <<= 1U;
        }
    } while(pulseCounter < static_cast<uint8_t>(_mode));

    auto int32Value = static_cast<int32_t>(newData);
    _value = static_cast<float>(int32Value) / Max31bitF;    //scaled to <-1,1>
    return _value;
}

