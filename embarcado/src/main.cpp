#include <Arduino.h>

#include "ApiController.h"
#include "ServoMotor.h"
#include "WifiConnection.h"

int SERVO_SIGNAL_PIN = 13;
WifiConnection wifi;
ApiController api;
// ServoMotor servo;


void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "12345678");
  wifi.connectWifi("Camikado2.4-5GHz", "a7a9y6A2@");
  
}

void loop() { wifi.handle(); }
