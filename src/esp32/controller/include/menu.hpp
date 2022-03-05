#ifndef IAMABUS_MENU_H
#define IAMABUS_MENU_H

#include "platform.h"

#include "externals/io.h"
#include "externals/screen.h"
#include "externals/util.h"
#include "externals/screensaver.h"

RotaryEncoder encoder(PIN_ROT_A, PIN_ROT_B, RotaryEncoder::LatchMode::FOUR3);

#define MAIN_MENU_ITEM_COUNT 3

String MAIN_MENU_OPTIONS[MAIN_MENU_ITEM_COUNT] = {
    "SELECT OPTION",
    "SELECT TEXT",
    "EXIT MENU"
};

int MAIN_MENU_INDEX = 0;

// Text selection logic

String SELECT_MENU_OPTIONS[16];

int SELECT_MENU_INDEX = 0;

int select_menu_size = -1;

int select_menu_go_back_id = 0;

#endif