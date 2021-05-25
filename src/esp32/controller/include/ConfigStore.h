#pragma once

#include <string.h>
#include <list>

#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

#define MAX_TEXT_OPTIONS 16
#define CONFIG_TEXTS_OPTIONS_TYPE std::list<String>

DynamicJsonDocument doc(2048);
char jsonOutBuffer[2048];

struct ConfigTexts
{
    String current;
    CONFIG_TEXTS_OPTIONS_TYPE options;
};

struct Config
{
    ConfigTexts texts;
};

class ConfigStore
{
private:
    String _displayText = "";
    bool _dirtyText = false;
    bool _dirtyConfig = false;
    Config _config;

    ConfigStore()
    {
        _loadConfig();
    }

    void _loadConfig()
    {
        File file = SPIFFS.open("/config.json", FILE_READ);

        if (!file)
        {
            Serial.println("- failed to open config.json for writing");
            return;
        }

        DeserializationError error = deserializeJson(doc, file);

        if (error)
            Serial.println(F("Failed to read file, using default configuration"));

        _config.texts.current = doc["texts"]["current"].as<String>();

        int optionsSize = doc["texts"]["options"].size();

        for (int o = 0; o < optionsSize; o++)
        {
            _config.texts.options.push_back(doc["texts"]["options"][o].as<String>());
        }
    }

    bool _saveConfig()
    {
        if (isConfigDirty())
        {
            Serial.printf("Writing config...\n");

            doc.clear();

            JsonObject texts = doc.createNestedObject("texts");
            texts["current"] = _config.texts.current;

            JsonArray texts_options = texts.createNestedArray("options");

            for (auto &option : _config.texts.options)
            {
                texts_options.add(option);
            }

            yield();

            Serial.printf("Opening config file...\n");
            File file = SPIFFS.open("/config.json", FILE_WRITE);

            yield();

            if (!file)
            {
                Serial.println("- failed to open config.json for writing");
                return;
            }

            serializeJson(doc, jsonOutBuffer);

            yield();

            if (file.print(jsonOutBuffer))
                Serial.println("- config.json written");
            else
                Serial.println("- write failed");

            yield();

            memset(&jsonOutBuffer[0], 0, sizeof(jsonOutBuffer));

            file.close();

            _dirtyConfig = false;
        }
    }

public:
    static ConfigStore &getInstance()
    {
        static ConfigStore _instance;

        return _instance;
    }

    String getCurrentText()
    {
        return _config.texts.current;
    }

    void setCurrentText(String currentText)
    {
        if (_config.texts.current != currentText)
        {
            _config.texts.current = currentText;
            _dirtyConfig = true;
            _dirtyText = true;
        }
    }

    bool isTextDirty()
    {
        return _dirtyText;
    }

    bool isConfigDirty()
    {
        return _dirtyConfig;
    }

    bool save()
    {
        return _saveConfig();
    }

    void deleteTextsOption(int optIdx)
    {
        CONFIG_TEXTS_OPTIONS_TYPE::iterator it = _config.texts.options.begin();
        std::advance(it, optIdx);
        _config.texts.options.erase(it);
        _dirtyConfig = true;
    }

    void addTextsOption(String newOpt)
    {
        _config.texts.options.push_back(newOpt);
        _config.texts.options.unique();
        _config.texts.options.sort();
        _dirtyConfig = true;
    }
};