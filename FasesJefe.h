#pragma once
#include "Personajes.h"
#include "Monstruos.h"

// =========================================================
// FASES DE JEFE (turno especial / jefes)
// Agrupa el Aura de Valdrame + sus transformaciones de fase, y las
// fases de Lancelot (Escudo del Rey / Furia de Angra). Se separan de
// Combate.cpp porque las tres cosas dependen de "quien es el jefe"
// (m.nombre / y) y de en que punto de HP esta -- no son turnos
// normales de jugador ni de enemigo.
// =========================================================
void manejarFasesJefe(Monstruo &m, Personaje &p, int y,
                       bool &fase2, bool &fase3, bool &faseValdrame2,
                       int hpInicial, int &turnosAbsorcion);