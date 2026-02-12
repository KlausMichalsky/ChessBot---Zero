#pragma once
/* Filtro exponencial con offset incluido */

float actualizarAngulo(float valorActual, float &valorFiltrado, float alpha, float offset);
