#include <Arduino.h>

#include "ApiController.h"
#include "WifiConnection.h"
#include <ESP32Servo.h>

WifiConnection wifi;
// ApiController api;
Servo servo;

void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "12345678");
  wifi.connectWifi("Camikado2.4-5GHz", "a7a9y6A2@");
  wifi.api.servo.begin();
  wifi.api.initiateRoutes();
}

// void setup() {
//   Serial.begin(115200);
//   servo.setPeriodHertz(50);
//   servo.attach(25, 500, 2400);
//   delay(1000);
//   servo.write(90);
//   delay(2000);
//   servo.write(0);
// }

void loop() {
  wifi.handle();
}
