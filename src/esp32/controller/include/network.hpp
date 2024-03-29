#ifndef IAMABUS_WEBSERVER_H
#define IAMABUS_WEBSERVER_H

#include "platform.h"

#include "externals/screen.h"
#include "externals/util.h"

#define MAX_CONTENT_LENGTH 16384

AsyncWebServer server(80);
AsyncEventSource events("/events");

bool networkInit = false;
bool networkUp = false;

#endif
