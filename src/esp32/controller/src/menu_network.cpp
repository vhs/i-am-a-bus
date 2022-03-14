#include "menu_network.hpp"

void displayNetworkMenu() {
    if ((NETWORK_MENU_INDEX == 1 && network_is_up()) || (NETWORK_MENU_INDEX == 2 && network_is_up() == false))
        drawMenu("* " + NETWORK_MENU_OPTIONS[NETWORK_MENU_INDEX]);
    else
        drawMenu(NETWORK_MENU_OPTIONS[NETWORK_MENU_INDEX]);
}

int handleNetworkMenu() {
    static int network_menu_pos = 0;
    static bool network_menu_dirty = false;

    encoder.tick();

    int newPos = encoder.getPosition();
    if (network_menu_pos != newPos) {
        int direction = (int)(encoder.getDirection());

        if (direction == 1) {
            NETWORK_MENU_INDEX = (NETWORK_MENU_INDEX - 1 + NETWORK_MENU_ITEM_COUNT) % NETWORK_MENU_ITEM_COUNT;
        }
        else if (direction == -1) {
            NETWORK_MENU_INDEX = (NETWORK_MENU_INDEX + 1) % NETWORK_MENU_ITEM_COUNT;
        }

        network_menu_pos = newPos;
        network_menu_dirty = true;
    }

    if (network_menu_dirty)
    {
        displayNetworkMenu();
        network_menu_dirty = false;
        updateLastActiveTime();
    }

    if (checkButton()) {
        switch (NETWORK_MENU_INDEX) {
        case 0:
            break;
        case 1:
            network_start();

            return MENU_ACTION_NETWORK_ENABLE;
        case 2:
            network_stop();

            return MENU_ACTION_NETWORK_DISABLE;
        case 3:
            return MENU_ACTION_EXIT;
        }
    }

    return MENU_ACTION_IDLE;
}

void resetNetworkMenu() {
    NETWORK_MENU_INDEX = 0;
}