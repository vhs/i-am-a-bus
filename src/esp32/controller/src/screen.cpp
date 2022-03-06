#include "screen.hpp"

void initScreen() {
    u8g2.begin();
    u8g2.setFlipMode(1);
}

void clearScreen() {
    u8g2.clear();
}

void drawBootScreen(String text)
{
    u8g2.firstPage();

    do {
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);

        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, u8g2.getHeight() - 2, text.c_str());

        delay(0);
    } while (u8g2.nextPage());
}

void drawMainScreen(String text)
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);

        // u8g2.drawLine(UBG2_FRAME_X1, u8g2.getHeight() - 16, 120, u8g2.getHeight() - 16);

        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, u8g2.getHeight() - 2, text.c_str());

        delay(0);
    } while (u8g2.nextPage());
}

void drawMenu(String text)
{
    u8g2.firstPage();

    do {
        // u8g2.drawBox(0,0,128,64);
        u8g2.setFont(U8G2_TITLE_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, U8G2_TITLE_FONT_OFFSET, U8G2_TITLE_TEXT);

        u8g2.setFont(U8G2_MENU_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, u8g2.getHeight() - 16, text.c_str());

        u8g2.drawLine(UBG2_FRAME_X1, u8g2.getHeight() - 16, 120, u8g2.getHeight() - 16);

        u8g2.setFont(U8G2_TEXT_FONT);
        u8g2.drawStr(UBG2_FRAME_X1, u8g2.getHeight() - 2, signText.c_str());

        delay(0);
    } while (u8g2.nextPage());
}

void displayMainScreen() {
    // clearScreen();
    drawMainScreen(signText);
}