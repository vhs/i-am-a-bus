#ifndef IAMABUS_IO_H
#define IAMABUS_IO_H

#include "platform.h"

bool isPressedState1 = false;
bool isPressedState2 = false;
long nextCheckTime = 0;
bool isLatched = false;

RotaryEncoder encoder(PIN_ROT_A, PIN_ROT_B, RotaryEncoder::LatchMode::FOUR3);

#endif