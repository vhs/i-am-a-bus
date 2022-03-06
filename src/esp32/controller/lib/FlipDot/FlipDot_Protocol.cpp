#include "FlipDot_Protocol.hpp"

FlipDotPacket FlipDotPacket_Discovery = { .msgType = FLIPDOT_MSG_TYPE_BUS, .msgData = {FLIPDOT_BUS_DISCOVER}, .msgDataLen = 0x01 };

FlipDotPacket FlipDotPacket_Goodbye = { .msgType = FLIPDOT_MSG_TYPE_BUS, .msgData = {FLIPDOT_BUS_GOODBYE}, .msgDataLen = 0x01 };

FlipDotPacket FlipDotPacket_Query_State = { .msgType = FLIPDOT_MSG_TYPE_BUS, .msgData = {FLIPDOT_BUS_QUERY_STATE}, .msgDataLen = 0x01 };

FlipDotPacket FlipDotPacket_Pixels_Complete = { .msgType = FLIPDOT_MSG_TYPE_PIXEL_COMPLETE, .msgData = {FLIPDOT_DATA_PIXELS_COMPLETE}, .msgDataLen = 0x01 };
