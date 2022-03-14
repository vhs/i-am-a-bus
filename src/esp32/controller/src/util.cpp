#include "util.hpp"

String getConfig() {
    String output;

    serializeJson(doc, output);

    return output;
}

void handleTextUpdate(void (*func)()) {
    if (dirtySignText == "" || dirtySignText == signText) {
        return;
    }

    signText = dirtySignText;
    dirtySignText = "";

    func();

    syncSign = true;
}

String loadCurrentText() {
    if (isConfigLoaded) return signText;

    Serial.println("loadCurrentText: opening FS to read config");

    File file = SPIFFS.open("/config.json");

    yield();

    if (!file)
    {
        Serial.println("- failed to open config.json for reading");
        return "config error";
    }

    String jsonContent = "";

    jsonContent = file.readString();

    file.close();

    deserializeJson(doc, jsonContent);

    const char* currentTextVal = doc["texts"]["default"];

    signText = String(currentTextVal);

    isConfigLoaded = true;

    return signText;
}

int loadTexts(String* textArray) {
    loadCurrentText();

    int size = 0;

    for (int i = 0; i < sizeof(textArray); i++) {
        Serial.println("loadText: " + String(i));

        textArray[i] = "";

        if (doc["texts"]["options"][i] != nullptr) {
            textArray[i] = doc["texts"]["options"][i].as<String>();

            size++;
        }
    }

    return size;
}

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

void halt(String msg) {
    drawBootScreen(msg);
    Serial.flush();
    while (true) {
        delay(1000);
    }
}