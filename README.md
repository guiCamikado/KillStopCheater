## uTrigger

###### English

#### About

This application is divided into two parts: the front-end, developed in React and used as the interface for interacting with the firmware, and the firmware running on an ESP32 device. Together, they form a Wi-Fi controlled IoT system with a lightweight web interface designed to trigger an electrical signal at a precisely timed moment.

The device acts as a precision timing mechanism with an accuracy of approximately 0.005 milliseconds (5 microseconds). It operates by receiving a target timestamp and activating an output signal once the specified time is reached.

#### working mechanism:

1. <b>main.cpp</b> - Starts the application with one of the functions startWifi (for AP network mode) or connectToWifi (for STA network mode).
2. Once connected, it starts the API. The API supports both HTTPS (port 443) and HTTP (port 80). HTTPS is used because smartphones usually do not allow data exchange over insecure connections.
3. After the connection is established, the user can access the interface using the device IP address (AP: 192.168.1.1, STA: depends on the network configuration).
4. The interface is simple and includes a Target Time configuration ("Tempo Alvo"), a correction time configuration ("Tempo após tentativa"), and a destination address that should be configured with "/control" after the IP address (example: 192.168.1.1/control).
5. Once configured and triggered, a signal is sent to the API, which checks whether the device should start or restart.
6. When started, the system uses the <b>Piston.h</b> class to control <b>GPIO pins 27 and 26</b>, firing a signal once 5 seconds after activation and again after a calculated delay based on the target timestamp adjusted by the reaction time.

#### Setup

Since the application was maded using PlatformIO the setup tutorial will be maded using it.

First you need to have PlatformIO extension installed in VsCode.

1. Open the terminal inside the "embarcado" file and type "code ."
2. PlatformIo will start to download the dependecies.
3. You are going to need to config the main.cpp with one of the connections

```cpp
wifi.startWifi("SSID", "Secret_Password"); // For AP
wifi.connectToWifi("SSID", "Secret_Password"); // For STA
```

you can also create an archive following this format and import latter:

```cpp
// .keys/secretKeys.h

const char STA_SSID[] = "SSID";
const char STA_PASSWORD[] = "PASSWORD";

const char AP_SSID[] = "SSID";
const char AP_PASSWORD[] = "PASSWORD";
```

4. Once the dependecies are installed and the application is configured you can use the build button that is found bellow as a checkmark "✔️"
5. After building you can ship the firmware into the ESP32 using the right arrow icon "➡️" founded besides the checkmark icon "✔️".

---

#### Sobre

###### Português

Esta aplicação é dividida em duas partes: o front-end, desenvolvido em React e utilizado como interface para interação com o firmware, e o firmware executado em um dispositivo ESP32. Juntos, eles formam um sistema IoT controlado por Wi-Fi com uma interface web leve projetada para disparar um sinal elétrico em um momento preciso.

O dispositivo funciona como um mecanismo de temporização de alta precisão, com uma exatidão aproximada de 0,005 milissegundos (5 microssegundos). Seu funcionamento consiste em receber um instante de tempo alvo e ativar um sinal de saída assim que o momento especificado for atingido.

#### Funcionamento:

1. <b>main.cpp</b> - Inicializa a aplicação utilizando uma das funções: startWifi (modo AP) ou connectToWifi (modo STA).

2. Após estabelecer conexão, a API é iniciada. Ela suporta HTTPS (porta 443) e HTTP (porta 80). O HTTPS é utilizado porque smartphones normalmente não permitem troca de dados por conexões inseguras.

3. Após a conexão ser estabelecida, o usuário pode acessar a interface utilizando o endereço IP do dispositivo (AP: 192.168.1.1, STA: depende da configuração da rede).

4. A interface é simples e contém uma configuração de Tempo Alvo, uma configuração de correção de tempo (Tempo após tentativa) e um endereço de destino que deve ser configurado com "/control" ao final do IP (exemplo: 192.168.1.1/control).

5. Após a configuração e acionamento, um sinal é enviado para a API, que verifica se o dispositivo deve iniciar ou reiniciar.

6. Quando iniciado, o sistema utiliza a classe <b>Piston.h</b> para controlar os <b>GPIOs 27 e 26</b>, disparando um sinal uma primeira vez após 5 segundos da ativação e novamente após um atraso calculado com base no instante alvo ajustado pelo tempo de reação.
