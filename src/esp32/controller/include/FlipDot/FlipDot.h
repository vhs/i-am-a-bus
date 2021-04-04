#pragma once

#include <Arduino.h>

#include "FlipDot/FlipDot_Protocol.h"
#include "FlipDot/FlipDot_Debug.h"
FlipDotDebugger flipDotDebugger;

class FlipDot
{

public:
    FlipDot()
    {
        clear();
    }

    void clear()
    {
        _msgDataLen = 0x00;
        _signAddress = 0x0000;
        _msgType = 0x00;
        std::fill(_msgData, _msgData + sizeof(_msgData), 0);
        _checksum = 0x00;
    }

    String encodeFrame()
    {
        generateChecksum();

        String encodedFrame = ":";

        encodedFrame += convertHex8(_msgDataLen);
        encodedFrame += convertHex16(_signAddress);
        encodedFrame += convertHex8(_msgType);
        encodedFrame += convertHexData();
        encodedFrame += convertHex8(_checksum);

        encodedFrame.trim();
        encodedFrame += "\r\n";

        return encodedFrame;
    }

    int decodeFrame(String frame)
    {
        frame.trim();

        if (frame.length() > sizeof(_rawFrame))
            return -1;

        frame.toCharArray(_rawFrame, sizeof(_rawFrame));
        _rawFrameLen = frame.length();

        _msgDataLen = (uint8_t)strtol(frame.substring(FLIPDOT_PACKET_DATA_LEN_OFFSET, FLIPDOT_PACKET_DATA_LEN_LEN).c_str(), NULL, 16);
        _signAddress = (uint16_t)strtol(frame.substring(FLIPDOT_PACKET_SIGN_ADDRESS_OFFSET, FLIPDOT_PACKET_SIGN_ADDRESS_LEN).c_str(), NULL, 16);
        _msgType = (uint8_t)strtol(frame.substring(FLIPDOT_PACKET_MSG_TYPE_OFFSET, FLIPDOT_PACKET_MSG_TYPE_LEN).c_str(), NULL, 16);

        for (int d = 0; d < (_msgDataLen); d++)
        {
            int offset = FLIPDOT_PACKET_MSG_DATA_OFFSET + (d * 2);
            _msgData[d] = (uint8_t)strtol(frame.substring(offset, offset + 2).c_str(), NULL, 16);
        }

        _checksum = (uint8_t)strtol(frame.substring(FLIPDOT_PACKET_CHECKSUM_OFFSET, FLIPDOT_PACKET_CHECKSUM_LEN).c_str(), NULL, 16);

        if (!verifyChecksum())
            return -2;

        return 0;
    }

    void dump()
    {
        Serial.printf("\n");

        Serial.printf("FlipDot::dump:\n");

        Serial.printf("\n");

        Serial.printf("Raw Frame:\t%s\n", _rawFrame);
        Serial.printf("Raw Frame Len:\t%d\n", _rawFrameLen);
        Serial.printf("\n");
        Serial.printf("Data Length:\t0x%02X\n", _msgDataLen);
        Serial.printf("Sign Address:\t0x%04X\n", _signAddress);
        Serial.printf("Msg Type:\t0x%02X\n", _msgType);

        Serial.printf("Frame Type:\t%s\n", flipDotDebugger.getFrameType(_msgType, _msgData, _msgDataLen).c_str());

        Serial.printf("Msg Data:\t");

        for (int d = 0; d < _msgDataLen; d++)
        {
            if (d > 0)
                Serial.printf(" ");

            Serial.printf("0x%02X", _msgData[d]);
        }

        Serial.printf("\n");

        Serial.printf("Checksum:\t0x%02X\n", _checksum);
        Serial.printf("Verified:\t%d\n", verifyChecksum());

        Serial.printf("\n");
    }

    bool verifyChecksum()
    {
        uint8_t checksum = 0x0;

        checksum -= _msgDataLen;
        checksum -= (_signAddress >> 8) & 0xff;
        checksum -= _signAddress & 0xff;
        checksum -= _msgType;

        for (int d = 0; d < _msgDataLen; d++)
        {
            checksum -= _msgData[d];
        }

        return (checksum == _checksum);
    }

    void generateChecksum()
    {
        uint8_t checksum = 0x0;

        checksum -= _msgDataLen;
        checksum -= (_signAddress >> 8) & 0xff;
        checksum -= _signAddress & 0xff;
        checksum -= _msgType;

        for (int d = 0; d < _msgDataLen; d++)
        {
            checksum -= _msgData[d];
        }
    }

    uint16_t getSignAddress()
    {
        return _signAddress;
    }

    void setSignAddress(uint16_t address)
    {
        _signAddress = address;
    }

    uint16_t getMsgType()
    {
        return _msgType;
    }

    void setMsgType(uint8_t msgType)
    {
        _msgType = msgType;
    }

    uint8_t getDataType()
    {
        return _msgData[0];
    }

    void setDataType(uint8_t dataType)
    {
        _msgData[0] = dataType;
    }

    void clearData()
    {
        _msgDataLen = 0;
        std::fill(_msgData, _msgData + sizeof(_msgData), 0);
    }

    boolean addData(uint8_t msgData)
    {
        if (!(_msgDataLen + 1 < sizeof(_msgData)))
            return false;

        _msgData[_msgDataLen] = msgData;
        _msgDataLen++;

        return true;
    }

    boolean setData(const uint8_t *msgData)
    {
        _msgDataLen = 0;

        if (sizeof(msgData) > sizeof(_msgData))
        {
            Serial.printf("FlipDot.setData received too much data\n");

            return false;
        }

        for (int c = 0; c < sizeof(msgData); c++)
        {
            _msgData[c] = msgData[c];
            _msgDataLen++;
        }

        return true;
    }

private:
    char _rawFrame[512];
    int _rawFrameLen = 0;
    uint8_t _msgDataLen = 0x00;
    uint16_t _signAddress;
    uint8_t _msgType;
    uint8_t _msgData[FLIPDOT_MAX_DATA + 1];
    uint8_t _checksum = 0x00;

    String convertHex8(uint8_t val)
    {
        char buffer[3];

        sprintf(buffer, "%02X", val);

        return String(buffer);
    }

    String convertHex16(uint16_t val)
    {
        char buffer[5];

        sprintf(buffer, "%04X", val);

        return String(buffer);
    }

    String convertHexData()
    {
        // Because I'm not so good at C/C++
        // Someone improve this

        String outp = "";

        char buffer[3];

        for (int d = 0; d < (_msgDataLen); d++)
        {
            sprintf(buffer, "%02X", _msgData[d]);
            outp += String(buffer);
        }

        return outp;
    }
};