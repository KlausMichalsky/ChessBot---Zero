♾️ ZERO (C++/Arduino) – Directorio
/zero/
│
├── zero_main.ino
│
├── motors.cpp
├── motors.h
│
├── sensors.cpp
├── sensors.h
│
├── communication.cpp
├── communication.h
│
└── config.h

🔵 1. zero_main.ino - Control principal del Zero
    Responsabilidades:
        Inicializar drivers
        Inicializar UART
        Bucle principal
        Procesar comandos del Pico
        Llamar a motores y sensores
    Ejemplo:
        #include "motors.h"
        #include "sensors.h"
        #include "communication.h"
        void loop() {
            handleIncomingCommands();
        }

🔵 2. motors.cpp / motors.h - Manejo completo del movimiento
    Responsabilidades:
        Control de drivers (A4988, TMC2209, XDrive MINI, etc.)
        Mover a coordenadas
        Homing
        Control de velocidad
        Rampa de aceleración
        Parámetros de torque/corriente
    Funciones:
        moveTo(x, y)
        home()
        stop()
        setSpeed()

🔵 3. sensors.cpp / sensors.h - Lectura precisa de posición
    Responsabilidades:
        Leer AS5600
        Leer sensores Hall
        Fusionar datos para precisión absoluta
        Enviar posición real al Pico
    Funciones:
        readAngleMotor1()
        readAngleMotor2()
        readHallLimits()
        getPosition()

🔵 4. communication.cpp / communication.h - Comunicación con el Pico (UART)
    Responsabilidades:
        Parsear comandos
        Confirmar mensajes
        Detectar errores
        Enviar datos del Zero al Pico
    Funciones:
        handleIncomingCommands()
        sendPosition()
        sendStatus()

🔵 5. config.h - Configuración hardware
    Incluye:
        Pines motores
        Pines AS5600
        Pines sensores Hall
        Límites mecánicos
        Velocidades
        Microstepping
    Ejemplo:
        #define MOTOR1_STEP 2
        #define MOTOR1_DIR 3
        #define AS5600_1_SDA 18
        #define AS5600_1_SCL 19