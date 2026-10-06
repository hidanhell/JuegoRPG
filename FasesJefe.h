#pragma once
#include "Personajes.h"
#include "Monstruos.h"

// =========================================================
// GESTIÓN DE MECÁNICAS Y FASES DE JEFES
// Procesa el aura pasiva de Valdrame y los cambios de fase 
// según el porcentaje de HP (Valdrame / Lancelot).
// =========================================================
void manejarFasesJefe(Monstruo &m, Personaje &p, int y,
                       bool &fase2, bool &fase3, bool &faseValdrame2,
                       int hpInicial, int &turnosAbsorcion);