#include "SignBus.hpp"

SignBus::SignBus(int rxPin, int txPin, int enPin)
{
    _enPin = enPin;

    _RS485Serial.begin(19200, SWSERIAL_8N1, rxPin, txPin, false, 256, 256 * 9);

    if (!_RS485Serial) {
        _halt("Invalid SoftwareSerial pin configuration, check config");
    }

    resetSigns();

    _RS485Serial.flush();
}

int SignBus::discoverSigns()
{
    _foundSigns = 0;

    _flipDotSend.clear();
    _flipDotSend.setMsgType(FLIPDOT_MSG_TYPE_BUS);
    _flipDotSend.addData(FLIPDOT_BUS_DISCOVER);

    for (int sa = 0; sa < FLIPDOT_MAX_SIGNS; sa++)
    {
        _flipDotSend.setSignAddress(sa);

        _sendFrame();

        bool recvResult = _receiveFrame();

        if (recvResult)
        {
            if (_flipDotRecv.getMsgType() == FLIPDOT_MSG_TYPE_REPORT_STATE && _flipDotRecv.getDataType() == FLIPDOT_STATE_UNCONFIGURED)
            {
                _knownSigns[sa] = true;
                _foundSigns++;
                Serial.printf("Sign FOUND at %d\n", sa);
            }
            else {
                Serial.printf("Invalid sign result for sign %d:\n%s\n", sa, _receiveBuffer.c_str());
            }
        }
        else
        {
            Serial.printf("No sign at %d\n", sa);
        }
    }

    return _foundSigns;
}

int SignBus::initializeSigns()
{
    _initSigns = 0;

    for (int sa = 0; sa < FLIPDOT_MAX_SIGNS; sa++)
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

bool* SignBus::getInitializedSigns()
{
    return _initializedSigns;
}

bool SignBus::isSignInitialized(int signAddress)
{
    return _initializedSigns[signAddress];
}

bool SignBus::sendPixelData(int signAddress, uint8_t* buffer, int bufferLines)
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
        _flipDotSend.clearData();

        for (int p = 0; p < FLIPDOT_PIXELS_PER_LINE; p++)
        {
            _flipDotSend.addData(buffer[l * FLIPDOT_PIXELS_PER_LINE + p]);
        }

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

    return true;
}

bool SignBus::sendPixelData(int signAddress, const char* buffer, int bufferLines)
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

    return true;
}

void SignBus::resetSigns() {
    for (int sa = 0; sa < FLIPDOT_MAX_SIGNS; sa++)
        _knownSigns[sa] = false;

    for (int sa = 0; sa < FLIPDOT_MAX_SIGNS; sa++)
        _initializedSigns[sa] = false;
}

bool SignBus::_sendFrame()
{
    _sendBuffer = _flipDotSend.encodeFrame();

    digitalWrite(_enPin, HIGH);

    delay(30);

    int sendResult = _RS485Serial.printf("%s\r\n", _sendBuffer.c_str());

    delay(30);

    return (sendResult > 0);
}

bool SignBus::_receiveFrame()
{
    _receiveBuffer.clear();

    digitalWrite(_enPin, LOW);

    delay(30);

    while (_RS485Serial.available()) {
        _receiveBuffer += (char)_RS485Serial.read();
    }

    if (_receiveBuffer.length() > 0)
    {
        _receiveBuffer.trim();

        _flipDotRecv.clear();
        _flipDotRecv.decodeFrame(_receiveBuffer);

        int decodeResult = _flipDotRecv.decodeFrame(_receiveBuffer);

        _receiveBuffer = "";

        return decodeResult > 0;
    }

    return false;
}

void SignBus::_halt(String msg) {
    Serial.println(msg);
    Serial.flush();
    while (true) {
        delay(1000);
    }
}