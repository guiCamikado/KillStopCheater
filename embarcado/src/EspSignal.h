#include <Arduino.h>>

class EspSignal {
private:
  int _ledPin = 2;
  unsigned long _time;
  bool _isLedOn = false;

public:
  void setup() {
    pinMode(_ledPin, OUTPUT);
    _time = millis();
  }
  void turnLedOnOff() {
    if (millis() - 500 >= _time) {
      digitalWrite(_ledPin, HIGH);
      _time = millis();
    } else {
      digitalWrite(_ledPin, LOW);
    }
  }

  void handle() { turnLedOnOff(); }
};