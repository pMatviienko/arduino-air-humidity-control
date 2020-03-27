//
// Created by pasha on 22.03.2020.
//

#include "Arduino.h"
#include <Wire.h>

#ifndef TEMP_EPROM_H
#define TEMP_EPROM_H


class Eprom {
    public:
        Eprom(uint8_t romAddress);
        void setTargetHumidity(float value);
        void setTargetTemp(float value);
        void setTempTolerance(float value);
        void setHumidityTolerance(float value);
        float getTargetHumidity();
        float getTargetTemp();
        float getTempTolerance();
        float getHumidityTolerance();
        void storeValues();
        void readValues();

    private:
        uint8_t _romAddress;
        word _humidityAddress;
        word _tempAddress;
        word _humidityToleranceAddress;
        word _tempToleranceAddress;
        float _targetHumidity;
        float _targetTemp;
        float _tempTolerance;
        float _humidityTolerance;
};


#endif //TEMP_EPROM_H
