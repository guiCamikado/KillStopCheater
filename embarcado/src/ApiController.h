// Classe responsavel pelas apis
#pragma once

#include <Arduino.h>
#include <JsonHandler.h>
#include <WebServer.h>
#include <WiFi.h>

class ApiController {
private:
  void loadDefaultHttpOptionsConfig() // Define configurações de rota
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  }
  String getArgs() // Recebe Post
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");

    // Wip
    Serial.println("--- /control POST recebido ---");
    Serial.print("Num args: ");
    Serial.println(server.args());

    if (!server.hasArg("plain")) {
      Serial.println("Erro: sem body");
      server.send(400, "application/json", "{\"error\":\"no body\"}");
      return "{}";
    }
    String json = server.arg("plain");
    Serial.print("Body: ");
    Serial.println(json);
    return json;
  }

  unsigned long _hitscore;         // Tempo que se deverá acertar
  String _measurementUnit;         // Unidade de medida para calculo
  unsigned long _pictureTimeFrame; // Tempo da foto digitada manualmente
  unsigned long _pictureTime;      // Tempo em que foto foi tirada desde a abertura da página
  unsigned long _sentTime;         // Tempo em que foto foi enviada desde a abertura da página
  unsigned long _timeScored;       // Tempo em que foi acertado ("Se maior que hitscore deverá subtrair se menor somar a
                                   // diferença para próxima tentativa")

public:
  WebServer server;
  JsonHandler json;

  void initiateRoutes() {

    server.on("/", HTTP_GET, [this]() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", "API ONLINE");
    });

    server.on("/control", HTTP_OPTIONS, [this]() { this->loadDefaultHttpOptionsConfig(); });

    server.on("/control", HTTP_POST, [this]() {
      const String data = this->getArgs();

      JsonObject itemsArray = json.stringToObject(data);

      Serial.println(data);

      this->_hitscore = itemsArray["hitscore"].as<unsigned long>();
      this->_measurementUnit = itemsArray["measurementUnity"].as<String>();
      this->_pictureTimeFrame = itemsArray["pictureTimeFrame"].as<unsigned long>();
      this->_pictureTime = itemsArray["pictureTime"].as<unsigned long>();
      this->_sentTime = itemsArray["sendTime"].as<unsigned long>();
      this->_timeScored = itemsArray["timeScored"].as<unsigned long>();

      Serial.println("Executou até aqui!!");

      server.send(200, "application/json", "{\"success\":true}");
    });

    server.begin();

    Serial.println("Servidor HTTP iniciado");
  }
  void handle() { server.handleClient(); }
};
