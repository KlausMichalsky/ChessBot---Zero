├── config.h
│   ├─ Constantes globales (homing, etc.)
│   └─ Declaración de variables configurables (alpha, offsets, etc.)
│
├── config.cpp
│   └─ Definición de variables configurables
│
├── filtro.h
│   └─ Declaración de la función actualizarAngulo()
│
├── filtro.cpp
│   └─ Implementación de actualizarAngulo()
│
├── leds.h
│   ├─ Declaración de variables LED (lastLedToggle, ledState, etc.)
│   └─ Declaración de funciones LED (actualizarLed, parpadeoFinal)
│
├── leds.cpp
│   └─ Definición de variables LED e implementación de funciones
│
├── motores.h
│   └─ Declaración de funciones de control de motores (ej. actualizarAngulos)
│
├── motores.cpp
│   ├─ Implementación de funciones de control de motores
│   └─ Uso de: filtro.h, leds.h, config.h, sensors.h, communication.h según corresponda
│
├── sensores.h
│   ├─ Declaración de funciones y variables de sensores (ej. leer ángulo, hall, etc.)
│
├── sensores.cpp
│   └─ Implementación de funciones de sensores
│
├── comunicacion.h
│   ├─ Declaración de funciones y variables de comunicación (UART, I2C, SPI)
│
├── comunicacion.cpp
│   └─ Implementación de funciones de comunicación
│
└── main.ino (o main.cpp)
    ├─ Incluye todos los headers necesarios
    └─ Loop principal que llama a motores, LEDs, sensores y comunicación



Explicación del diagrama
main.ino
    Incluye todos los headers que necesita.
    Llama funciones de motors, leds, sensors, y communication.
motores.cpp
    Puede usar funciones de filtro para suavizar ángulos.
    Usa variables de config (offsets, α).
leds.cpp
    Controla LEDs normales y parpadeos finales.
    Usa variables de config si quieres cambiar intervalos u offsets.
sensores.cpp
    Lee sensores (ángulos, Hall, etc.).
    Puede pasar datos a motors o communication.
comunicacion.cpp
    Maneja UART, I2C, SPI.
    Envía/recibe datos de otros módulos.
filtro.cpp
    Implementa funciones del filtro exponencial.
    Variables como α se definen en config.cpp y se declaran en config.h.
config.h / config.cpp
    Centraliza constantes y variables compartidas entre módulos.
    Cada módulo que necesite esas variables solo incluye config.h.