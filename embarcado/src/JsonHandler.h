#pragma once
#include <ArduinoJson.h>

JsonDocument config;

class JsonHandler {
public:
  JsonObject stringToObject(const String &jsonString) {
    DeserializationError error = deserializeJson(config, jsonString);
    if (error) {
      Serial.println("Erro ao ler JSON");
    }
    return config.as<JsonObject>();
  }
};
