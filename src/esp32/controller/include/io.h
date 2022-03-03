#ifndef IAMABUS_IO_H
#define IAMABUS_IO_H

#include "platform.h"
#include "config.h"

bool isPressedState1 = false;
bool isPressedState2 = false;
long nextCheckTime = 0;
bool isLatched = false;

bool readInput() {
    return digitalRead(PIN_ROT_SW) == LOW;
}

bool latchInput() {
    delay(DEBOUNCE_DELAY);
    return readInput();
}

bool checkButton()
{
    return readInput() && latchInput();
}

void setupPins() {
    pinMode(PIN_LED_GREEN, OUTPUT);

    digitalWrite(PIN_LED_GREEN, LOW);

    pinMode(PIN_LED_RED, OUTPUT);

    digitalWrite(PIN_LED_GREEN, LOW);

    pinMode(PIN_ROT_A, INPUT);
    pinMode(PIN_ROT_B, INPUT);
    pinMode(PIN_ROT_SW, INPUT);

    pinMode(PIN_RS485_RX, INPUT);
    pinMode(PIN_RS485_TX, OUTPUT);
    pinMode(PIN_RS485_EN, OUTPUT);

    digitalWrite(PIN_RS485_EN, LOW);
}

#endif