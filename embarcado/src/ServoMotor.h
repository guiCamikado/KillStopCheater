#include <ESP32Servo.h>
#include <Utils.h>

// Classe referente ao uso do servoMotor
class ServoMotor {
private:
  /* data */
  Servo servo;
  Utils utils;
  int _defaultDegree;
  unsigned long _triggerTime;
  bool _waitingTrigger = false;

public:
  // O servo deverá ser instanciado pela classe "main"
  // Quando instanciado deverá ser especificado o pino conectado

  ServoMotor(int signalPin, int defaultDegree) {
    _defaultDegree = defaultDegree;
    servo.setPeriodHertz(50);
    servo.attach(signalPin, 500, 2400); // WIP verificar isso
  }

  void cheatLogic(float hitscore, float pictureTime, float sentTime, float pictureTimeFrame, float timeScored,
                  String measurementUnit) {
    float timePassed;
    // É importante ressaltar que o tempo que a foto é tirada e o tempo de envio estão em microsegundos
    // 1seg = 1000ms = 1.000.000us
    // 1ms = 1.000us

    // Obtem valor do tempo que passou desde a ativação da função.
    if (timeScored) {
      // Obtem valor dentre o tempo que o usuário digitou e o tempo de fato (Levando em conta a taxa de erro)
      timePassed =
          ((pictureTimeFrame * 1000) - (sentTime - pictureTime) + ((hitscore * 1000) - (timeScored * 1000))) / 1000;
    } else {
      // Obtem valor dentre o tempo que o usuário digitou e o tempo de fato (sem levar em conta a taxa de erro)
      timePassed = (pictureTimeFrame * 1000) - (sentTime - pictureTime) / 1000;
    }

    // Trata valores us -> ms -> s
    if (measurementUnit == "microsecond") { timePassed = timePassed; }
    if (measurementUnit == "milisecond") { timePassed = timePassed / 1000; }
    if (measurementUnit == "second") { timePassed = timePassed / 1000000; }

    // Cria um gatilho para um countdown que ativará o Servomotor
    _triggerTime = micros() + (timePassed * 1000);
    _waitingTrigger = true;
    servo.write(this->_defaultDegree);
  }

  void update() {
    if (_waitingTrigger) {
      if ((long)(micros() - _triggerTime) >= 0) {
        _waitingTrigger = false;

        servo.write(_defaultDegree);
        Serial.println("Servo ativado!!");
      }
    }

    void moveFoward(int degree) { servo.write(degree); }

    void moveBackward(int degree) { servo.write(degree * -1); }
  };