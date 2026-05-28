#include <Arduino.h>

#include "WifiConnection.h"

WifiConnection wifi;

void setup() {
  Serial.begin(115200);
  // wifi.startWifi("ESP32", "12345678");
  wifi.connectWifi("Camikado2.4-5GHz", "a7a9y6A2@");
}

void loop() { wifi.handle(); }
