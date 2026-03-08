// =======================================================================
//                 🔹 C H E S S B O T  —   Z E R O 🔹
// =======================================================================
//  Archivo    : sensors.cpp
//  Autor      : Klaus Michalsky
//  Fecha      : Feb-2026
// -----------------------------------------------------------------------
//  ▫️ DESCRIPCIÓN
//      - Inicializar y configurar sensores.
//      - Implementación de funciones para la lectura y gestión de
//        sensores AS5600.
// =======================================================================

#include <Arduino.h>

#include "config.h"
#include "sensors.h"
#include "utils.h"

// VARIABLES INTERNAS DEL MÓDULO ❌ Mover estas variables a core❓
// -----------------------------------------------------------------------
// -1000 es solo un valor de “inicio imposible” para asegurar que el primer envío siempre se haga
unsigned long lastSendTime_1 = 0; // Guarda el momento en milisegundos del último envío
float lastSentAngle_1 = -1000.0f; // Guarda el último ángulo enviado
unsigned long lastSendTime_2 = 0;
float lastSentAngle_2 = -1000.0f;

// API PÚBLICA DE SENSORES
// -----------------------------------------------------------------------
void sensorsInit()
{
    pinMode(HALL_1, INPUT_PULLUP);
    pinMode(HALL_2, INPUT_PULLUP);
    pinMode(HALL_3, INPUT_PULLUP);

    Wire.setSDA(AS5600_1_SDA);
    Wire.setSCL(AS5600_1_SCL);
    Wire.begin();
    Wire1.setSDA(AS5600_2_SDA);
    Wire1.setSCL(AS5600_2_SCL);
    Wire1.begin();
}

// TwoWire → le decimos “la función va a recibir un objeto de tipo TwoWire”.
// &wire → le decimos “pasalo por referencia, no por copia”.
uint16_t sensorReadAngle(TwoWire &wire)
{
    wire.beginTransmission(AS5600_ADDR);
    wire.write(0x0E);
    wire.endTransmission(false);
    wire.requestFrom(AS5600_ADDR, (uint8_t)2);

    if (wire.available() < 2)
        return 0;

    uint8_t high = wire.read();
    uint8_t low = wire.read();

    return ((high & 0x0F) << 8) | low;
}

void sensorSendAngle(TwoWire &wire)
{
    uint16_t rawAngle = sensorReadAngle(wire); // Leer sensor AS5600
    float degrees = rawToDegrees(rawAngle);    // Convertir a grados
    degrees = round1Decimal(degrees);          // Redondear a 1 decimal
    Serial1.print(degrees, 1);                 // Asegurar envio de solo 1 decimal
    Serial1.print("\n");
}

// ⚠️ Solo para pruebas de lectura de angulo continuo
// -> El envio continuo bloquea movimiento de motores
void sensorStreamAngle(TwoWire &wire, float &lastSentAngle, unsigned long &lastSendTime)
{
    uint16_t rawAngle = sensorReadAngle(wire);                                 // Leer sensor AS5600
    float degrees = rawToDegrees(rawAngle);                                    // Convertir a grados
    degrees = round1Decimal(degrees);                                          // Redondear a 1 decimal
    sendFilteredFloat(degrees, lastSentAngle, lastSendTime, 0.5, 33, Serial1); // Enviar angulo filtrado por UART
}

// ⚠️ No muy exato:
// si un valor se desvia mucho por ruido el promedio es inexacto
// por otra parte si la lectura esta entre el 0° y 360°
// entonces el promedio da un valor muy fuera del rango
// float sensorHomingOffset()
// {
//     const uint8_t samples = 30;
//     float sum = 0;
//     for (uint8_t i = 0; i < samples; i++)
//     {
//         sum += sensorReadAngle(Wire);
//         delay(2); // para evitar saturar el I2C
//     }
//     float average = sum / samples;

//     return average;
// }

// Si alguna vez tu homing mecanico cae cerca del 0 digital del sensor:
// entonces el promedio normal se rompe
// Mejor solucion:
// 30 lecturas + corrección si cruza 0° + promedio 👀💡

//         0°
//    330°     30°
//  300°         60°
//  270°         90°
//  240°        120°
//   210°      150°
//         180°

float sensorHomingOffset()
{
    const uint8_t samples = 30;
    float sum = 0;
    float firstAngle = sensorReadAngle(Wire);

    for (uint8_t i = 0; i < samples; i++)
    {
        float angle = sensorReadAngle(Wire);
        // corregir salto 0° / 360°
        // fabs -> valor absoluto |angle - firstAngle|
        // Si la diferencia es mayor que 180°,
        // significa que cruzamos el cero del sensor.
        // si cruzamos el cero
        // mover los valores al mismo lado del círculo
        if (fabs(angle - firstAngle) > 180)
        {
            if (angle < firstAngle)
                angle += 360;
            else
                angle -= 360;
        }
        sum += angle;
    }

    // Normalizar el angulo si el promedio queda fuera del rango
    // Eso solo mueve el número al rango correcto
    // Si un cálculo da 361° eso en realidad es lo mismo que 1°
    // si pasa de 360 → restar 360, si es menor que 0 → sumar 360
    float avg = sum / samples;

    if (avg >= 360)
        avg -= 360;
    if (avg < 0)
        avg += 360;

    return avg;
}