#pragma once
#include "personajes.h"

// =========================================================
// BLOQUE M1: ENEMIGOS Y COMBATE DETALLADO (v1.18)
// =========================================================

struct Monstruo {
    string nombre;
    int hp, ataque, defensa, velocidad, expAlMorir, nivelEnemigo;
    bool esRaro;
};

Monstruo generarEnemigo(int y);
void iniciarCombate(Personaje &p, int y);
