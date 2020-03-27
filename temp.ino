#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <USBAPI.h>
#include <RelayControl.h>
#include <Eprom.h>
#include "Encoder.h"


#define EEPROM_ADDRESS 0x50
#define LCD_ADDRESS 0x27
#define pinDht 5
#define pin_CLK 4
#define pin_DT  2
#define pin_BTN 3

#define LCD_TIME 20;

#define VENT_MAX_ON_SEC 30
#define VENT_REST_SEC 15
#define PIN_RELAY_VENT 12
#define PIN_RELAY_HEATER 11

float outerHumidity = 0.00;
float outerTemp = 0.00;
bool outerMeasureError = false;

DHT dht(pinDht, DHT11);
RelayControl ventRelay(PIN_RELAY_VENT, VENT_MAX_ON_SEC, VENT_REST_SEC);
RelayControl heaterRelay(PIN_RELAY_HEATER, VENT_MAX_ON_SEC, VENT_REST_SEC);
Eprom mem(EEPROM_ADDRESS);
LiquidCrystal_I2C lcd(LCD_ADDRESS,16,2);
Encoder encoder(pin_CLK, pin_DT, pin_BTN);

unsigned long lastMeasureTime = 0;
void airControl(){
    unsigned long currentTimeSec = millis() / 1000;
    if(currentTimeSec - lastMeasureTime >= 3){
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (!isnan(h) && !isnan(t)) {
            outerHumidity = h;
            outerTemp = t;
            outerMeasureError = false;
        } else {
            outerMeasureError = true;
        }

        lastMeasureTime = currentTimeSec;

        Serial.println("-----------------------------------------------------------");
        Serial.println("Measured humidity:");
        Serial.println(outerHumidity);
        Serial.println("Target humidity:");
        Serial.println(mem.getTargetHumidity());
        Serial.println("Humidity tolerance:");
        Serial.println(mem.getHumidityTolerance());
        Serial.println("Vent state");
        Serial.println(ventRelay.isEnabled() ? "Enabled" : "Disabled");
        Serial.println("-----------------------------------------------------------");

        if(outerHumidity  > (mem.getTargetHumidity() + mem.getHumidityTolerance())){
            ventRelay.enable();
        };
        if(outerHumidity < (mem.getTargetHumidity() - mem.getHumidityTolerance())){
            ventRelay.disable();
        }

        if(outerTemp < (mem.getTargetTemp() - mem.getTempTolerance())){
            heaterRelay.enable();
        }
        if(outerTemp > (mem.getTargetTemp() + mem.getTempTolerance())){
            heaterRelay.disable();
        }
    }
}


byte heatIcon[8] = {
        0b00000,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b10101,
        0b00000
};
byte ventIcon[8] = {
        0b00000,
        0b00000,
        0b11001,
        0b01011,
        0b00100,
        0b11010,
        0b10011,
        0b00000
};

byte degreeIcon[8] = {
        0b11100,
        0b10100,
        0b11100,
        0b00000,
        0b01111,
        0b01000,
        0b01000,
        0b01111
};

byte arrowLeftIcon[8] = {
        0b00010,
        0b00100,
        0b01000,
        0b11110,
        0b11110,
        0b01000,
        0b00100,
        0b00010
};

byte arrowRightIcon[8] = {
        0b01000,
        0b00100,
        0b00010,
        0b01111,
        0b01111,
        0b00010,
        0b00100,
        0b01000
};


