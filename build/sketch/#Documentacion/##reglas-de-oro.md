#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/#Documentacion/##reglas-de-oro.md"
# **Reglas de oro**

🔹 enum (enumeración)
Qué es:
Un tipo de dato que solo define un conjunto de valores posibles.
Es útil para estados, comandos, flags, etc.
Cada valor del enum es una constante.
Ejemplo clásico:
enum MotorState {
    OFF,
    RUNNING,
    ERROR
};
MotorState es un tipo.
Puede tomar solo OFF, RUNNING, ERROR.
Con enum class (C++ moderno):
enum class MotorState {
    OFF,
    RUNNING,
    ERROR
};
Ahora los valores se usan con ::: MotorState::OFF
Son encapsulados, no contaminan el espacio global
Más seguro: no se mezclan con enteros ni otros enums
Lo que NO puede hacer un enum:
No tiene variables adicionales
No tiene métodos
No puede “guardar” datos asociados a cada valor
🔹 struct (estructura)
Qué es:
Un contenedor de datos que puede tener varias variables.
Puede almacenar datos de distintos tipos.
En C++ moderno, puede tener métodos (aunque normalmente se usa como contenedor simple).
Ejemplo:
struct HomingRuntimeXY {
    MotorState state;      // enum dentro de la struct
    long firstEdge;
    long secondEdge;
    long centerPosition;
    bool fault;
};
HomingRuntimeXY sí tiene “campos”, podés hacer:
HomingRuntimeXY motor1;
motor1.state = MotorState::OFF;
motor1.firstEdge = 123;
motor1.fault = false;
Básicamente, struct = agrupación de variables, mientras que enum = lista de etiquetas posibles.


Necesito…
Lista de valores posibles (estados, comandos) Usar…	enum class
Necesito…
Tipo de objeto con datos y funciones Usar… class o struct


┌─────────────────────────┐
│       enum class        │
│     HomingStateXY       │
│-------------------------│
│ INACTIVE                │
│ FIND_FIRST_EDGE_CW      │
│ FIND_SECOND_EDGE_CW     │
│ FIND_FIRST_EDGE_CCW     │
│ FIND_SECOND_EDGE_CCW    │
│ SEARCH_FAST_CW          │
│ SEARCH_FAST_CCW         │
│ REVERSE_EDGE_CW         │
│ REVERSE_EDGE_CCW        │
│ CALC_CENTER             │
│ MOVE_TO_CENTER          │
│ OK                      │
│ ERROR                   │
└─────────────────────────┘
             ▲
             │
             │ (tipo de la variable state)
┌─────────────────────────┐
│         struct          │
│   HomingRuntimeXY       │
│-------------------------│
│ HomingStateXY state     │
│ unsigned long startTime │
│ long firstEdge          │
│ long secondEdge         │
│ long centerPosition     │
│ bool fault              │
└─────────────────────────┘


┌─────────────────────────┐
│       enum class        │
│        Command          │
│-------------------------│
│ STATUS                  │
│ RESET_ERRORS            │
│ HOME_MOTOR1             │
│ HOME_MOTOR2             │
│ GET_ANGLE1              │
│ GET_ANGLE1_START        │
│ GET_ANGLE2              │
│ GET_ANGLE2_START        │
│ GET_ANGLE_STOP          │
│ UNKNOWN                 │
└─────────────────────────┘
             ▲
             │
             │ (variable de comando a procesar)
┌─────────────────────────┐
│     Función UART        │
│   processCommand(cmd)   │
│-------------------------│
│ switch(cmd)             │
│   case Command::STATUS  │
│   case Command::HOME_MOTOR1 │
│   ...                   │
└─────────────────────────┘