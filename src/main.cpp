#include <Arduino.h>
#include "lfoContainer.h"

uint8_t lfoCastValue;

LfoContainer::LfoContainer lfoContainer;

const uint8_t kReadUpdateTime = 25;
const float kReadScaling = 1.0f / (1 << 10);

struct Times {
    unsigned long lastLfoUpdate;
    unsigned long lastRead;
    unsigned long now;
};
Times times;

struct InputValues
{
    float rateEncoder;
    float rateCV;
    float attenuverter;
};
InputValues inputValues;

// Fix issues with millis due to changing timer0 prescaler (see https://nerdytechy.com/how-to-change-the-pwm-frequency-of-arduino/)
#define CORRECT_CLOCK 6
#define millis() (millis() >> CORRECT_CLOCK)

void setup() {
    // Set PWM pins as output
    pinMode(3, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);

    // Set prescalers for all PWM pins to x1
    TCCR0B = 0b00000001;
    TCCR1B = 0b00001001;
    TCCR2B = 0b00000001;

    // Set Fast PWM mode for all PWM pins
    TCCR0A = 0b00000011;
    TCCR1A = 0b00000001;
    TCCR2A = 0b00000011;

    lfoContainer.Init();
    times.now = millis();
    times.lastLfoUpdate = times.now;
    times.lastRead = times.now;
}

void loop() {
    times.now = millis();
    lfoContainer.Process((times.now - times.lastLfoUpdate) * 1e-3);
    times.lastLfoUpdate = times.now;

    if (times.now - times.lastRead >= kReadUpdateTime) {
        inputValues.rateEncoder = analogRead(PIN_A0) * kReadScaling;
        inputValues.rateCV = analogRead(PIN_A1) * kReadScaling;
        inputValues.attenuverter = analogRead(PIN_A2) * kReadScaling * 2 - 1;
        lfoContainer.SetFrequencyScaler(inputValues.rateEncoder + inputValues.rateCV * inputValues.attenuverter);
        times.lastRead = times.now;
    }

    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(0) * UINT8_MAX);
    analogWrite(3, lfoCastValue);
    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(1) * UINT8_MAX);
    analogWrite(5, lfoCastValue);
    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(2) * UINT8_MAX);
    analogWrite(6, lfoCastValue);
    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(3) * UINT8_MAX);
    analogWrite(9, lfoCastValue);
    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(4) * UINT8_MAX);
    analogWrite(10, lfoCastValue);
    lfoCastValue = static_cast<uint8_t>(lfoContainer.Get(5) * UINT8_MAX);
    analogWrite(11, lfoCastValue);
}
