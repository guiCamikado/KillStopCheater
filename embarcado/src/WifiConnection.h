#pragma once

#include <Arduino.h>
#include <JsonHandler.h>
#include <WebServer.h>
#include <WiFi.h>

#include <ApiController.h>

class WifiConnection {
private:
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
  void loadDefaultHttpOptionsConfig() // Define configurações de rota
  {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "POST, OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
    server.send(204);
  }

public:
  WebServer server;
  JsonHandler json;
  ApiController api;

  WifiConnection() : server(80) {}

  void startWifi(const char *ssid, const char *password) {
    IPAddress ip(192, 168, 1, 1);
    IPAddress gateway(192, 168, 0, 1);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(ip, gateway, subnet);
    WiFi.softAP(ssid, password);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
  }

  void connectWifi(const char *ssid, const char *password) {
    WiFi.mode(WIFI_STA);
    Serial.print("Conectando em: ");
    Serial.println(ssid);
    Serial.println(password);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println();
    Serial.println("WiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    api.initiateRoutes();
  }

  void handle() { api.handle(); }
};