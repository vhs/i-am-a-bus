#ifndef IAMABUS_PLATFORM_H
#define IAMABUS_PLATFORM_H

#include <Arduino.h>

#include <ArduinoOTA.h>

#ifdef ESP32
#include <ESPmDNS.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESP8266mDNS.h>
#endif

#include <FS.h>
#include <SPIFFS.h>

#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>

#include <RotaryEncoder.h>

#include <U8g2lib.h>

#include <ArduinoJson.h>

#include <FunctionFSM.h>

#include "config.h"

#include "constants.h"

#endif
