#include "platform.h"

#include "screen.h"
#include "io.h"
#include "webserver.h"
#include "fsm.h"
#include "sign.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.flush();
  Serial.println();
  Serial.println("Bus Sign Control");
  Serial.println();
  Serial.flush(); // Get serial all nice and ready, with some new lines.

  initScreen();

  drawBootScreen("Initializing...");

  Serial.println("Initializing IO...");

  setupPins();

  drawBootScreen("Initializing WiFi...");

  Serial.print("Initializing WiFi...");

#ifdef WIFI_MODE_CLIENT
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(65);
  }

  IPAddress myIP = WiFi.localIP();
#else
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
  IPAddress myIP = WiFi.softAPIP();
#endif
  Serial.println("OK");
  Serial.println("");
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  drawBootScreen("Initializing FS...");

  SPIFFS.begin();

  drawBootScreen("Initializing webserver...");

  webserver_start();

  drawBootScreen("Initializing FSM...");

  FSM_Init();

  Serial.println("Finished setup");

  Serial.printf("U8G2_BASE_X: %d\n", U8G2_BASE_X);
  Serial.printf("U8G2_BASE_Y: %d\n", U8G2_BASE_Y);
  Serial.printf("U8G2_HEIGHT: %d\n", U8G2_HEIGHT);
  Serial.printf("U8G2_TEXT_FONT: %s\n", U8G2_TEXT_FONT);
  Serial.printf("U8G2_TITLE_FONT: %s\n", U8G2_TITLE_FONT);
  Serial.printf("U8G2_TITLE_FONT_OFFSET: %d\n", U8G2_TITLE_FONT_OFFSET);
  Serial.printf("U8G2_WIDTH: %d\n", U8G2_WIDTH);
  Serial.printf("UBG2_FRAME_Y2: %d\n", UBG2_FRAME_Y2);
  Serial.printf("UBG2_FRAME_X1: %d\n", UBG2_FRAME_X1);
  Serial.printf("UBG2_FRAME_Y1: %d\n", UBG2_FRAME_Y1);
  Serial.printf("UBG2_FRAME_X2: %d\n", UBG2_FRAME_X2);
  Serial.printf("UBG2_TEXT_OFFSET_X: %d\n", UBG2_TEXT_OFFSET_X);
  Serial.printf("UBG2_TEXT_OFFSET_Y: %d\n", UBG2_TEXT_OFFSET_Y);
  Serial.println("");
  Serial.printf("Buffer Tile Height: %d\n", u8g2.getBufferTileHeight());
  Serial.printf("Buffer Tile Width: %d\n", u8g2.getBufferTileWidth());
  Serial.printf("Total Buffer Size: %d\n", (8 * u8g2.getBufferTileHeight() * u8g2.getBufferTileWidth()));
}

void loop()
{
  unsigned long startTime = millis();

  ArduinoOTA.handle();

  // Do Finite State Machine tasks
  fsm.run_machine();

  if (dirtyConfig != "")
  {
    writeConfig();
  }

  if (syncSign)
  {
    drawSign();

    syncSign = false;
  }

  unsigned long endTime = millis();
  unsigned long elapsedTime = endTime - startTime;

  if (elapsedTime > 50)
    Serial.printf("Loop time: %sms\n", String(elapsedTime));
}
