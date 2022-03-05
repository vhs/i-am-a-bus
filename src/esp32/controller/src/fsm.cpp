#include "fsm.hpp"

/**
 * FSM States:
 *  Boot - Boot up, sends init code to sign, auto falls to rest
 *  Rest - Resting loop. Rotary encoder adjusts brightness.
 *  Select - Selecting the message or select "program" to move into that state, control with rotary encoder. Times out back to rest.
 *  Program - Serves up network to join with phone, serves up webpage to modify stored codes. Times out back to rest.
 */

 // fsm state functions
void FSM_STATE_Boot_start()
{
    drawMainScreen("Booting...");
    Serial.println("Entering Boot state");

    // Load default text
    drawMainScreen("Loading default text...");
    loadCurrentText();

    drawMainScreen("Discovering LED signs...");

    Serial.print("Discovering LED signs...");
    if (signBus.discoverSigns() > 0)
    {
        drawBootScreen("Initializing signs...");
        signBus.initializeSigns();
    }
    Serial.println("done!");
    initialized = true;

    drawMainScreen("Ready");
}

void FSM_STATE_Boot_loop() {
}

void FSM_STATE_Boot_stop()
{
    Serial.println("Leaving Boot state");
}

// fsm states
FunctionState state_boot(&FSM_STATE_Boot_start, &FSM_STATE_Boot_loop, &FSM_STATE_Boot_stop);

// fsm
FunctionFsm fsm(&state_boot);

void FSM_STATE_Rest_start()
{
    Serial.println("Enter Rest state");
    clearScreen();
    resetMenu();
    drawMainScreen(signText);
}

void FSM_STATE_Rest_loop()
{
    if (checkButton())
    {
        Serial.println("REST: Button pressed");
        fsm.trigger(FSM_TRIGGER_MENU);
    }

    handleTextUpdate(&displayMainScreen);
}

void FSM_STATE_Rest_stop()
{
    Serial.println("Leaving Rest state");
}

void FSM_STATE_Menu_start()
{
    Serial.println("Entering Menu State");
    displayMainMenu();
}

void FSM_STATE_Menu_loop()
{
    int menuAction = handleMainMenu();

    if (menuAction == MENU_ACTION_EXIT)
    {
        fsm.trigger(FSM_TRIGGER_REST);
    }
    else if (menuAction == MENU_ACTION_SELECT)
    {
        fsm.trigger(FSM_TRIGGER_SELECT);
    }

    handleTextUpdate(&displayMainMenu);
}

void FSM_STATE_Menu_stop()
{
    Serial.println("Leaving Menu State");
}

void FSM_STATE_Select_start()
{
    Serial.println("Entering Select state");
    clearScreen();
    loadSelectOptions();
    displaySelectMenu();
}

void FSM_STATE_Select_loop()
{

    int menuAction = handleSelectMenu();

    if (menuAction == MENU_ACTION_EXIT)
    {
        fsm.trigger(FSM_TRIGGER_MENU);
    }
    else
        if (menuAction != MENU_ACTION_IDLE) {
            Serial.println("Select: " + String(menuAction));
        }

    handleTextUpdate(&displaySelectMenu);
}

void FSM_STATE_Select_stop()
{
    Serial.println("Leaving Select State");
}

FunctionState state_rest(&FSM_STATE_Rest_start, &FSM_STATE_Rest_loop, &FSM_STATE_Rest_stop);
FunctionState state_menu(&FSM_STATE_Menu_start, &FSM_STATE_Menu_loop, &FSM_STATE_Menu_stop);
FunctionState state_select(&FSM_STATE_Select_start, &FSM_STATE_Select_loop, &FSM_STATE_Select_stop);

void FSM_Init() {
    // Transition from boot to rest
    fsm.add_transition(&state_boot, &state_rest, FSM_TRIGGER_REST, nullptr);

    // Transition from rest to menu and back
    fsm.add_transition(&state_rest, &state_menu, FSM_TRIGGER_MENU, nullptr);
    fsm.add_transition(&state_menu, &state_rest, FSM_TRIGGER_REST, nullptr);

    // Transition from menu to select and back
    fsm.add_transition(&state_menu, &state_select, FSM_TRIGGER_SELECT, nullptr);
    fsm.add_transition(&state_select, &state_menu, FSM_TRIGGER_MENU, nullptr);

    // Run FSM once to initialize
    fsm.run_machine();

    if (initialized)
        fsm.trigger(FSM_TRIGGER_REST);
    else
        Serial.println("CRITICAL: Sign bus not initialized!");
}