int backlightOnSeconds = 30;
int lcdMode = 0;
bool lcdInEditMode = false;
bool isLcdOn = true;
unsigned long lastInteractionTime = millis() / 1000;
void handleLcd()
{
    if(isLcdOn){
        switch (lcdMode){
            case 1:
                lcd.setCursor(0,0);
                lcd.print("Target Humidity");
                if(lcdInEditMode){
                    lcd.setCursor(0,1);
                    lcd.write((uint8_t)3);
                    lcd.setCursor(1,1);
                    lcd.write((uint8_t)4);
                } else {
                    lcd.setCursor(0,1);
                    lcd.print("  ");
                }
                lcd.setCursor(5,1);
                lcd.print(mem.getTargetHumidity());
                break;
            case 2:
                lcd.setCursor(0,0);
                lcd.print("Target Temp");
                if(lcdInEditMode){
                    lcd.setCursor(0,1);
                    lcd.write((uint8_t)3);
                    lcd.setCursor(1,1);
                    lcd.write((uint8_t)4);
                } else {
                    lcd.setCursor(0,1);
                    lcd.print("  ");
                }
                lcd.setCursor(5,1);
                lcd.print(mem.getTargetTemp());
                break;
            case 3:
                lcd.setCursor(0,0);
                lcd.print("Hum Tolerance");
                if(lcdInEditMode){
                    lcd.setCursor(0,1);
                    lcd.write((uint8_t)3);
                    lcd.setCursor(1,1);
                    lcd.write((uint8_t)4);
                } else {
                    lcd.setCursor(0,1);
                    lcd.print("  ");
                }
                lcd.setCursor(5,1);
                lcd.print(mem.getHumidityTolerance());
                break;
            case 4:
                lcd.setCursor(0,0);
                lcd.print("Temp Tolerance");
                if(lcdInEditMode){
                    lcd.setCursor(0,1);
                    lcd.write((uint8_t)3);
                    lcd.setCursor(1,1);
                    lcd.write((uint8_t)4);
                } else {
                    lcd.setCursor(0,1);
                    lcd.print("  ");
                }
                lcd.setCursor(5,1);
                lcd.print(mem.getTempTolerance());
                break;
            case 5:
                lcd.setCursor(0,0);
                lcd.print("Force Vent");
                lcd.setCursor(4,1);
                if(ventRelay.isForceOn()){
                    lcd.print("Force On");
                } else {
                    lcd.print("Normal");
                }
                break;
            case 6:
                lcd.setCursor(0,0);
                lcd.print("Force Heat");
                lcd.setCursor(4,1);
                if(heaterRelay.isForceOn()){
                    lcd.print("Force On");
                } else {
                    lcd.print("Normal");
                }
                break;
            default:
                lcdInEditMode = false;
                if(outerMeasureError){
                    lcd.setCursor(0,0);
                    lcd.print("DHT Error");
                } else {
                    lcd.setCursor(0,0);
                    lcd.print("H: ");
                    lcd.print(outerHumidity);
                    lcd.print (" %");
                    if(ventRelay.isOn() || ventRelay.isForceOn()){
                        lcd.setCursor(15,0);
                        lcd.write((uint8_t)1);
                    } else {
                        lcd.setCursor(15,0);
                        lcd.print(" ");
                    }
                    lcd.setCursor(0,1);
                    lcd.print("T: ");
                    lcd.print(outerTemp);
                    lcd.print(' ');
                    lcd.write((uint8_t)2);
                    if(heaterRelay.isOn() || heaterRelay.isForceOn()){
                        lcd.setCursor(15,1);
                        lcd.write((uint8_t)0);
                    } else {
                        lcd.setCursor(15,1);
                        lcd.print(" ");
                    }
                }
                break;
        }
    }

    if(millis() > (lastInteractionTime + backlightOnSeconds * 1000)) {
        if (isLcdOn) {
            lcd.clear();
            lcd.noBacklight();
            lcd.off();
            isLcdOn = false;
            lcdInEditMode = false;
            lcdMode = 0;
        }
    }
    if(encoder.isRelease() || encoder.isPress() || encoder.isTurn()){
        if(!isLcdOn){
            isLcdOn = true;
            lcdInEditMode = false;
            lcd.on();
            lcd.backlight();
        }
        lcd.clear();
        lastInteractionTime = millis();
    }
}

