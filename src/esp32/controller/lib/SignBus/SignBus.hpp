#ifndef SIGNBUS_H
#define SIGNBUS_H

#include <FlipDot.hpp>

#define SIGNBUS_MAX_SIGNS 16

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

private:
    bool _knownSigns[SIGNBUS_MAX_SIGNS];
    bool _initializedSigns[SIGNBUS_MAX_SIGNS];
    FlipDot _flipDotRecv;
    FlipDot _flipDotSend;
    String _buffer;
    int _foundSigns = 0;
    int _initSigns = 0;
    int _enPin;

    bool _sendFrame();
    bool _receiveFrame();
};

#endif