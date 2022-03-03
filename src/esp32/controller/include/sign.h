#ifndef IAMABUS_SIGN_H
#define IAMABUS_SIGN_H

#include "platform.h"
#include "config.h"

#include "screen.h"

void drawSign() {
    Serial.println("drawSign: " + signText);
}

#endif