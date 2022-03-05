#include "menu.hpp"

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

    SELECT_MENU_OPTIONS[select_menu_size] = "BACK TO MENU";

    select_menu_size++;

    select_menu_go_back_id = (select_menu_size - 1);
}

void resetSelectMenu() {
    SELECT_MENU_INDEX = 0;
}