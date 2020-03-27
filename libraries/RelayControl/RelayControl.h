//
// Created by pasha on 22.03.2020.
//
#include "Arduino.h"

#ifndef TEMP_RELAYCONTROL_H
#define TEMP_RELAYCONTROL_H

class RelayControl {
    public:
        RelayControl(uint8_t pin, uint8_t maxOnSec, uint8_t restSec);
        void enable();
        void forceOn(bool isOn);
        bool isForceOn();
        void disable();
        bool isEnabled();
        bool isOn();
        void loop();
        void setup();

    private:
        uint8_t _pin;
        uint8_t _maxOnSec;
        uint8_t _restSec;
        unsigned long _switchOnTime;
        unsigned long _switchOffTime;
        bool _isEnabled;
        bool _isOn;
        bool _isForceOn;
        void _calculateSwitch();
        void _controlRelay();
};

#endif //TEMP_RELAYCONTROL_H