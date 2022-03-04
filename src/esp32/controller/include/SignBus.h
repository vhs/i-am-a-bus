#ifndef IAMABUS_SIGNBUS_H
#define IAMABUS_SIGNBUS_H

#include <SoftwareSerial.h>
#include <SPI.h>

#include "platform.h"

#include "FlipDot/FlipDot.h"

SoftwareSerial RS485Serial;

const uint8_t initCmd[17] = { 0x08, 0xB3, 0x00, 0x10, 0x0C, 0x10, 0x00, 0x78, 0x03, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };

class SignBus
{
public:
    SignBus(int rxPin, int txPin, int enPin)
    {
        _enPin = enPin;
        RS485Serial.begin(19200, SWSERIAL_8N1, rxPin, txPin, false, 256);

        for (int sa = 0; sa < FLIPDOT_MAX_SIGN; sa++)
            _knownSigns[sa] = false;

        for (int sa = 0; sa < FLIPDOT_MAX_SIGN; sa++)
            _initializedSigns[sa] = false;
    }

    int discoverSigns()
    {
        _foundSigns = 0;

        _flipDotSend.clear();
        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_BUS);
        _flipDotSend.addData(FLIPDOT_BUS_DISCOVER);

        for (int sa = 0; sa < FLIPDOT_MAX_SIGN; sa++)
        {
            _flipDotSend.setSignAddress(sa);

            _sendFrame();

            delay(25);

            if (_receiveFrame())
            {
                if (_flipDotRecv.getMsgType() == FLIPDOT_MSG_TYPE_REPORT_STATE && _flipDotRecv.getDataType() == FLIPDOT_STATE_UNCONFIGURED)
                {
                    _knownSigns[sa] = true;
                    _foundSigns++;
                }
            }
        }

        return _foundSigns;
    }

    int initializeSigns()
    {
        _initSigns = 0;

        for (int sa = 0; sa < FLIPDOT_MAX_SIGN; sa++)
        {
            if (_knownSigns[sa] == true)
            {
                _flipDotSend.clear();
                _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_REQUEST_OPERATION);
                _flipDotSend.addData(FLIPDOT_OPS_REQUEST_RECEIVE_CONFIG);

                _flipDotSend.setSignAddress(sa);

                _sendFrame();

                if (_receiveFrame())
                {
                    if (_flipDotRecv.getMsgType() == FLIPDOT_MSG_TYPE_ACK_OPERATION && _flipDotRecv.getDataType() == FLIPDOT_ACK_RECEIVE_SIGN_CONFIG)
                    {
                        _flipDotSend.clear();
                        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_SEND_DATA);
                        _flipDotSend.setData((const uint8_t*)initCmd);

                        _sendFrame();

                        delay(5);

                        _flipDotSend.clearData();

                        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_DATA_CHUNKS_SENT);

                        _sendFrame();

                        delay(5);

                        _flipDotSend.clearData();

                        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_BUS);
                        _flipDotSend.setDataType(FLIPDOT_BUS_QUERY_STATE);

                        _sendFrame();

                        delay(5);

                        if (_receiveFrame() && _flipDotRecv.getMsgType() == FLIPDOT_MSG_TYPE_REPORT_STATE && _flipDotRecv.getDataType() == FLIPDOT_STATE_CONFIG_RECEIVED)
                        {
                            _initializedSigns[sa] = true;
                            _initSigns++;
                            Serial.printf("Sign %d is ONLINE\n", sa);
                        }
                    }
                }
            }
        }

        return _initSigns;
    }

    bool* getInitializedSigns()
    {
        return _initializedSigns;
    }

    bool isSignInitialized(int signAddress)
    {
        return _initializedSigns[signAddress];
    }

    bool sendPixelData(int signAddress, const char* buffer, int bufferLines)
    {
        _flipDotSend.clear();
        _flipDotSend.setSignAddress(signAddress);
        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_REQUEST_OPERATION);
        _flipDotSend.addData(FLIPDOT_OPS_REQUEST_RECEIVE_PIXELS);

        _sendFrame();

        if (_receiveFrame() && _flipDotRecv.getMsgType() != FLIPDOT_MSG_TYPE_ACK_OPERATION && _flipDotRecv.getDataType() != FLIPDOT_ACK_RECEIVE_PIXELS)
            return false;

        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_SEND_DATA);

        for (int l = 0; l < bufferLines; l++)
        {
            _flipDotSend.setSignAddress(l << 4);
            _flipDotSend.setData((const uint8_t*)buffer[l]);
            _sendFrame();
        }

        _flipDotSend.clearData();
        _flipDotSend.setSignAddress(signAddress);
        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_DATA_CHUNKS_SENT);
        _sendFrame();

        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_BUS);
        _flipDotSend.setData(FLIPDOT_BUS_QUERY_STATE);
        _sendFrame();

        if (_receiveFrame() && _flipDotRecv.getMsgType() != FLIPDOT_MSG_TYPE_REPORT_STATE && _flipDotRecv.getDataType() != FLIPDOT_STATE_SAVED)
            return false;

        _flipDotSend.clearData();
        _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_PIXEL_COMPLETE);
        _flipDotSend.setData(FLIPDOT_DATA_PIXELS_COMPLETE);
        _sendFrame();
    }

private:
    bool _knownSigns[FLIPDOT_MAX_SIGN];
    bool _initializedSigns[FLIPDOT_MAX_SIGN];
    FlipDot _flipDotRecv;
    FlipDot _flipDotSend;
    String _buffer;
    int _foundSigns = 0;
    int _initSigns = 0;
    int _enPin;

    bool _sendFrame()
    {
        _buffer = _flipDotSend.encodeFrame();

        digitalWrite(PIN_RS485_EN, HIGH);

        return (RS485Serial.write(_buffer.c_str()) > 0);
    }

    bool _receiveFrame()
    {
        digitalWrite(PIN_RS485_EN, LOW);

        delay(10);

        _buffer = RS485Serial.readStringUntil('\n');

        _buffer.trim();

        _flipDotRecv.clear();

        return _flipDotRecv.decodeFrame(_buffer);
    }
};

SignBus signBus(PIN_RS485_RX, PIN_RS485_TX, PIN_RS485_EN);

#endif