// Classe responsavel pelas apis
#pragma once

#include <Arduino.h>
#include <JsonHandler.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "ServoMotor.h"

class ApiController {
private:
  String _measurementUnit;         // Unidade de medida para calculo
  unsigned long _hitscore;         // Tempo que se deverá acertar
  unsigned long _pictureTimeFrame; // Tempo da foto digitada manualmente
  unsigned long _pictureTime;      // Tempo em que foto foi tirada desde a abertura da página
  unsigned long _sentTime;         // Tempo em que foto foi enviada desde a abertura da página
  unsigned long _timeScored;       // Tempo em que foi acertado ("Se maior que hitscore deverá subtrair se menor somar a
                                   // diferença para próxima tentativa")

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

    // Serial.println("--- /control POST recebido ---");
    // Serial.print("Num args: ");
    // Serial.println(server.args());

    if (!server.hasArg("plain")) {
      Serial.println("Erro: sem body");
      server.send(400, "application/json", "{\"error\":\"no body\"}");
      return "{}";
    }
    String json = server.arg("plain");
    return json;
  }

public:
  WebServer server;
  JsonHandler json;
  ServoMotor servo{25};

  unsigned long timeLeftToTrigger;

  // ApiController() : servo(25) {}

  void initiateRoutes() {
    LittleFS.begin(true);
    server.serveStatic("/", LittleFS, "/");
    // server.on("/", HTTP_GET, [this]() {
    //   server.sendHeader("Access-Control-Allow-Origin", "*");
    //   server.send(200, "text/plain", "API ONLINE");
    // });

    server.on("/control", HTTP_OPTIONS, [this]() { this->loadDefaultHttpOptionsConfig(); });
    server.on("/control", HTTP_POST, [this]() {
      const String data = this->getArgs();

      JsonObject itemsArray = json.stringToObject(data);

      _hitscore = itemsArray["hitscore"].as<unsigned long>();
      _measurementUnit = itemsArray["measurementUnity"].as<String>();
      _pictureTimeFrame = itemsArray["pictureTimeFrame"].as<unsigned long>();
      _pictureTime = itemsArray["pictureTime"].as<unsigned long>();
      _sentTime = itemsArray["sendTime"].as<unsigned long>();
      _timeScored = itemsArray["timeScored"].as<unsigned long>();

      timeLeftToTrigger = servo.cheatLogic(_hitscore, _pictureTime, _sentTime, _pictureTimeFrame, _timeScored);

      // servo.scheduleKillCheat(timeLeftToTrigger, 25, 180);
      // servo.moveFoward(); //WIP
      // delay(5000);
      // servo.moveBackward();

      server.send(200, "application/json", "{\"success\":true}");
    });

    server.onNotFound([this]() {
      File file = LittleFS.open("/index.html", "r");

      if (!file) {
        server.send(404, "text/plain", "index.html not found");
        return;
      }

      server.streamFile(file, "text/html");
      file.close();
    });
    server.begin();
    // Serial.println("Servidor HTTP iniciado");
  }

  void handle() {
    server.handleClient();
    servo.update();
  }
};
