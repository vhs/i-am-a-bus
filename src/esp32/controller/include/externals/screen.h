// #undef INTERFACE
#if !defined(IAMABUS_SCREEN_H)

void displayMainScreen();
void drawMenu(String text);
void drawMainScreen(String text);
void drawBootScreen(String text);
void clearScreen();
void initScreen();

extern U8G2_SSD1309_128X64_NONAME2_F_HW_I2C u8g2;

#endif
