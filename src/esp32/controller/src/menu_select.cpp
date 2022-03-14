#include "menu_select.hpp"

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
        updateLastActiveTime();
    }

    if (checkButton()) {
        if (SELECT_MENU_INDEX == select_menu_go_back_id) {
            return MENU_ACTION_EXIT;
        }
        else
        {
            dirtySignText = SELECT_MENU_OPTIONS[SELECT_MENU_INDEX];
        }
    }

    return MENU_ACTION_IDLE;
}

void loadSelectOptions() {
    select_menu_size = loadTexts(SELECT_MENU_OPTIONS);

    SELECT_MENU_OPTIONS[select_menu_size] = "BACK TO MAIN MENU";

    select_menu_size++;

    select_menu_go_back_id = (select_menu_size - 1);
}

void resetSelectMenu() {
    SELECT_MENU_INDEX = 0;
}