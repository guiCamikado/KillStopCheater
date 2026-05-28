#include <ArduinoJson.h>

DynamicJsonDocument config(2048);

class JsonHandler
{
public:
    JsonObject stringToObject(const String &jsonString)
    {
        DeserializationError error = deserializeJson(config, jsonString);
        if (error)
        {
            Serial.println("Erro ao ler JSON");
        }
        return config.as<JsonObject>();
    }
};