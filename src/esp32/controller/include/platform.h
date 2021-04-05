#pragma once

#include <Arduino.h>

#include <SoftwareSerial.h>

#include <SPI.h>
#include <U8g2lib.h>
#include <FunctionFSM.h>

#include "util.h"

#include "SignBus.h"

#include "webserver.h"

#include "FSM.h"

#include "sign_static.h"

#include "config.h"

/* Constructor  for screen */
U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R0);
