// #undef INTERFACE
#if !defined(IAMABUS_MENU_H)

void resetSelectMenu();
void loadSelectOptions();
int handleSelectMenu();
void displaySelectMenu();
void resetMenu();
int handleMainMenu();
void displayMainMenu();

extern RotaryEncoder encoder;

#endif
