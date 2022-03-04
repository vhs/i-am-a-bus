#ifndef IAMABUS_MENU_H
#define IAMABUS_MENU_H

#include "platform.h"

#include "screen.h"
#include "util.h"

#include <RotaryEncoder.h>

#define MENU_ACTION_IDLE 0
#define MENU_ACTION_EXIT 1
#define MENU_ACTION_SELECT 2

RotaryEncoder encoder(PIN_ROT_A, PIN_ROT_B, RotaryEncoder::LatchMode::FOUR3);

#define MAIN_MENU_ITEM_COUNT 3

String MAIN_MENU_OPTIONS[MAIN_MENU_ITEM_COUNT] = {
    "SELECT OPTION",
    "SELECT TEXT",
    "EXIT MENU"
};

int MAIN_MENU_INDEX = 0;

void displayMainMenu() {
    // clearScreen();
    drawMenu(MAIN_MENU_OPTIONS[MAIN_MENU_INDEX]);
}

int handleMainMenu() {
    static int main_menu_pos = 0;
    static bool main_menu_dirty = false;

    encoder.tick();

    int newPos = encoder.getPosition();
    if (main_menu_pos != newPos) {
        int direction = (int)(encoder.getDirection());

        Serial.printf("pos -> newPos: %d -> %d := %s\n", main_menu_pos, newPos, direction == 1 ? "CCW" : "CW");

        if (direction == 1) {
            MAIN_MENU_INDEX = (MAIN_MENU_INDEX - 1 + MAIN_MENU_ITEM_COUNT) % MAIN_MENU_ITEM_COUNT;
        }
        else if (direction == -1) {
            MAIN_MENU_INDEX = (MAIN_MENU_INDEX + 1) % MAIN_MENU_ITEM_COUNT;
        }

        main_menu_pos = newPos;
        main_menu_dirty = true;
    }

    if (main_menu_dirty)
    {
        displayMainMenu();
        main_menu_dirty = false;
    }

    if (checkButton()) {
        switch (MAIN_MENU_INDEX) {
        case 0:
            break;
        case 1:
            return MENU_ACTION_SELECT;
        case 2:
            return MENU_ACTION_EXIT;
        }
    }

    return MENU_ACTION_IDLE;
}

void resetMenu() {
    MAIN_MENU_INDEX = 0;
}

// Text selection logic

String SELECT_MENU_OPTIONS[16];

int SELECT_MENU_INDEX = 0;

int select_menu_size = -1;

int select_menu_go_back_id = 0;

void displaySelectMenu() {
    // clearScreen();
    drawMenu(SELECT_MENU_OPTIONS[SELECT_MENU_INDEX]);
}

int handleSelectMenu() {
    static int select_menu_pos = 0;
    static bool select_menu_dirty = false;

    if (select_menu_size == -1) {
        select_menu_size = sizeof(SELECT_MENU_OPTIONS);
    }

    encoder.tick();

    int newPos = encoder.getPosition();
    if (select_menu_pos != newPos) {
        int direction = (int)(encoder.getDirection());

        Serial.printf("pos -> newPos: %d -> %d := %s\n", select_menu_pos, newPos, direction == 1 ? "CCW" : "CW");

        Serial.printf("SELECT_MENU_INDEX: %d -> ", SELECT_MENU_INDEX);

        if (direction == 1) {
            SELECT_MENU_INDEX = (SELECT_MENU_INDEX - 1 + select_menu_size) % select_menu_size;
        }
        else if (direction == -1) {
            SELECT_MENU_INDEX = (SELECT_MENU_INDEX + 1) % select_menu_size;
        }

        Serial.println(SELECT_MENU_INDEX);

        select_menu_pos = newPos;
        select_menu_dirty = true;
    }

    if (select_menu_dirty) {
        displaySelectMenu();
        select_menu_dirty = false;
    }

    if (checkButton()) {
        if (SELECT_MENU_INDEX == select_menu_go_back_id) {
            return MENU_ACTION_EXIT;
        }
        else
        {
            Serial.printf("TEXT: %d - %s\n", SELECT_MENU_INDEX, SELECT_MENU_OPTIONS[SELECT_MENU_INDEX].c_str());
            dirtySignText = SELECT_MENU_OPTIONS[SELECT_MENU_INDEX];
        }
    }

    return MENU_ACTION_IDLE;
}

void loadSelectOptions() {
    select_menu_size = loadTexts(SELECT_MENU_OPTIONS);

    SELECT_MENU_OPTIONS[select_menu_size] = "BACK TO MENU";

    select_menu_size++;

    select_menu_go_back_id = (select_menu_size - 1);
}

void resetSelectMenu() {
    SELECT_MENU_INDEX = 0;
}

#endif