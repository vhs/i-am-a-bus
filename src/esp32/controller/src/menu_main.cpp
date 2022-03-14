#include "menu_main.hpp"

void displayMainMenu() {
    drawMenu(MAIN_MENU_OPTIONS[MAIN_MENU_INDEX]);
}

int handleMainMenu() {
    static int main_menu_pos = 0;
    static bool main_menu_dirty = false;

    encoder.tick();

    int newPos = encoder.getPosition();
    if (main_menu_pos != newPos) {
        int direction = (int)(encoder.getDirection());

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
        updateLastActiveTime();
    }

    if (checkButton()) {
        switch (MAIN_MENU_INDEX) {
        case 0:
            break;
        case 1:
            return MENU_ACTION_SELECT_MENU;
        case 2:
            return MENU_ACTION_NETWORK_MENU;
        case 3:
            return MENU_ACTION_EXIT;
        }
    }

    return MENU_ACTION_IDLE;
}

void resetMainMenu() {
    MAIN_MENU_INDEX = 0;
}