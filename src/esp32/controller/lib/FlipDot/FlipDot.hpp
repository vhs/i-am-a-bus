#ifndef IAMABUS_FLIPDOT_H
#define IAMABUS_FLIPDOT_H

#include <Arduino.h>

#include "FlipDot_Defines.hpp"
#include "FlipDot_Protocol.hpp"
#include "FlipDot_Debug.hpp"

class FlipDot
{
public:
    FlipDot();
    void clear();
    String encodeFrame();
    int decodeFrame(String frame);
    void dump();
    bool verifyChecksum();
    void generateChecksum();
    uint16_t getSignAddress();
    void setSignAddress(uint16_t address);
    uint16_t getMsgType();
    void setMsgType(uint8_t msgType);
    uint8_t getDataType();
    void setDataType(uint8_t dataType);
    void clearData();
    boolean addData(uint8_t msgData);
    boolean setData(const uint8_t* msgData);

private:
    char _rawFrame[512];
    int _rawFrameLen = 0;
    uint8_t _msgDataLen = 0x00;
    uint16_t _signAddress;
    uint8_t _msgType;
    uint8_t _msgData[FLIPDOT_MAX_DATA + 1];
    uint8_t _checksum = 0x00;

    String convertHex8(uint8_t val);
    String convertHex16(uint16_t val);
    String convertHexData();
};

#define FLIPDOT_LOADED

#endif