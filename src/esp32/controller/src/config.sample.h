#pragma once

#define PIN_RS485_RX 18
#define PIN_RS485_TX 19
#define PIN_RS485_EN 26 // RX=Low, TX=High

#define PIN_LED_GREEN 4
#define PIN_LED_RED 23

#define PIN_ROT_A 14
#define PIN_ROT_B 27
#define PIN_ROT_SW 13

// We actually use hardware defaults for I2C, but defined here for completeness
#define PIN_I2C_SDA 21
#define PIN_I2C_SCL 22

// #define WIFI_MODE_CLIENT
#define WIFI_SSID "Wireless Network Name"
#define WIFI_PASS "PleasePickASecurePassword"

#define HOST_NAME "mobile-bus-controller"

#define HTTP_USER "admin"
#define HTTP_PASS "admin"

#define U8G2_TITLE_FONT u8g2_font_ncenB14_tr
#define U8G2_TITLE_TEXT "i-am-a-bus v1"
#define U8G2_TITLE_FONT_OFFSET 12

#define U8G2_MENU_FONT u8g2_font_helvR08_tf

#define U8G2_TEXT_FONT u8g2_font_helvR08_tf 
#define U8G2_TEXT_FONT_HEIGHT 16
#define U8G2_TEXT_BEVEL_WIDTH 2
#define UBG2_TEXT_OFFSET 63

#define U8G2_BASE_X 0
#define U8G2_BASE_Y 0
#define U8G2_WIDTH 128
#define U8G2_HEIGHT 64
#define UBG2_FRAME_X1 1
#define UBG2_FRAME_LENGTH 120

#define UBG2_FRAME_Y1 UBG2_TEXT_OFFSET - (U8G2_TEXT_FONT_HEIGHT + U8G2_TEXT_BEVEL_WIDTH)
#define UBG2_FRAME_X2 UBG2_FRAME_LENGTH + U8G2_TEXT_BEVEL_WIDTH
#define UBG2_FRAME_Y2 U8G2_TEXT_FONT_HEIGHT + U8G2_TEXT_BEVEL_WIDTH
#define UBG2_TEXT_OFFSET_X U8G2_TEXT_BEVEL_WIDTH
#define UBG2_TEXT_OFFSET_Y UBG2_TEXT_OFFSET - (U8G2_TEXT_FONT_HEIGHT + U8G2_TEXT_BEVEL_WIDTH) + U8G2_TEXT_FONT_HEIGHT

#define DEBOUNCE_DELAY 50

#define SCREENSAVER_TIMEOUT 30