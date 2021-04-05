#include <FS.h>
#include <SPIFFS.h>

String dirtyConfig = "";
String dirtyText = "";

void writeConfig()
{
    Serial.printf("Writing config...\n");

    File file = SPIFFS.open("/config.json", FILE_WRITE);

    yield();

    if (!file)
    {
        Serial.println("- failed to open config.json for writing");
        return;
    }

    if (file.print(dirtyConfig.c_str()))
        Serial.println("- config.json written");
    else
        Serial.println("- write failed");

    yield();

    file.close();

    dirtyConfig = "";
}
