#line 1 "C:\\Users\\Klaus\\Documents\\ChessBot---Zero\\#Archiv\\filtro.cpp"
#include <Arduino.h>
#include "filtro.h"

float actualizarAngulo(float valorActual, float &valorFiltrado, float alpha, float offset)
{
    // Filtrado exponencial
    valorFiltrado = alpha * valorActual + (1.0 - alpha) * valorFiltrado;
    // Aplicar offset
    return valorFiltrado + offset;
}
