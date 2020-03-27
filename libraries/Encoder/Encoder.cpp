//
// Created by pasha on 23.03.2020.
//
#include "Arduino.h"
#include "Encoder.h"

Encoder::Encoder(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw) {
    _pinClk = pinClk;
    _pinDt = pinDt;
    _pinSw = pinSw;
}

void Encoder::init(){
    pinMode (_pinClk, INPUT);
    pinMode (_pinDt, INPUT);
    pinMode (_pinSw, INPUT);
    _encoderDtState = digitalRead(_pinDt);
    _encoderClkState = digitalRead(_pinClk);
    _encoderSwState = digitalRead(_pinSw);
}

void Encoder::tick() {
    int dtState = digitalRead(_pinDt);
    int clkState = digitalRead(_pinClk);
    int swState = digitalRead(_pinSw);
    _turnRight = false;
    _turnLeft = false;
    _press = false;
    _release = false;
    if(dtState != _encoderDtState){
        if(clkState != dtState){
            _turnRight = true;
        } else {
            _turnLeft = true;
        }
        _encoderDtState = dtState;
    }

    if(_encoderClkState != clkState){
        _encoderClkState = clkState;
    }
    if(_encoderSwState != swState){
        if(1 == swState){
            _press = true;
        } else {
            _release = true;
        }
        _encoderSwState = swState;
    }
}

bool Encoder::isTurn() {
    return _turnLeft || _turnRight;
}

bool Encoder::isRight() {
    return _turnRight;
}

bool Encoder::isLeft() {
    return _turnLeft;
}

bool Encoder::isPress() {
    return _press;
}

bool Encoder::isRelease() {
    return _release;
}