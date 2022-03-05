#ifndef IAMABUS_UTIL_H
#define IAMABUS_UTIL_H

#include "platform.h"

DynamicJsonDocument doc(2048);

bool isConfigLoaded = false;

String dirtyConfig = "";
String dirtySignText = "";
String signText = "Initializing...";
String texts[16];

bool syncSign = false;

#endif