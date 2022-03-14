// #undef INTERFACE
#if !defined(IAMABUS_IO_H)

#include "platform.h"

void setupPins();
bool checkButton();
bool latchInput();
bool readInput();

extern RotaryEncoder encoder;

#endif