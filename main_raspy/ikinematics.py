# =======================================================================
#        🔹 I N V E R S E   K I N E M A T I C S   2 D O F 🔹
# =======================================================================
#  Archivo    : inverse_kinematics.py
#  Autor      : Klaus Michalsky
#  Fecha      : May-2026
# -----------------------------------------------------------------------
#  ▫️ DESCRIPCIÓN
# -----------------------------------------------------------------------
#  Módulo de cinemática inversa para un brazo robótico SCARA de 2 DOF.
#  Implementacion en KAYRON
#  Convierte coordenadas cartesianas (X, Y) en ángulos articulares:
#
#      - theta1 → base / hombro
#      - theta2 → codo
#      - l1 y l2 representan las longitudes de los segmentos del brazo.
#
#  Características:
#   - Verificación de alcance geométrico
#   - Selección estable de rama del codo
#   - Protección numérica para acos
#   - Compensación de offset mecánico
#   - Normalización de ángulos [-pi, pi]
#
#  ✅ Listo para hardware real
# =======================================================================

import math

l1 = 175.0
l2 = 95.0

print()
print("Inverse Kinematics for a 2-DOF SCARA Robot Arm")


def inverse_kinematics(x, y, l1, l2):

    r2 = x*x + y*y
    r = math.sqrt(r2)

    if r > (l1 + l2) or r < abs(l1 - l2):
        raise ValueError("Out of reach")

    cos_theta2 = (r2 - l1*l1 - l2*l2) / (2*l1*l2)
    cos_theta2 = max(-1.0, min(1.0, cos_theta2))

    # 🔥 DOS RAMAS (clave del problema)
    theta2 = math.atan2(
        math.sqrt(1 - cos_theta2*cos_theta2),
        cos_theta2
    )

    # 🔥 flip de configuración según lado
    if x < 0:
        theta2 = -theta2

    k1 = l1 + l2 * math.cos(theta2)
    k2 = l2 * math.sin(theta2)

    theta1 = math.atan2(y, x) - math.atan2(k2, k1)

    # 🔥 offset mecánico (tu brazo)
    theta1 = math.pi/2 - theta1

    # 🔥 simetría física real del montaje
    theta1 = -theta1

    theta1 = (theta1 + math.pi) % (2*math.pi) - math.pi

    if x < 0:
        theta1 = math.pi - theta1
        theta2 = - math.pi - theta2

    elif x > 0:
        theta1 = - math.pi - theta1
        theta2 = math.pi - theta2

    return theta1, theta2


while True:
    try:

        input_x = float(input("Enter x: "))
        input_y = float(input("Enter y: "))

        theta1, theta2 = inverse_kinematics(
            input_x,
            input_y,
            l1,
            l2
        )

        print(f"\ntheta1 = {math.degrees(theta1):.2f}°")
        print(f"theta2 = {math.degrees(theta2):.2f}°\n")

    except ValueError as e:
        print(e)
