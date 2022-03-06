#ifndef IAMABUS_FLIPDOT_DEBUG_H
#define IAMABUS_FLIPDOT_DEBUG_H

#include <Arduino.h>

#include <FlipDot_Protocol.hpp>

class FlipDotDebugger
{
public:
    String getFrameType(uint8_t msgType, uint8_t* msgData, uint8_t msgDataLen);
};

extern FlipDotDebugger flipDotDebugger;

#endif