void handleEncoder(){
    if(!lcdInEditMode){
        if(encoder.isRight()){
            lcdMode = lcdMode - 1;
            if(lcdMode > 6){
                lcdMode = 0;
            }
            lcd.clear();
        } else if(encoder.isLeft()){
            lcdMode = lcdMode + 1;
            if(lcdMode < 0){
                lcdMode = 6;
            }
            lcd.clear();
        }

        if(encoder.isPress() && lcdMode > 0 && lcdMode < 5){
            lcdInEditMode = true;
        } else if(encoder.isPress() && lcdMode == 5){
            ventRelay.forceOn(!ventRelay.isForceOn());
        } else if(encoder.isPress() && lcdMode == 6){
            heaterRelay.forceOn(!heaterRelay.isForceOn());
        }
    } else {
        float targetHumidity;
        float targetTemp;
        float humidityTolerance;
        float tempTolerance;
        switch (lcdMode){
            case 1:
                targetHumidity = mem.getTargetHumidity();
                if(encoder.isRight()){
                    targetHumidity = targetHumidity + 0.5;
                    Serial.println(targetHumidity);
                } else if (encoder.isLeft()){
                    targetHumidity = targetHumidity - 0.5;
                    Serial.println(targetHumidity);
                }
                if(targetHumidity > 80.00){
                    targetHumidity = 10.00;
                } else if(targetHumidity < 20.00){
                    targetHumidity = 80.00;
                }
                mem.setTargetHumidity(targetHumidity);
                break;
            case 2:
                targetTemp = mem.getTargetTemp();
                if(encoder.isRight()) {
                    targetTemp = targetTemp + 0.5;
                } else if (encoder.isLeft()){
                    targetTemp = targetTemp - 0.5;
                }
                if (targetTemp > 40) {
                    targetTemp = 1.00;
                } else if(targetTemp < 1){
                    targetTemp = 40.00;
                }
                mem.setTargetTemp(targetTemp);
                break;
            case 3:
                humidityTolerance = mem.getHumidityTolerance();
                if(encoder.isRight()){
                    humidityTolerance = humidityTolerance + 0.5;
                    if(humidityTolerance > 10){
                        humidityTolerance = 1;
                    }
                } else if (encoder.isLeft()){
                    humidityTolerance = humidityTolerance - 0.5;
                    if(humidityTolerance < 1){
                        humidityTolerance = 10;
                    }
                }
                mem.setHumidityTolerance(humidityTolerance);
                break;
            case 4:
                tempTolerance = mem.getTempTolerance();
                if(encoder.isRight()){
                    tempTolerance = tempTolerance + 0.5;
                    if(tempTolerance > 10){
                        tempTolerance = 1;
                    }
                } else if (encoder.isLeft()) {
                    tempTolerance = tempTolerance - 0.5;
                    if(tempTolerance < 1){
                        tempTolerance = 10;
                    }
                }
                mem.setTempTolerance(tempTolerance);
                break;
            default:
                lcdInEditMode = false;
                break;
        }
        if(encoder.isPress()){
            mem.storeValues();
            lcdInEditMode = false;
        }
    }
}

bool isLoaded = false;
void setup(){
    Serial.begin(9600);
    Wire.begin();
    dht.begin();
    ventRelay.setup();
    heaterRelay.setup();
    mem.readValues();
    lcd.init();
    lcd.backlight();
    lcd.setCursor(3,0);
    lcd.print("Loading...");
    lcd.createChar(0, heatIcon);
    lcd.createChar(1, ventIcon);
    lcd.createChar(2, degreeIcon);
    lcd.createChar(3, arrowLeftIcon);
    lcd.createChar(4, arrowRightIcon);
}

void loop(){
    encoder.tick();
    handleEncoder();
    if(!isLoaded){
        isLoaded = true;
        lcd.clear();
        lcdMode = 0;
    }
    ventRelay.loop();
    heaterRelay.loop();
    airControl();
    handleLcd();
}




