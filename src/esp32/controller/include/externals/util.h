#if !defined(IAMABUS_UTIL_H)

#include "platform.h"

String getConfig();
int loadTexts(String* textArray);
String loadCurrentText();
void handleTextUpdate(void(*func)());
void writeConfig();
void halt(String msg);

extern bool syncSign;
extern String signText;
extern String dirtySignText;
extern String dirtyConfig;
extern bool isConfigLoaded;

#endif
