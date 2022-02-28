#include <Arduino.h>
#include <HX711.h>
#include <ACS712.h>
#include <ArduinoJson.h>

#define PIN_ENGINE 3

#define PIN_RPM 2

#define PIN_DT1 9
#define PIN_SCK1 8
#define PIN_DT2 11
#define PIN_SCK2 10
#define PIN_DT3 13
#define PIN_SCK3 12

#define PIN_ACS A0

const float CONV_RATE = 0.035274;

volatile unsigned long previous_rpm = 0;
volatile auto current_rpm = 0;

auto running = false;

HX711 hx1;
HX711 hx2;
HX711 hx3;

ACS712* acs = new ACS712(ACS712_20A, PIN_ACS);

void calc_rpm(){
    current_rpm = (micros() - previous_rpm) / 1000000 * 60;
    previous_rpm = micros();
}

void setup() {
    pinMode(PIN_ENGINE, OUTPUT);

    pinMode(PIN_RPM, INPUT);

    pinMode(PIN_DT1, INPUT);
    pinMode(PIN_SCK1, INPUT);
    pinMode(PIN_DT2, INPUT);
    pinMode(PIN_SCK2, INPUT);
    pinMode(PIN_DT3, INPUT);
    pinMode(PIN_SCK3, INPUT);

    pinMode(PIN_ACS, INPUT);

    hx1.begin(PIN_DT1, PIN_SCK1);
    hx1.set_scale();
    hx1.tare();
    hx2.begin(PIN_DT2, PIN_SCK2);
    hx2.set_scale();
    hx2.tare();
    hx3.begin(PIN_DT3, PIN_SCK3);
    hx3.set_scale();
    hx3.tare();

    acs->calibrate();

    Serial.begin(115200);

    attachInterrupt(0, calc_rpm, FALLING);
}

float get_tensometer1_data(){
    return hx1.get_units(10) * CONV_RATE; // in grams
}

float get_tensometer2_data(){
    return hx2.get_units(10) * CONV_RATE; // in grams
}

float get_tensometer3_data(){
    return hx3.get_units(10) * CONV_RATE; // in grams
}

float get_amperes(){
    return acs->getCurrentAC();
}

float get_voltage(){
    return acs->getCurrentDC();
}

void send_current(){
    auto current_amperes = get_amperes();
    auto current_voltage = get_voltage();

    auto tensometer1 = get_tensometer1_data();
    auto tensometer2 = get_tensometer2_data();
    auto tensometer3 = get_tensometer3_data();


    DynamicJsonDocument doc(1024);
    doc["current_rpm"] = current_rpm;
    doc["current_amperes"] = current_amperes;
    doc["current_voltage"] = current_voltage;
    doc["tensometer1"] = tensometer1;
    doc["tensometer2"] = tensometer2;
    doc["tensometer3"] = tensometer3;
    doc["running"] = running;

    serializeJson(doc, Serial);

    delay(1000);
}

void loop() {
    if (Serial.available() && !running){
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, Serial);

        const bool* soft_start = doc["params"]["soft_start"];

        running = true;

        analogWrite(PIN_ENGINE, 2000);
        delay(10);
        analogWrite(PIN_ENGINE, 1000);

        if (soft_start){
            for (int i = 1000; i < 2000; ++i) {
                analogWrite(PIN_ENGINE, i);

                if (i % 100 == 0){
                    send_current();
                    delay(1000);
                }

                delay(1);
            }
        } else {
            analogWrite(PIN_ENGINE, 2000);

            for (int i = 0; i < 1000; ++i) {
                send_current();
                delay(2);
            }
        }

        delay(5000);

        running = false;
        send_current();

        analogWrite(PIN_ENGINE, 0);
    }
}