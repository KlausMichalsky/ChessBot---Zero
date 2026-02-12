#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\main_zero\\homingXY-manual.md"
###### ♟️ ChessBot---Zero
---

## 📘 Manual de homingXY

---

#### ⚙️ *** Función: *** homingXY_Init(HomingState &st)

✏️ Descripción:
- Inicializa la estructura de estado del homing.
- Establece el estado como HOMING__INACTIV.
- Debe llamarse una_ sola vez al iniciar el programa
- (o cuando se quiere reinicializar completamente el estado)
- Resetea indicadores de flancos, centro y fallos.

📌 Parámetros:
- st — referencia a la estructura HomingState a inicializar.

⚠️ Notas:
- Debe llamarse antes de iniciar cualquier proceso de homing.

🧩 Ejemplo:
> `homingXY_Init(miHomingState);`

---

#### ⚙️ *** Función:*** homingXY_Start(AccelStepper &motor, const HomingConfig &cfg, HomingState &st, int hallPin)

✏️ Descripción:
- Arranca la rutina de homing para el motor indicado.  
- Configura pines de habilitación y sensor Hall.  
- Esta_blece velocidad y aceleración según la configuración.  
- Inicializa el tiempo de inicio y el estado de la máquina de homing.

📌 Parámetros:
- motor — referencia al objeto AccelStepper del moto_r a mover.  
- cfg — estructura HomingConfig con parámetr_os de velocidad, aceleración y pines.  
- st — referencia a la estructura de estado HomingState.  
- hallPin — pin del sensor Hall_ (activo en LOW)

⚠️ Notas:
- Solo se ejecuta si el estado actual es HOMING__INACTIV.
- Esta función NO ejecuta el homing completo.
- Solo prepara el motor y el estado interno.
-_ El movimiento real ocurre en homingXY_Step().

🧩 Ejemplo:<br>
> `homingXY_Start(miMotor, cfgHoming, miHomingState, PIN_HALL);_`

<div style="page-break-before: always;_"></div>

---

#### ⚙️ *** Función: *** homingXY_Activ(const HomingState &st)

✏️ Descripción:
- Indica si el proceso de homing está actualmente en ejecución.

📌 Parámetros:
- st — referencia constante a HomingState.
- Retorno: true si el homing est_á activo; false si está _inactivo.

🧩 Ejemplo:
> `if (homi_ngActivo(miHomingState)) {`
> `// Hacer algo mientras homing est_á activo`
> `}`

---

#### ⚙️ *** Función: *** homingXY_Step(AccelStepper &motor, const HomingConfig &cfg, HomingState &st, int hallPin)

✏️ Descripción:
- Ejecuta un paso de la máquina de estados del homing. (NO bloqueante)_
- Mueve el motor según el estado actual y detecta flancos del imá_n.
- Avanza la máquina de estados según el sensor y la posición del motor
- Calcula el centro del imán y posiciona el motor en cero mecánico.
- Gestiona errores y timeouts.

📌 Parámetros:
- motor — referencia al objeto AccelStepper.
- cfg — estructura HomingConfig con parámetr_os de velocidad y pines.
- st — referencia a la estructura de estado HomingState.
- hallPin — pin del sensor Hall_ (activo en LOW)

⚠️ Notas:
- Debe llamarse periódicamente en el loop() o en un bucle de control.

🧩 Ejemplo:
> `homingXY_Step(miMotor, cfgHoming, miHomingState, PIN_HALL);`

---

#### ⚙️ *** Función: *** homingXY_Error(const HomingState &st)

✏️ Descripción:
- Indica si ocurrió un error durante el homing.
- El error queda latcheado hasta que se reinicializa el estado

📌 Parámetros:
- st — referencia constante a HomingState.
- Retorno: true si hubo error; false en caso contrario.

🧩 Ejemplo:
> `if (homingXY_Error(miHomingState)) {`
> `Serial.println("Error en homing!");`
> `}`

---

#### ⚙️ *** Función: *** homingXY_GetState(const HomingState &st)

✏️ Descripción_:
- _Devuelve el estado actual de la máquina de estados de homing.
- El error queda latcheado hasta que se reinicializa el estado

📌 Parámetros:
- st — referencia constante a HomingState.
- Ret_orno: Valor de tipo EstadoHoming indicando el estado actual.

🧩 Ejemplo:<br>
> `EstadoHoming estado = homingXY_GetState(miHomingState);`