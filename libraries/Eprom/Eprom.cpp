//
// Created by pasha on 22.03.2020.
//

#include "Arduino.h"
#include "Eprom.h"
#include <Wire.h>

Eprom::Eprom(uint8_t romAddress) {
    _romAddress = romAddress;
    _tempAddress = 0;
    _humidityAddress = 6;
    _humidityToleranceAddress = 12;
    _tempToleranceAddress = 18;
    _targetHumidity = 45.00;
    _targetTemp = 10.00;
    _tempTolerance = 3.00;
    _humidityTolerance = 3.00;
}

void Eprom::setTargetHumidity(float value){
    _targetHumidity = value;
}

void Eprom::setTargetTemp(float value) {
    _targetTemp = value;
}

void Eprom::setHumidityTolerance(float value) {
    _humidityTolerance = value;
}

void Eprom::setTempTolerance(float value) {
    _tempTolerance = value;
}

float Eprom::getTargetHumidity() {
    return _targetHumidity;
}

float Eprom::getTargetTemp() {
    return _targetTemp;
}

float Eprom::getHumidityTolerance() {
    return _humidityTolerance;
}

float Eprom::getTempTolerance() {
    return _tempTolerance;
}

void Eprom::storeValues() {
    char data_send[6];
    byte status;

    dtostrf(_targetTemp, 6, 2, data_send);
    Wire.beginTransmission(_romAddress);
    Wire.write(_tempAddress >> 8);
    Wire.write(_tempAddress & 0xFF);
    Wire.write(data_send);
    status= Wire.endTransmission();
    delay(10);

    dtostrf(_targetHumidity, 6, 2, data_send);
    Wire.beginTransmission(_romAddress);
    Wire.write(_humidityAddress >> 8);
    Wire.write(_humidityAddress & 0xFF);
    Wire.write(data_send);
    status= Wire.endTransmission();
    delay(10);

    dtostrf(_humidityTolerance, 6, 2, data_send);
    Wire.beginTransmission(_romAddress);
    Wire.write(_humidityToleranceAddress >> 8);
    Wire.write(_humidityToleranceAddress & 0xFF);
    Wire.write(data_send);
    status= Wire.endTransmission();
    delay(10);

    dtostrf(_tempTolerance, 6, 2, data_send);
    Wire.beginTransmission(_romAddress);
    Wire.write(_tempToleranceAddress >> 8);
    Wire.write(_tempToleranceAddress & 0xFF);
    Wire.write(data_send);
    status= Wire.endTransmission();
    delay(10);
}

void Eprom::readValues() {
    byte status;
    Wire.begin();

    Wire.beginTransmission(_romAddress);
    Wire.write(_tempAddress >> 8);
    Wire.write(_tempAddress & 0xFF);
    status = Wire.endTransmission();
    Wire.requestFrom(_romAddress, (byte) 6);
    char rawTemp[6];
    for (int i=0 ; i<6 ;i++) {
        if (Wire.available()){
            rawTemp[i] = Wire.read();
        }
    }
    _targetTemp = atof(rawTemp);

    Wire.beginTransmission(_romAddress);
    Wire.write(_humidityAddress >> 8);
    Wire.write(_humidityAddress & 0xFF);
    status = Wire.endTransmission();
    Wire.requestFrom(_romAddress, (byte) 6);
    char rawHum[6];
    for (int i=0 ; i<6 ;i++) {
        if (Wire.available()){
            rawHum[i] = Wire.read();
        }
    }
    _targetHumidity = atof(rawHum);

    Wire.beginTransmission(_romAddress);
    Wire.write(_humidityToleranceAddress >> 8);
    Wire.write(_humidityToleranceAddress & 0xFF);
    status = Wire.endTransmission();
    Wire.requestFrom(_romAddress, (byte) 6);
    char rawHumTol[6];
    for (int i=0 ; i<6 ;i++) {
        if (Wire.available()){
            rawHumTol[i] = Wire.read();
        }
    }
    _humidityTolerance = atof(rawHumTol);

    Wire.beginTransmission(_romAddress);
    Wire.write(_tempToleranceAddress >> 8);
    Wire.write(_tempToleranceAddress & 0xFF);
    status = Wire.endTransmission();
    Wire.requestFrom(_romAddress, (byte) 6);
    char rawTmpTol[6];
    for (int i=0 ; i<6 ;i++) {
        if (Wire.available()){
            rawTmpTol[i] = Wire.read();
        }
    }
    _tempTolerance = atof(rawTmpTol);
}