#pragma once

#include <Arduino.h>

#include "FlipDot/FlipDot_Protocol.h"

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
        _msgType = FLIPDOT_MSG_TYPE_DEFAULT;
        std::fill(_msgData, _msgData + sizeof(_msgData), 0);
        _checksum = 0x00;
    }

    String encodePacket()
    {
        if (!generateChecksum())
            return "ERROR";

        String encodedPacket = ":";

        encodedPacket += convertHex8(_msgDataLen);
        encodedPacket += convertHex16(_signAddress);
        encodedPacket += convertHex8(_msgType);
        encodedPacket += convertHexData();
        encodedPacket += convertHex8(_checksum);

        return encodedPacket;
    }

    int decodePacket(String packet)
    {
        packet.trim();

        if (packet.length() > sizeof(_rawPacket))
            return -1;

        packet.toCharArray(_rawPacket, sizeof(_rawPacket));
        _rawPacketLen = packet.length();

        _msgDataLen = (uint8_t)strtol(packet.substring(FLIPDOT_PACKET_DATA_LEN_OFFSET, FLIPDOT_PACKET_DATA_LEN_LEN).c_str(), NULL, 16);
        _signAddress = (uint16_t)strtol(packet.substring(FLIPDOT_PACKET_SIGN_ADDRESS_OFFSET, FLIPDOT_PACKET_SIGN_ADDRESS_LEN).c_str(), NULL, 16);
        _msgType = (uint8_t)strtol(packet.substring(FLIPDOT_PACKET_MSG_TYPE_OFFSET, FLIPDOT_PACKET_MSG_TYPE_LEN).c_str(), NULL, 16);

        for (int d = 0; d < _msgDataLen; d++)
        {
            int offset = FLIPDOT_PACKET_MSG_DATA_OFFSET + (d * 2);
            _msgData[d] = (uint8_t)strtol(packet.substring(offset, offset + 2).c_str(), NULL, 16);
        }

        _checksum = (uint8_t)strtol(packet.substring(FLIPDOT_PACKET_CHECKSUM_OFFSET, FLIPDOT_PACKET_CHECKSUM_LEN).c_str(), NULL, 16);

        if (!verifyChecksum())
            return -2;

        return 0;
    }

    void dump()
    {
        Serial.printf("\n");

        Serial.printf("FlipDot::dump:\n");

        Serial.printf("\n");

        Serial.printf("Raw Packet:\t%s\n", _rawPacket);
        Serial.printf("Raw Packet Len:\t%d\n", _rawPacketLen);
        Serial.printf("\n");
        Serial.printf("Data Length:\t%02x\n", _msgDataLen);
        Serial.printf("Sign Address:\t%04X\n", _signAddress);
        Serial.printf("Msg Type:\t%02X\n", _msgType);

        Serial.printf("Msg Data:\t");

        for (int d = 0; d < _msgDataLen; d++)
        {
            if (d > 0)
                Serial.printf(" ");

            Serial.printf("%02X", _msgData[d]);
        }

        Serial.printf("\n");

        Serial.printf("Checksum:\t%02X\n", _checksum);

        Serial.printf("\n");
    }

    bool verifyChecksum()
    {
        return true;
    }

    bool generateChecksum()
    {
        return true;
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

private:
    char _rawPacket[512];
    int _rawPacketLen = 0;
    uint8_t _msgDataLen = 0x00;
    uint16_t _signAddress;
    uint8_t _msgType;
    uint8_t _msgData[FLIPDOT_MAX_DATA];
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

        for (int d = 0; d < _msgDataLen; d++)
        {
            sprintf(buffer, "%02X", _msgData[d]);
            outp += String(buffer);
        }

        return outp;
    }
};