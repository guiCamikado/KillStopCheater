#pragma once

#include <ESP32Servo.h>

// Classe referente ao uso do servoMotor
class ServoMotor {
private:
  Servo servo;
  unsigned long _timeToTrigger = 0;
  int _defaultSpin = 180;
  int _signalPin = 0;
  bool waitingReturn = false;
  unsigned long _timeToReturn = 0;

public:
  bool waitingTrigger = false;

  // Construtor padrão (necessário para instanciar sem argumentos)
  ServoMotor() : _signalPin(0) {}

  // Construtor com pino
  ServoMotor(int signalPin) : _signalPin(signalPin) {}

  void begin() {
    servo.setPeriodHertz(50);
    servo.attach(_signalPin, 500, 2400);
  }

  unsigned long cheatLogic(long hitscore, long pictureTime, long sentTime, long pictureTimeFrame, long timeScored) {
    long timePassed;

    if (timeScored && timeScored != 0) {
      timePassed = (((sentTime - pictureTime) + (pictureTimeFrame * 1000))) + (timeScored * 1000 - hitscore * 1000);
    } else {
      timePassed = ((sentTime - pictureTime) + (pictureTimeFrame * 1000));
    }

    unsigned long timeLeft = (hitscore * 1000) - timePassed + micros();
    _timeToTrigger = timeLeft;

    // Serial.println("===================DATA SERVO========================");
    // Serial.print("Variavel timePassed: ");
    // Serial.println(timePassed);
    // Serial.print("Variavel pictureTimeFrame: ");
    // Serial.println(pictureTimeFrame);
    // Serial.print("Tempo decorrido dentre foto e envio: ");
    // Serial.println((sentTime - pictureTime));
    // Serial.println("_______________________________________");
    // Serial.print("tempo para ativar (timeLeft): ");
    // Serial.println(timeLeft);
    // Serial.println("=====================================================");

    waitingTrigger = true;
    return timeLeft;
  }

  // Agenda o acionamento do servo — chamar scheduleKillCheat() + update() no loop()
  void scheduleKillCheat(unsigned long timeToTrigger, int signalPin, int defaultSpin) {
    _timeToTrigger = timeToTrigger;
    _signalPin = signalPin;
    _defaultSpin = defaultSpin;
    waitingTrigger = true;
  }

  // Chamar no loop() — NÃO bloqueante
  void update() {
    if (!waitingTrigger) return;

    if ((long)(micros() - _timeToTrigger) >= 0) {
      waitingTrigger = false;
      servo.write(_defaultSpin);
      _timeToReturn = micros() + 5000000UL; // 5seg.
      waitingReturn = true;
    }

    if (waitingReturn && (long)(micros() - _timeToReturn) >= 0) {
      waitingReturn = false;
      servo.write(0);
    }
  }

  void moveFoward() { servo.write(180); }
  void moveBackward() { servo.write(0); }
};
