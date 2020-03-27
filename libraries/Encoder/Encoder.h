//
// Created by pasha on 23.03.2020.
//

#include "Arduino.h"

#ifndef TEMP_ENCODER_H
#define TEMP_ENCODER_H


class Encoder {
public:
    Encoder(uint8_t pinClk, uint8_t pinDt, uint8_t pinSw);
    void init();
    void tick();
    bool isTurn();
    bool isRight();
    bool isLeft();
    bool isPress();
    bool isRelease();

private:
    uint8_t _pinClk;
    uint8_t _pinDt;
    uint8_t _pinSw;
    int _encoderDtState;
    int _encoderClkState;
    int _encoderSwState;
    bool _press;
    bool _release;
    bool _turnRight;
    bool _turnLeft;
};


#endif //TEMP_ENCODER_H
