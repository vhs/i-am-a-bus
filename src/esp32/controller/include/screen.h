#ifndef IAMABUS_SCREEN_H
#define IAMABUS_SCREEN_H

#include <U8g2lib.h>

#include "platform.h"

#include "config.h"

#include "util.h"

/* Constructor for screen */
U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2(U8G2_R2);

void initScreen() {
    u8g2.begin(); /* u8g2.begin() is required and will sent the setup/init sequence to the LCD display */
}

void clearScreen() {
    u8g2.clear();
}

void drawBootScreen(String text)
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(4, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);
        // u8g2.drawFrame(1, 63 - (16 + 2), 120 + 2, 16 + 2);
        u8g2.drawLine(UBG2_FRAME_X1, u8g2.getHeight() - 16, UBG2_FRAME_X2, u8g2.getHeight() - 16);
        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(4, u8g2.getHeight() - 2, text.c_str());
        delay(0);
    } while (u8g2.nextPage());
}

void drawMainScreen(String text)
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(4, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);
        // u8g2.drawFrame(1, 63 - (16 + 2), 120 + 2, 16 + 2);
        u8g2.drawLine(UBG2_FRAME_X1, u8g2.getHeight() - 16, UBG2_FRAME_X2, u8g2.getHeight() - 16);
        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(4, u8g2.getHeight() - 2, text.c_str());
        delay(0);
    } while (u8g2.nextPage());
}

void drawMenu(String text)
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(4, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);
        // u8g2.drawFrame(1, 63 - (16 + 2), 120 + 2, 16 + 2);
        u8g2.setFont(U8G2_MENU_FONT);
        u8g2.drawStr(4, u8g2.getHeight() - 16, text.c_str());
        // u8g2.drawLine(UBG2_FRAME_X1, u8g2.getHeight() - 16, UBG2_FRAME_X2, u8g2.getHeight() - 16);
        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(4, u8g2.getHeight() - 2, signText.c_str());
        delay(0);
    } while (u8g2.nextPage());
}

void displayMainScreen() {
    // clearScreen();
    drawMainScreen(signText);
}

#endif