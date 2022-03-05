#include "fsm.hpp"

/**
 * FSM States:
 *  Boot - Boot up, sends init code to sign, transitions to rest
 *  Rest - Resting loop.
 *  Menu - Menu loop. Rotary encoder selects option. Click to activate option. Times out to screensaver.
 *  Select - Rotary encoder selects message. Click to select. Times out to screensaver.
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
    updateLastActiveTime();
}

// fsm states
FunctionState state_boot(&FSM_STATE_Boot_start, &FSM_STATE_Boot_loop, &FSM_STATE_Boot_stop);

FunctionFsm fsm(&state_boot);

// Rest state

void FSM_STATE_Rest_start()
{
    Serial.println("Enter Rest state");
    clearScreen();
    resetMenu();
    drawMainScreen(signText);
    updateLastActiveTime();
}

void FSM_STATE_Rest_loop()
{
    if (checkButton())
    {
        Serial.println("REST: Button pressed");
        fsm.trigger(FSM_TRIGGER_MAIN_MENU);
    }

    handleTextUpdate(&displayMainScreen);
}

void FSM_STATE_Rest_stop()
{
    Serial.println("Leaving Rest state");
    updateLastActiveTime();
}

FunctionState state_rest(&FSM_STATE_Rest_start, &FSM_STATE_Rest_loop, &FSM_STATE_Rest_stop);

// Main Menu

void FSM_STATE_Main_Menu_start()
{
    Serial.println("Entering Menu State");
    displayMainMenu();
    updateLastActiveTime();
}

void FSM_STATE_Main_Menu_loop()
{
    int menuAction = handleMainMenu();

    if (menuAction == MENU_ACTION_EXIT)
    {
        fsm.trigger(FSM_TRIGGER_REST);
    }
    else if (menuAction == MENU_ACTION_SELECT)
    {
        fsm.trigger(FSM_TRIGGER_SELECT_MENU);
    }

    handleTextUpdate(&displayMainMenu);
}

void FSM_STATE_Main_Menu_stop()
{
    Serial.println("Leaving Menu State");
    updateLastActiveTime();
}

FunctionState state_main_menu(&FSM_STATE_Main_Menu_start, &FSM_STATE_Main_Menu_loop, &FSM_STATE_Main_Menu_stop);

// Select Menu

void FSM_STATE_Select_Menu_start()
{
    Serial.println("Entering Select state");
    clearScreen();
    loadSelectOptions();
    displaySelectMenu();
    updateLastActiveTime();
}

void FSM_STATE_Select_Menu_loop()
{

    int menuAction = handleSelectMenu();

    if (menuAction == MENU_ACTION_EXIT)
    {
        fsm.trigger(FSM_TRIGGER_MAIN_MENU);
    }
    else
        if (menuAction != MENU_ACTION_IDLE) {
            Serial.println("Select: " + String(menuAction));
            updateLastActiveTime();
        }

    handleTextUpdate(&displaySelectMenu);
}

void FSM_STATE_Select_Menu_stop()
{
    Serial.println("Leaving Select State");

    updateLastActiveTime();
}

FunctionState state_select_menu(&FSM_STATE_Select_Menu_start, &FSM_STATE_Select_Menu_loop, &FSM_STATE_Select_Menu_stop);

// Screensaver

void FSM_STATE_Screensaver_start()
{
    Serial.println("Entering Screensaver state");
    clearScreen();
}

void FSM_STATE_Screensaver_loop()
{
    static long screensaver_clear_screen_trigger = 0;

    if (checkButton()) {
        updateLastActiveTime();

        Serial.println("Screensaver: Button pressed");

        fsm.trigger(FSM_TRIGGER_REST);
    }

    if (screensaver_clear_screen_trigger < millis()) {
        clearScreen();
        screensaver_clear_screen_trigger = millis() + 1000;
    }
}

void FSM_STATE_Screensaver_stop()
{
    Serial.println("Leaving Screensaver State");
}

FunctionState state_screensaver(&FSM_STATE_Screensaver_start, &FSM_STATE_Screensaver_loop, &FSM_STATE_Screensaver_stop);

void FSM_Init() {
    // Transition from boot to rest
    fsm.add_transition(&state_boot, &state_rest, FSM_TRIGGER_REST, nullptr);

    // Transition from rest to menu and back
    fsm.add_transition(&state_rest, &state_main_menu, FSM_TRIGGER_MAIN_MENU, nullptr);
    fsm.add_transition(&state_main_menu, &state_rest, FSM_TRIGGER_REST, nullptr);

    // Transition from menu to select and back
    fsm.add_transition(&state_main_menu, &state_select_menu, FSM_TRIGGER_SELECT_MENU, nullptr);
    fsm.add_transition(&state_select_menu, &state_main_menu, FSM_TRIGGER_MAIN_MENU, nullptr);

    // Screensaver transitions
    fsm.add_transition(&state_rest, &state_screensaver, FSM_TRIGGER_SCREENSAVER, nullptr);
    fsm.add_transition(&state_main_menu, &state_screensaver, FSM_TRIGGER_SCREENSAVER, nullptr);
    fsm.add_transition(&state_select_menu, &state_screensaver, FSM_TRIGGER_SCREENSAVER, nullptr);
    fsm.add_transition(&state_screensaver, &state_rest, FSM_TRIGGER_REST, nullptr);

    // Run FSM once to initialize
    fsm.run_machine();

    if (initialized)
        fsm.trigger(FSM_TRIGGER_REST);
    else
        Serial.println("CRITICAL: Sign bus not initialized!");
}