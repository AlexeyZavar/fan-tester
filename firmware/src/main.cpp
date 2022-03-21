//
// Created by AlexeyZavar on 09.03.2022.
//
#include <ArduinoJson.h>
#include <Servo_Hardware_PWM.h>

#include "HX711.h"
#include "ACS712.h"

// ------------------------
// DEVELOPER OPTIONS
// ------------------------
//#define RELEASE

// ------------------------
// PIN SETTINGS
// ------------------------

// http://developer.alexanderklimov.ru/arduino/sensors/voltage.php
// requires analog
#define PIN_VOLTAGE A0

// https://3d-diy.ru/wiki/arduino-datchiki/datchik-toka-acs712/
// requires analog
#define PIN_ACS712 A1

// https://3d-diy.ru/wiki/arduino-datchiki/infrakrasnyj-datchik-prepyatstvij-yl-63/
// requires digital
#define PIN_YL63 2

// https://3d-diy.ru/wiki/arduino-moduli/acp-dlya-analogovyx-vesov-hx711/
// https://wiki.iarduino.ru/page/hx_711_with_tenzo/
// requires digital
#define PIN_HX711_1_DT 22
#define PIN_HX711_1_SCK 23
#define PIN_HX711_2_DT 24
#define PIN_HX711_2_SCK 25
#define PIN_HX711_3_DT 26
#define PIN_HX711_3_SCK 27

#define PIN_ENGINE 8


// ------------------------
// CONSTANTS
// ------------------------
#define OUNCES_TO_GRAMS 0.035274
#define R1 30000.0
#define R2 7500.0

#define PWM_MIN 1000
#define PWM_MAX 2000
#define PWM_STEP ((PWM_MAX - PWM_MIN) / 100)

#define BENCHMARK_TIME 30000

// ------------------------
// INSTANCES
// ------------------------
ACS712 acs712(ACS712_20A, PIN_ACS712);

HX711 hx711_1;
HX711 hx711_2;
HX711 hx711_3;

Servo engine;

// ------------------------
// VARIABLES
// ------------------------
unsigned long previous_rpm_interruption = 0;
double current_rpm = 0;
double delta = 0;

unsigned long start_time = 0;
double ramp_time = 0;

void interruptionRPM();

void setup() {
    Serial.begin(9600);

#ifdef RELEASE
    // setup three HX711
    hx711_1.begin(PIN_HX711_1_DT, PIN_HX711_1_SCK);
    hx711_2.begin(PIN_HX711_2_DT, PIN_HX711_2_SCK);
    hx711_3.begin(PIN_HX711_3_DT, PIN_HX711_3_SCK);

    hx711_1.set_scale();
    hx711_2.set_scale();
    hx711_3.set_scale();

    hx711_1.tare();
    hx711_2.tare();
    hx711_3.tare();

    // todo: calibration factor
    // hx711_1.set_scale(HX711_SCALE_FACTOR);
    // hx711_2.set_scale(HX711_SCALE_FACTOR);
    // hx711_3.set_scale(HX711_SCALE_FACTOR);

    // setup DC
    pinMode(PIN_VOLTAGE, INPUT);

    // setup ACS712
    pinMode(PIN_ACS712, INPUT);

    // setup YL63
    pinMode(PIN_YL63, INPUT);

    // setup engine
    engine.attach(PIN_ENGINE, PWM_MIN, PWM_MAX);

    // todo: setup interruptions (rpm...)
    attachInterrupt(PIN_YL63, interruptionRPM, FALLING);
#endif
}

void interruptionRPM() {
    auto prev_rpm = current_rpm;
    current_rpm = 60000.0 / (millis() - previous_rpm_interruption);

    if (delta > 100 && ramp_time == 0) {
        delta = abs(prev_rpm - current_rpm);
    } else {
        ramp_time = millis() - start_time;
    }
}

// ------------------------
// UTILITIES
// ------------------------

float getVoltage() {
#if RELEASE
    int value = analogRead(PIN_VOLTAGE);
    float vout = (value * 5.0) / 1024.0;
    float vin = vout / (R2 / (R1 + R2));

    return vin;
#else
    return 4;
#endif
}

float getAmperes() {
#ifdef RELEASE
    return acs712.getCurrentDC();
#else
    return 10;
#endif
}

float getLoad(HX711 hx711) {
#ifdef RELEASE
    // todo: 1 or 10?
    return hx711.get_units(10) * OUNCES_TO_GRAMS;
#else
    return 15;
#endif
}

float getTotalLoad() {
    float total = 0.0;
    total += getLoad(hx711_1);
    total += getLoad(hx711_2);
    total += getLoad(hx711_3);

    return total;
}

void setEnginePWM(int val) {
#ifdef RELEASE
    auto realValue = map(val, PWM_MIN, PWM_MAX, 0, 180);

    engine.write(realValue);
#endif
}

void sendSensorsData(bool running, int pwm) {
    DynamicJsonDocument doc(1024);

    doc["pwm"] = pwm;
    doc["current_rpm"] = current_rpm;
    doc["current_amperes"] = getAmperes();
    doc["current_voltage"] = getVoltage();
    doc["tensometer1"] = getLoad(hx711_1);
    doc["tensometer2"] = getLoad(hx711_2);
    doc["tensometer3"] = getLoad(hx711_3);
    doc["running"] = running;

    serializeJson(doc, Serial);
    Serial.println();
}

// ------------------------
// MAIN LOOP & BENCHMARKS
// ------------------------

void startBenchmark(bool softStart) {
    // start up the engine
    setEnginePWM(PWM_MAX);
    delay(100);
    setEnginePWM(PWM_MIN);

    start_time = millis();
    if (softStart) {
        for (int i = PWM_MIN; i < PWM_MAX; i += PWM_STEP) {
            setEnginePWM(i);
            delay(10);

            if (i % 50 == 0) {
                sendSensorsData(true, i);
            }
            delay(100);
        }
    } else {
        setEnginePWM(PWM_MAX);
    }

    auto start = millis();
    while (millis() - start < BENCHMARK_TIME) {
        sendSensorsData(true, PWM_MAX);
        delay(1000);
    }

    delay(3000);
    sendSensorsData(false, PWM_MAX);

    setEnginePWM(PWM_MIN);
}

void loop() {
    if (Serial.available()) {
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, Serial);

        const char *action = doc["action"];
        if (strcmp(action, "start_benchmark") == 0) {
            startBenchmark(doc["params"]["soft_start"]);
        }
    }
}
