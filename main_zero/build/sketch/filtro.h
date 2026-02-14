#line 1 "/Users/klausmichalsky/Proyectos Mac/ChessBot---Zero/main_zero/filtro.h"
#pragma once
/* Filtro exponencial con offset incluido */

float actualizarAngulo(float valorActual, float &valorFiltrado, float alpha, float offset);
