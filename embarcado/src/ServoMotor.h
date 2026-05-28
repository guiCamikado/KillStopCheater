#pragma once

#include <ESP32Servo.h>

// Classe referente ao uso do servoMotor
class ServoMotor {
private:
  /* data */
  Servo servo;

public:
  // Construtor

  bool waitingTrigger = false;

  // WIP CORRIGIR!!! Essa função é crucial para o funcionamento da máquina!!
  static unsigned long cheatLogic(long hitscore, long pictureTime, long sentTime, long pictureTimeFrame,
                                  long timeScored, String measurementUnit) {
    Servo servo;
    // waitingTrigger = false;
    float timePassed;
    // Obtem valor do tempo que passou desde a ativação da função.
    if (timeScored) {
      timePassed = ((pictureTimeFrame * 1000) - (sentTime - pictureTime) + ((hitscore * 1000) - (timeScored * 1000)));
    } else {
      timePassed = ((pictureTimeFrame * 1000) - (sentTime - pictureTime));
    }

    // (WIP) Trata valores us -> ms -> s
    // if (measurementUnit == "microsecond") { timePassed = timePassed; }
    // if (measurementUnit == "milisecond") { timePassed = timePassed / 1000; }
    // if (measurementUnit == "second") { timePassed = timePassed / 1000000; }

    // Cria um gatilho para um countdown que ativará o Servomotor
    // unsigned long = micros() + (timePassed * 1000);
    // waitingTrigger = true;
    unsigned long timeLeft = (hitscore * 1000) - timePassed;

    Serial.println("===================DATA SERVO========================");
    Serial.print("Tempo para acertar apartir da foto:");
    Serial.println(((hitscore * 1000) - (timeScored * 1000)));
    Serial.print("Tempo decorrido do envio da foto:");
    Serial.println((sentTime - pictureTime));
    Serial.println("=====================================================");

    return timeLeft;
  }

  // Ativa servomotor
  void activateKillCheat(unsigned long timeToTrigger, int signalPin, int defaultSpin) {
    int _defaultSpin;
    servo.setPeriodHertz(50);
    servo.attach(signalPin, 500, 2400); // WIP verificar isso

    if (waitingTrigger) {
      if ((long)(micros() - timeToTrigger) >= 0) {
        waitingTrigger = false;

        servo.write(defaultSpin);
        Serial.println("Servo ativado!!");
      }
    }
  }
  void moveFoward(int degree) { servo.write(degree); }
  void moveBackward(int degree) { servo.write(degree * -1); }
};