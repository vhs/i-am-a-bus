#ifndef SIGNBUS_H
#define SIGNBUS_H

#include <SoftwareSerial.h>
#include <SPI.h>

#include <FlipDot.hpp>

#define SIGNBUS_MAX_SIGNS 16

#define FLIPDOT_PIXELS_PER_LINE 16

const uint8_t initCmd[17] = { 0x08, 0xB3, 0x00, 0x10, 0x0C, 0x10, 0x00, 0x78, 0x03, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };

class SignBus
{
public:
    SignBus(int rxPin, int txPin, int enPin);
    int discoverSigns();
    int initializeSigns();
    bool* getInitializedSigns();
    bool isSignInitialized(int signAddress);
    bool sendPixelData(int signAddress, uint8_t* buffer, int bufferLines);
    bool sendPixelData(int signAddress, const char* buffer, int bufferLines);
    void resetSigns();

private:
    SoftwareSerial _RS485Serial;

    bool _knownSigns[SIGNBUS_MAX_SIGNS];
    bool _initializedSigns[SIGNBUS_MAX_SIGNS];
    FlipDot _flipDotRecv;
    FlipDot _flipDotSend;
    String _receiveBuffer = "";
    String _sendBuffer = "";
    int _foundSigns = 0;
    int _initSigns = 0;
    int _enPin = 26;

    bool _sendFrame();
    bool _receiveFrame();
    void _halt(String msg);
};

#endif