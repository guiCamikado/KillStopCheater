class Utils {
private:
  static unsigned long startTime;
  static bool running;

public:
  static bool startTimer(unsigned long durationMs) {
    // Inicia o timer
    if (!running) {
      startTime = micros();
      running = true;
    }

    // Verifica tempo decorrido
    if (micros() - startTime >= durationMs * 1000) {
      running = false;
      return false;
    }

    return true;
  }
};

// Inicialização das variáveis estáticas
unsigned long Utils::startTime = 0;
bool Utils::running = false;