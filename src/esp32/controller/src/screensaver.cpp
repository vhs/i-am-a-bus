#include "screensaver.hpp"

#include "externals/fsm.h"

void updateLastActiveTime() {
    Serial.printf("Updating last active time...\n");
    lastActiveTime = millis();
}

long screensaver_timeout = 0;

void checkScreenSaver() {

    if (screensaver_timeout == 0)
        screensaver_timeout = SCREENSAVER_TIMEOUT > 1000
        ? SCREENSAVER_TIMEOUT
        : SCREENSAVER_TIMEOUT * 1000;

    if (millis() - lastActiveTime > screensaver_timeout) {
        fsm.trigger(FSM_TRIGGER_SCREENSAVER);
    }
}