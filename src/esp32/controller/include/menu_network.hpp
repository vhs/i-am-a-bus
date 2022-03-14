#ifndef IAMABUS_MENU_NETWORK_H
#define IAMABUS_MENU_NETWORK_H

#include "platform.h"

#include "externals/io.h"
#include "externals/screen.h"
#include "externals/util.h"
#include "externals/screensaver.h"
#include "externals/network.h"

#define NETWORK_MENU_ITEM_COUNT 4

String NETWORK_MENU_OPTIONS[NETWORK_MENU_ITEM_COUNT] = {
    "<SELECT OPTION>",
    "ENABLE NETWORK",
    "DISABLE NETWORK",
    "BACK TO MAIN MENU"
};

unsigned int NETWORK_MENU_INDEX = 0;

#endif