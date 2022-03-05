#include "webserver.hpp"

void webserver_start()
{
    //Send OTA events to the browser
    ArduinoOTA.onStart([]() {
        events.send("Update Start", "ota");
        }
    );

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        char p[32];
        sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
        events.send(p, "ota");
        }
    );

    ArduinoOTA.onEnd([]() {
        events.send("Update End", "ota");
        }
    );

    ArduinoOTA.onError([](ota_error_t error) {
        if (error == OTA_AUTH_ERROR)
            events.send("Auth Failed", "ota");
        else if (error == OTA_BEGIN_ERROR)
            events.send("Begin Failed", "ota");
        else if (error == OTA_CONNECT_ERROR)
            events.send("Connect Failed", "ota");
        else if (error == OTA_RECEIVE_ERROR)
            events.send("Recieve Failed", "ota");
        else if (error == OTA_END_ERROR)
            events.send("End Failed", "ota");
        }
    );
    ArduinoOTA.setHostname(HOST_NAME);
    ArduinoOTA.begin();

    MDNS.addService("http", "tcp", 80);

    events.onConnect([](AsyncEventSourceClient* client) {
        client->send("hello!", NULL, millis(), 1000);
        }
    );
    server.addHandler(&events);

#ifdef ESP32
    server.addHandler(new SPIFFSEditor(SPIFFS, HTTP_USER, HTTP_PASS));
#elif defined(ESP8266)
    server.addHandler(new SPIFFSEditor(HTTP_USER, HTTP_PASS));
#endif

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on(
        "/heap",
        HTTP_GET,
        [](AsyncWebServerRequest* request) {
            request->send(200, "text/plain", String(ESP.getFreeHeap()));
        }
    );

    server.on(
        "/api/getcurrenttext",
        HTTP_GET,
        [](AsyncWebServerRequest* request) {
            Serial.println("Get default text request...");

            request->send(200, "application/json", String("{\"result\":\"OK\",\"text\":\"" + signText + "\"}"));
        }
    );

    server.on(
        "/api/updatetext",
        HTTP_POST,
        [](AsyncWebServerRequest* request) {
            Serial.println("Update text request...");

            if (!request->hasParam("text", true, false))
            {
                Serial.println("Update text missing!");

                request->send(400, "application/json", String("{\"result\":\"ERROR\"}"));

                return;
            }

            AsyncWebParameter* updateTextParam = request->getParam("text", true, false);

            Serial.print("Text: ");
            Serial.println(updateTextParam->value());

            Serial.printf("Updating text to: %s\n", updateTextParam->value().c_str());

            dirtySignText = updateTextParam->value();

            request->send(200, "application/json", String("{\"result\":\"OK\",\"text\":\"" + signText + "\"}"));
        }
    );

    server.on(
        "/api/updateconfig",
        HTTP_POST,
        [](AsyncWebServerRequest* request) {
            Serial.println("Updating config...");

            if (!request->hasParam("config", true, false))
            {
                Serial.println("Update config missing!");

                request->send(400, "application/json", String("{\"result\":\"ERROR\"}"));

                return;
            }

            AsyncWebParameter* updateConfigParam = request->getParam("config", true, false);

            Serial.print("Config: ");
            Serial.println(updateConfigParam->value());

            Serial.printf("Updating config to: %s\n", updateConfigParam->value().c_str());

            dirtyConfig = updateConfigParam->value();

            request->send(200, "application/json", String("{\"result\":\"OK\"}"));
        }
    );

    server.onNotFound(
        [](AsyncWebServerRequest* request) {
            Serial.printf("NOT_FOUND: ");

            if (request->method() == HTTP_GET)
                Serial.printf("GET");
            else if (request->method() == HTTP_POST)
                Serial.printf("POST");
            else if (request->method() == HTTP_DELETE)
                Serial.printf("DELETE");
            else if (request->method() == HTTP_PUT)
                Serial.printf("PUT");
            else if (request->method() == HTTP_PATCH)
                Serial.printf("PATCH");
            else if (request->method() == HTTP_HEAD)
                Serial.printf("HEAD");
            else if (request->method() == HTTP_OPTIONS)
                Serial.printf("OPTIONS");
            else
                Serial.printf("UNKNOWN");

            Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

            request->send(404);
        }
    );

    server.begin();
}