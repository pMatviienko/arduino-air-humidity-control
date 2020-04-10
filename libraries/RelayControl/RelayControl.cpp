//
// Created by pasha on 22.03.2020.
//
#include "Arduino.h"
#include "RelayControl.h"

/**
 *
 * @param pin
 * @param maxOnSec
 * @param restSec
 */
RelayControl::RelayControl(uint8_t pin, uint8_t maxOnSec, uint8_t restSec) {
    _pin = pin;
    _maxOnSec = maxOnSec;
    _restSec = restSec;
    _isEnabled = false;
    _isOn = false;
    _switchOnTime = 0;
    _switchOffTime = 0;
}

void RelayControl::forceOn(bool isOn) {
    _isForceOn = isOn;
}

bool RelayControl::isForceOn() {
    return _isForceOn;
}

void RelayControl::enable() {
    _isEnabled = true;
}

void RelayControl::disable() {
    _isEnabled = false;
}

bool RelayControl::isEnabled() {
    return _isEnabled;
}

bool RelayControl::isOn() {
    return _isOn;
}

void RelayControl::setup() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, HIGH);
}

void RelayControl::loop() {
    _calculateSwitch();
    _controlRelay();
}

void RelayControl::_calculateSwitch(){
    unsigned long currentTimeSec = millis() / 1000;
    if(_isEnabled){
        if(_switchOnTime == 0 && _switchOffTime == 0){
            _switchOnTime = currentTimeSec;
            _isOn = true;
        } else if(_switchOnTime != 0){
            if(currentTimeSec - _switchOnTime >= _maxOnSec){
                _switchOnTime = 0;
                _switchOffTime = currentTimeSec;
                _isOn = false;
            } else {
                _isOn = true;
            }
        } else {
            if(currentTimeSec - _switchOffTime >= _restSec){
                _switchOffTime = 0;
                _switchOnTime = currentTimeSec;
                _isOn = true;
            } else {
                _isOn = false;
            }
        }
    } else {
        _switchOnTime = 0;
        _switchOffTime = 0;
        _isOn = false;
    }
}

void RelayControl::_controlRelay(){
    if(_isOn || _isForceOn){
        digitalWrite(_pin, LOW);
//        Serial.println("Relay on");
    } else {
        digitalWrite(_pin, HIGH);
    }
}
