#pragma once

#include "Personajes.h"
#include <string>

// =========================================================
// GESTIÓN DE LOOT, BOTÍN Y MUNDO
// =========================================================

// Generación de botín regular (Consumibles, Armas, Artefactos, Reliquias)
void gestionarLoot(Personaje &p, int y, bool raro);

// Botín exclusivo otorgado tras derrotar al Arzobispo Valdrame
void gestionarLootValdrame(Personaje &p);

// Mapeo de posición Y a nombre de la zona actual
std::string obtenerNombreZona(int y);

// Eventos de texto ambiental al avanzar en el mapa
void lanzarDialogoAmbiental(int y);