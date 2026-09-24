#pragma once
#include "Personajes.h"
#include "Monstruos.h"

// =========================================================
// TURNO DEL ENEMIGO INTELIGENTE
// Decide entre usar una habilidad de monstruo, atacar de forma
// normal, o (en el caso de Lancelot) activar la Espada del Dios del
// Caos en su Fase 3. Devuelve true si el monstruo huyo exitosamente.
// =========================================================
bool turnoEnemigo(Personaje &p, Monstruo &m, bool faseValdrame2, bool fase3Lancelot);