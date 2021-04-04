#include "platform.h"

#include "config.h"

/* Constructor  for screen */
U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R0);

SignBus signBus(PIN_RS485_RX, PIN_RS485_TX, PIN_RS485_EN);

// fsm states
FunctionState state_boot(&FSM_STATE_Boot_start, nullptr, nullptr);
FunctionState state_rest(&FSM_STATE_Rest_start, &FSM_STATE_Rest_loop, &FSM_STATE_Rest_stop);
FunctionState state_select(&FSM_STATE_Select_start, &FSM_STATE_Select_loop, &FSM_STATE_Select_stop);
FunctionState state_program(&FSM_STATE_Program_start, &FSM_STATE_Program_loop, &FSM_STATE_Program_stop);

// fsm
FunctionFsm fsm(&state_boot);

void setup()
{
  Serial.begin(115200);
  Serial.flush();
  Serial.println();
  Serial.println("Bus Sign Control");
  Serial.println();
  Serial.flush(); // Get serial all nice and ready, with some new lines.

  u8g2.begin(); /* u8g2.begin() is required and will sent the setup/init sequence to the display */

  pinMode(PIN_LED_GREEN, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  pinMode(PIN_ROT_A, OUTPUT);
  pinMode(PIN_ROT_B, OUTPUT);
  pinMode(PIN_ROT_SW, OUTPUT);

  pinMode(PIN_RS485_RX, INPUT);
  pinMode(PIN_RS485_TX, OUTPUT);
  pinMode(PIN_RS485_EN, OUTPUT);
  digitalWrite(PIN_RS485_EN, LOW);
}

void loop()
{
  fsm.run_machine(); // Do Finite State Machine tasks

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0, 20, "BITCH I'M A BUS");
  } while (u8g2.nextPage());
  delay(1000);
}