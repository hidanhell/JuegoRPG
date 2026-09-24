#pragma once
#include "Personajes.h"
#include "Monstruos.h"

// =========================================================
// TURNO DEL JUGADOR — Menu tactico completo
// Ataque basico, habilidades/ultimates, inventario y huida.
// Devuelve true si el jugador huyo exitosamente.
// =========================================================
bool turnoJugador(Personaje &p, Monstruo &m, int &turnosAbsorcion);