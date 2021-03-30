#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <FunctionFSM.h>
#include "sign_static.h"

#define pin_led_green 4
#define pin_led_red 23

#define pin_rot_a 14
#define pin_rot_b 27
#define pin_rot_sw 13

#define pin_rs485_rx 18
#define pin_rs485_tx 19
#define pin_rs485_en 26 // RX=Low, TX=High

// We actually use hardware defaults for i2c, but defined here for completeness
#define pin_i2c_sda 21
#define pin_i2c_scl 22


/* Constructor  for screen */
U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R0);

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
void
FSM_STATE_Boot_start()
{
Serial.println("Enter Boot state");
// Send a bunch of crap to the RS485 port to init it 
}

void
FSM_STATE_Rest_start()
{
  Serial.println("Enter Rest state");
}
void
FSM_STATE_Rest_loop()
{
}
void
FSM_STATE_Rest_stop()
{
  Serial.println("Leaving Rest state");
}

void
FSM_STATE_Select_start()
{
  Serial.println("Entering Select state");
}

void
FSM_STATE_Select_loop()
{
}

void
FSM_STATE_Select_stop()
{
}

void
FSM_STATE_Program_start()
{
  Serial.println("Entering Program State");
}
void
FSM_STATE_Program_loop()
{
}
void
FSM_STATE_Program_stop()
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

void
setup()
{
  Serial.begin(115200);
  Serial.flush();
  Serial.println();
  Serial.println("Bus Sign Control");
  Serial.println();
  Serial.flush(); // Get serial all nice and ready, with some new lines.
  
  u8g2.begin(); /* u8g2.begin() is required and will sent the setup/init sequence to the display */
  
  pinMode(pin_led_green, OUTPUT);
  digitalWrite(pin_led_green, LOW);
  pinMode(pin_led_red, OUTPUT);
  digitalWrite(pin_led_green, LOW);
  pinMode(pin_rot_a, OUTPUT);
  pinMode(pin_rot_b, OUTPUT);
  pinMode(pin_rot_sw, OUTPUT);

  pinMode(pin_rs485_rx, INPUT);
  pinMode(pin_rs485_tx, OUTPUT);
  pinMode(pin_rs485_en, OUTPUT);
  digitalWrite(pin_rs485_en, LOW);
}

void loop()
{
  fsm.run_machine(); // Do Finite State Machine tasks

  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,20,"BITCH I'M A BUS");
  } while ( u8g2.nextPage() );
  delay(1000);
}