#ifndef IAMABUS_FLIPDOT_PROTOCOL_H
#define IAMABUS_FLIPDOT_PROTOCOL_H

#include "FlipDot.hpp"

struct FlipDotPacket
{
    uint8_t msgType;
    uint8_t msgData[FLIPDOT_MAX_DATA];
    uint8_t msgDataLen;
};

typedef struct FlipDotPacket FlipDotPacket;

extern FlipDotPacket FlipDotPacket_Discovery;

extern FlipDotPacket FlipDotPacket_Goodbye;

extern FlipDotPacket FlipDotPacket_Query_State;

extern FlipDotPacket FlipDotPacket_Pixels_Complete;

#endif