#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <JsonHandler.h>

#include "html.h"

class WifiConnection
{
private:
	String getArgs() // Recebe Post
	{
		server.sendHeader("Access-Control-Allow-Origin", "*");

		// Wip
		Serial.println("--- /control POST recebido ---");
		Serial.print("Num args: ");
		Serial.println(server.args());

		if (!server.hasArg("plain"))
		{
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
	int hitscore;						// Tempo que se deverá acertar
	String measurementUnit; // Unidade de medida para calculo
	int pictureTimeFrame;		// Tempo da foto digitada manualmente
	int pictureTime;				// Tempo em que foto foi tirada desde a abertura da página
	int sendTime;						// Tempo em que foto foi enviada desde a abertura da página
	int timeScored;					// Tempo em que foi acertado ("Se maior que hitscore deverá subtrair se menor somar a diferença para próxima tentativa")

	WebServer server;
	JsonHandler json;

	WifiConnection() : server(80) {}

	void startWifi(const char *ssid, const char *password)
	{
		IPAddress ip(192, 168, 1, 1);
		IPAddress gateway(192, 168, 0, 1);
		IPAddress subnet(255, 255, 255, 0);

		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(ip, gateway, subnet);
		WiFi.softAP(ssid, password);

		Serial.print("AP IP: ");
		Serial.println(WiFi.softAPIP());

		server.on("/", HTTP_GET, [this]()
							{
            server.sendHeader("Access-Control-Allow-Origin", "*");
            server.send(200, "text/html", INDEX_HTML); });

		server.on("/control", HTTP_OPTIONS, [this]()
							{ this->loadDefaultHttpOptionsConfig(); });
		server.on("/control", HTTP_POST, [this]()
							{ this->getArgs(); });

		server.begin();
		Serial.println("Servidor HTTP iniciado");
	}

	void connectWifi(const char *ssid, const char *password)
	{
		WiFi.mode(WIFI_STA);
		Serial.print("Conectando em: ");
		Serial.println(ssid);
		Serial.println(password);
		WiFi.begin(ssid, password);

		while (WiFi.status() != WL_CONNECTED)
		{
			delay(500);
			Serial.print(".");
		}
		Serial.println();
		Serial.println("WiFi conectado!");
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());

		// WIP trabalhar em cima daqui agora, a lógica de ativação do servomotor!!
		server.on("/control", HTTP_OPTIONS, [this]()
							{ this->loadDefaultHttpOptionsConfig(); });

		server.on("/control", HTTP_POST, [this]()
							{
								const String data = this->getArgs();
								JsonObject itemsArray = json.stringToObject(data);
								Serial.println(data);

								hitscore = itemsArray["hitscore"].as<int>() 										| 0;
								measurementUnit = itemsArray["measurementUnity"].as<String>();
								pictureTimeFrame = itemsArray["pictureTimeFrame"].as<int>() 		| 0;
								pictureTime = itemsArray["pictureTime"].as<int>() 							| 0;
								sendTime = itemsArray["sendTime"].as<int>() 										| 0;
								timeScored = itemsArray["timeScored"].as<int>() 								| 0; 
							
								Serial.println("Executou até aqui!!"); });

		server.begin();
		Serial.println("Servidor HTTP iniciado");
	}

	// Precisa ser utilizado no loop
	void handle()
	{
		server.handleClient();
	}

	int returnData()
	{
	}
};