#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\main_zero\\motorsXY-manual.md"
###### ♟️ ChessBot---Zero

---

## 📘 Manual de `motorsXY`

Módulo responsable del control del sistema **XY** del robot de ajedrez. Este módulo gestiona **energía, movimiento y estado** del sistema XY. El proceso de referencia (homing) se realiza en el módulo `homingXY`.

---

### ⚙️ Función: `motorsXY_Init()`

✏️ **Descripción:**
Inicializa el módulo de motores XY. Configura pines, drivers, variables internas y deja el sistema listo para operar.

📌 **Parámetros:**

* Ninguno.

⚠️ **Notas:**

* Debe llamarse **una sola vez** al inicio del programa.
* No habilita los motores.
* No realiza homing.

🧩 **Ejemplo:**

```cpp
motorsXY_Init();
```

---

### ⚙️ Función: `motorsXY_Enable()`

✏️ **Descripción:**
Habilita eléctricamente los motores XY.

📌 **Parámetros:**

* Ninguno.

⚠️ **Notas:**

* Requerido antes de cualquier movimiento.
* No inicia movimiento.

🧩 **Ejemplo:**

```cpp
motorsXY_Enable();
```

---

### ⚙️ Función: `motorsXY_Disable()`

✏️ **Descripción:**
Deshabilita eléctricamente los motores XY.

📌 **Parámetros:**

* Ninguno.

⚠️ **Notas:**

* Libera los motores.
* Útil para reposo o seguridad.

🧩 **Ejemplo:**

```cpp
motorsXY_Disable();
```

---

### ⚙️ Función: `motorsXY_Move(long x, long y)`

✏️ **Descripción:**
Ordena un movimiento relativo del sistema XY.

📌 **Parámetros:**

* `x` → Pasos a mover en el eje X.
* `y` → Pasos a mover en el eje Y.

⚠️ **Notas:**

* Movimiento **no bloqueante**.
* Requiere motores habilitados.

🧩 **Ejemplo:**

```cpp
motorsXY_Move(2000, -500);
```

---

### ⚙️ Función: `motorsXY_SetSpeed(long speed)`

✏️ **Descripción:**
Configura la velocidad del sistema XY.

📌 **Parámetros:**

* `speed` → Velocidad en pasos por segundo.

⚠️ **Notas:**

* Debe llamarse antes del movimiento.
* Afecta a ambos ejes.

🧩 **Ejemplo:**

```cpp
motorsXY_SetSpeed(1200);
```

---

### ⚙️ Función: `motorsXY_Run()`

✏️ **Descripción:**
Servicio del módulo XY. Actualiza el estado y ejecuta los movimientos pendientes.

📌 **Parámetros:**

* Ninguno.

⚠️ **Notas:**

* Debe llamarse continuamente en `loop()`.
* No bloquea.

🧩 **Ejemplo:**

```cpp
void loop()
{
    motorsXY_Run();
}
```

---

### ⚙️ Función: `motorsXY_Stop()`

✏️ **Descripción:**
Detiene inmediatamente el movimiento del sistema XY.

📌 **Parámetros:**

* Ninguno.

⚠️ **Notas:**

* No deshabilita los motores.
* Pensada para situaciones de emergencia.

🧩 **Ejemplo:**

```cpp
motorsXY_Stop();
```

---

### ⚙️ Función: `motorsXY_Done()`

✏️ **Descripción:**
Indica si el último movimiento del sistema XY finalizó.

📌 **Parámetros:**

* Ninguno.

📤 **Retorna:**

* `true` → movimiento finalizado.
* `false` → movimiento en curso.

⚠️ **Notas:**

* No inicia ni detiene movimientos.

🧩 **Ejemplo:**

```cpp
if (motorsXY_Done())
{
    // siguiente acción
}
```

---

### 📌 Flujo típico de uso

```cpp
motorsXY_Init();
motorsXY_Enable();
motorsXY_SetSpeed(1000);

motorsXY_Move(3000, 1500);

while (!motorsXY_Done())
{
    motorsXY_Run();
}
```

---

### 📎 Relación con otros módulos

* `homingXY` → referencia del sistema XY
* `motorsZ` → control del eje Z
* `communication` → comandos UART

---

### ⚠️ Estado del módulo

* Control de energía ✔️
* Movimiento relativo ✔️
* Máquina de estados no bloqueante ✔️

---
