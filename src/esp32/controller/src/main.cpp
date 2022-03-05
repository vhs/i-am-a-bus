#include "platform.h"

#include "externals/screen.h"
#include "externals/io.h"
#include "externals/webserver.h"
#include "externals/fsm.h"
#include "externals/sign.h"
#include "externals/util.h"
#include "externals/screensaver.h"

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

  checkScreenSaver();

  unsigned long endTime = millis();
  unsigned long elapsedTime = endTime - startTime;

  if (elapsedTime > 50)
    Serial.printf("Loop time: %sms\n", String(elapsedTime));
}
