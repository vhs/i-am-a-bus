#include "platform.h"

#include "externals/screen.h"
#include "externals/network.h"
#include "externals/fsm.h"
#include "externals/sign.h"
#include "externals/util.h"
#include "externals/screensaver.h"

void setup()
{
  FSM_Init();
}

void loop()
{
  unsigned long startTime = millis();

  network_loop();

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
    Serial.printf("Loop time: %sms\n", String(elapsedTime).c_str());
}
