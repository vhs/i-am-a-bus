#include "platform.h"

#include "config.h"

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