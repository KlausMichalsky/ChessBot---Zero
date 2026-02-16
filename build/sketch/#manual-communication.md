#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\#manual-communication.md"
###### ♟️ ChessBot---Zero
---

## 📘 Manual de communication.cpp

---

#### ⚙️ *** Módulo: *** communication.cpp

✏️ Descripción:
- Gestiona la comunicación UART entre el RP2040 Zero y el Pico.
- Inicializa el hardware UART del Zero.
- Recibe comandos entrantes.
- Envía datos de confirmación y estado.

📌 Responsabilidades:
- Configurar Serial1 para comunicación Zero ↔ Pico.
- Interpretar comandos recibidos.
- Extraer ID de motor y valores numéricos.
- Enviar confirmaciones y estados.

⚠️ Notas:
- Serial se usa para depuración por USB.
- Serial1 se usa exclusivamente para la comunicación UART entre placas.
- La velocidad de comunicación debe coincidir con la del otro dispositivo.
- Este módulo implementa lógica básica de parsing.

🧩 Ejemplo de uso:
> `uart_zero_init();`
>  
> `leerAngulos();`

---

<div style="page-break-before: always;"></div>

---

#### ⚙️ *** Función: *** uart_zero_init()

✏️ Descripción:
- Inicializa la comunicación UART en el RP2040 Zero.
- Configura Serial para depuración USB.
- Configura los pines TX y RX para Serial1.
- Inicializa Serial1 con el baudrate definido.

📌 Parámetros:
- Ninguno.

⚠️ Notas:
- La espera `while (!Serial1)` no es necesaria en RP2040.
- El baudrate debe coincidir con el del otro dispositivo (ej. 115200).
- Debe llamarse una sola vez en `setup()`.

🧩 Ejemplo:
> `void setup() {`
> `    uart_zero_init();`
> `}`

---

#### ⚙️ *** Función: *** leerAngulos()

✏️ Descripción:
- Lee datos entrantes desde Serial1.
- Espera comandos en formato estructurado.
- Extrae:
  - ID del motor.
  - Ángulo objetivo.
- Convierte los valores recibidos a tipo numérico.
- Llama a la función de movimiento correspondiente.
- Envía confirmación por UART.

📌 Formato esperado:
> `M_1: 90.5`

📌 Separadores:
- `_` separa prefijo e ID de motor.
- `:` separa ID y valor del ángulo.

⚠️ Notas:
- Comandos inválidos se ignoran.
- Debe llamarse periódicamente en `loop()`.
- No debe bloquear la ejecución principal.

🧩 Ejemplo:
> `void loop() {`
> `    leerAngulos();`
> `}`

---

#### ⚙️ *** Función: *** mandarPosition()

✏️ Descripción:
- Envía la posición actual al Pico mediante Serial1.
- Se utiliza como confirmación o sincronización.

📌 Parámetros:
- Dependen de la implementación (ej. coordenadas o ángulos).

⚠️ Notas:
- El mensaje debe terminar con salto de línea.
- Mantener formato consistente con el protocolo definido.

🧩 Ejemplo:
> `mandarPosition();`

---

#### ⚙️ *** Función: *** mandarStatus()

✏️ Descripción:
- Envía información de estado al Pico.
- Puede indicar:
  - Motor en movimiento.
  - Movimiento finalizado.
  - Error detectado.

📌 Parámetros:
- Dependen de la implementación interna.

⚠️ Notas:
- Usar formato claro y delimitado.
- Mantener compatibilidad con el parser del otro lado.

🧩 Ejemplo:
> `mandarStatus();`

---

## 🔄 Flujo típico de comunicación

1. El Pico envía un comando por UART.
2. El RP2040 Zero recibe el mensaje en Serial1.
3. Se parsea el comando.
4. Se ejecuta el movimiento correspondiente.
5. El Zero responde con confirmación o estado.

---

## 🎯 Buenas prácticas

- Mantener formato de mensaje consistente.
- Finalizar siempre los mensajes con `\n`.
- No bloquear el loop principal.
- Validar datos antes de convertirlos.
- Mantener este módulo separado de la lógica de motores.

---

✅ communication.cpp es el puente de comunicación entre el Pico y el RP2040 Zero dentro del proyecto ChessBot---Zero.
