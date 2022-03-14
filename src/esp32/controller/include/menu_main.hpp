#ifndef IAMABUS_MENU_MAIN_H
#define IAMABUS_MENU_MAIN_H

#include "platform.h"

#include "externals/io.h"
#include "externals/screen.h"
#include "externals/util.h"
#include "externals/screensaver.h"

#define MAIN_MENU_ITEM_COUNT 4

String MAIN_MENU_OPTIONS[MAIN_MENU_ITEM_COUNT] = {
    "<SELECT OPTION>",
    "SELECT TEXT",
    "NETWORK",
    "EXIT MENU"
};

int MAIN_MENU_INDEX = 0;

#endif