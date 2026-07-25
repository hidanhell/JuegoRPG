#pragma once

#include "Personajes.h"
#include <string>

bool guardarPartida(const Personaje &p, int y, const std::string &ruta);
bool cargarPartida(Personaje &p, int &y, const std::string &ruta);
void nuevaPartida(Personaje &p, int yInicial);
