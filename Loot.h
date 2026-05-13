#pragma once
#include "personajes.h"
#include <string>

void gestionarLoot(Personaje &p, int y, bool raro);
void gestionarLootValdrame(Personaje& p);
std::string obtenerNombreZona(int y);
void lanzarDialogoAmbiental(int y);