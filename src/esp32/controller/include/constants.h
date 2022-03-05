#ifndef IAMABUS_CONSTANTS_H
#define IAMABUS_CONSTANTS_H

#ifndef DEBOUNCE_DELAY
#define DEBOUNCE_DELAY 50
#endif

#define MENU_ACTION_IDLE 0
#define MENU_ACTION_EXIT 1
#define MENU_ACTION_SELECT 2

enum FSM_Triggers {
    FSM_TRIGGER_REST,
    FSM_TRIGGER_MENU,
    FSM_TRIGGER_SELECT
};

#endif