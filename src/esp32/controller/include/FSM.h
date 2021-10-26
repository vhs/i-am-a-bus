#pragma once

#include "platform.h"

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
    Serial.println("Entering Boot state");
    // Send a bunch of crap to the RS485 port to init it

    Serial.print("Discovering LED signs...");
    if (signBus.discoverSigns() > 0)
    {
        signBus.initializeSigns();
    }
    Serial.println("done!");
}

void FSM_STATE_Rest_start()
{
    Serial.println("Enter Rest state");
}

void FSM_STATE_Rest_loop()
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(u8g2_font_ncenB08_tf);
        u8g2.drawStr(0, 12, "i-am-a-bus");
        u8g2.drawFrame(1, 63 - (16 + 2), 120 + 2, 16 + 2);
        u8g2.setFont(u8g2_font_ncenB14_tr);
        u8g2.drawStr(2, 63 - (16 + 2) + 16, signText.c_str());
    } while (u8g2.nextPage());
}

void FSM_STATE_Rest_stop()
{
    Serial.println("Leaving Rest state");
}

void FSM_STATE_Menu_start()
{
    Serial.println("Entering Menu State");
}

void FSM_STATE_Menu_loop()
{
}

void FSM_STATE_Menu_stop()
{
    Serial.println("Leaving Menu State");
}

void FSM_STATE_Select_start()
{
    Serial.println("Entering Select state");
}

void FSM_STATE_Select_loop()
{
}

void FSM_STATE_Select_stop()
{
}

// fsm states
FunctionState state_boot(&FSM_STATE_Boot_start, nullptr, nullptr);
FunctionState state_rest(&FSM_STATE_Rest_start, &FSM_STATE_Rest_loop, &FSM_STATE_Rest_stop);
FunctionState state_menu(&FSM_STATE_Menu_start, &FSM_STATE_Menu_loop, &FSM_STATE_Menu_stop);
FunctionState state_select(&FSM_STATE_Select_start, &FSM_STATE_Select_loop, &FSM_STATE_Select_stop);

// fsm
FunctionFsm fsm(&state_boot);

enum FSM_Triggers {
    FSM_TRIGGER_MENU
};

void FSM_Init() {
    fsm.add_transition(&state_rest, &state_menu, FSM_TRIGGER_MENU, nullptr);
    fsm.add_transition(&state_menu, &state_rest, FSM_TRIGGER_MENU, nullptr);
}
