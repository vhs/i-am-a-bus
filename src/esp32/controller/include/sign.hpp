#ifndef IAMABUS_SIGN_H
#define IAMABUS_SIGN_H

#include "platform.h"

#include "externals/screen.h"
#include "externals/util.h"

#include <SignBus.hpp>

SignBus signBus(PIN_RS485_RX, PIN_RS485_TX, PIN_RS485_EN);

#endif