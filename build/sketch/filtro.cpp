#line 1 "C:\\Users\\Benutzer1\\Documents\\# Github repositories\\ChessBot---Zero\\filtro.cpp"
#include <Arduino.h>
#include "filtro.h"

float actualizarAngulo(float valorActual, float &valorFiltrado, float alpha, float offset)
{
    // Filtrado exponencial
    valorFiltrado = alpha * valorActual + (1.0 - alpha) * valorFiltrado;
    // Aplicar offset
    return valorFiltrado + offset;
}
