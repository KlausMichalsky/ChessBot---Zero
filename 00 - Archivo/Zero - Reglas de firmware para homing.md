
REGLAS CLAVE DE DISEÑO DE FIRMWARE (HOMING / SCARA)

1. enum = Tipo de dato del sistema
Define estados válidos y el lenguaje del sistema.

2. .h declara, .cpp implementa
Separación clara entre interfaz y lógica.

3. setup() es solo hardware
Pines, drivers, periféricos. Nada de lógica.

4. homingInit() es reset lógico
Limpia estado interno, no toca hardware.

5. Variables no se declaran en funciones
Solo se reinician allí.

6. Variables internas → static en .cpp
Encapsulación y seguridad.
static es la memoria interna de una máquina de estados

7. config.h define el idioma del sistema
Pines, constantes, enums. No estado.

8. Homing es un módulo
Tiene .h y .cpp propios.

9. loop() no bloquea
Máquina de estados, sin delay().

10. Constantes no son variables
Usar const o static const.

11. Hardware abajo, lógica arriba
Arquitectura por capas.

12. Si se lee sin comentarios, está bien diseñado
Nombres claros y flujo evidente.
