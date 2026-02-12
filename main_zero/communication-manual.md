# 📘 Manual de Comunicación — RP2040 Zero ↔ Pico

---

⚙️ *** Función: *** `uart_zero_init()`  
> Descripción:  
Inicializa la comunicación UART en el RP2040 Zero.
• Configura el puerto Serial para depuración vía USB.  
• Asigna los pines TX y RX de Serial1 al hardware del Zero.  
• Inicializa Serial1 para la comunicación UART con el Pico.

> Parámetros: Ninguno

> Notas: 
• La espera `while (!Serial1)` no es necesaria en el RP2040 Zero.  
• La velocidad de comunicación es 115200 baudios.  
• Serial1 se utiliza exclusivamente para la comunicación Zero ↔ Pico.

---

⚙️ *** Función: *** `leerAngulos()`  
> Descripción:  
Lee ángulos entrantes desde el Pico por Serial1 con el formato:
>> Ejemplo: `M_1: 90.5`
• Extrae el ID del motor y el ángulo objetivo.  
• Convierte los valores a tipos numéricos.  
• Reenvía el ángulo recibido por Serial1 como confirmación.  
• Llama a la función de movimiento del motor correspondiente.

> Notas:  
• El carácter `_` separa el prefijo del ID del motor.  
• El carácter `:` separa el ID del valor del ángulo.  
• Los comandos inválidos se ignoran.

---

⚙️ *** Función: *** `mandarPosition()`  
> Descripción:  
Manda coordenadas al Pico por Serial1 para confirmación.

---

⚙️ *** Función: *** `mandarStatus()`  
> Descripción:  
Manda coordenadas al Pico por Serial1 para confirmación.
