#pragma once

#include "platform.h"

// clang-format off
/*
FSM States:
  Boot - Boot up, sends init code to sign, auto falls to rest
  Rest - Resting loop. Rotary encoder adjusts brightness.
  Select - Selecting the message or select "program" to move into that state, control with rotary encoder. Times out back to rest.
  Program - Serves up network to join with phone, serves up webpage to modify stored codes. Times out back to rest.
*/
// clang-format on

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
}

void FSM_STATE_Rest_stop()
{
    Serial.println("Leaving Rest state");
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

void FSM_STATE_Program_start()
{
    Serial.println("Entering Program State");
}

void FSM_STATE_Program_loop()
{
}

void FSM_STATE_Program_stop()
{
    Serial.println("Leaving Program State");
}

// fsm states
FunctionState state_boot(&FSM_STATE_Boot_start, nullptr, nullptr);
FunctionState state_rest(&FSM_STATE_Rest_start, &FSM_STATE_Rest_loop, &FSM_STATE_Rest_stop);
FunctionState state_select(&FSM_STATE_Select_start, &FSM_STATE_Select_loop, &FSM_STATE_Select_stop);
FunctionState state_program(&FSM_STATE_Program_start, &FSM_STATE_Program_loop, &FSM_STATE_Program_stop);

// fsm
FunctionFsm fsm(&state_boot);