#pragma once
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include <optional>
#include <string>

// =========================================================
// Generación de Enemigos
// =========================================================
// Devuelve std::nullopt si no hay monstruos disponibles para la zona.
std::optional<Monstruo> generarEnemigo(int zonaActual);

// Determina la zona (1-5) segun la posicion Y. Usada por generarEnemigo
// y por manejarFasesJefe. (loot.cpp y main.cpp todavia repiten esta
// misma logica por su cuenta — pendiente de unificar en un futuro
// modulo de Zonas).
int obtenerZonaPorY(int y);

// =========================================================
// Núcleo de Combate
// =========================================================
void iniciarCombate(Personaje &p, int zonaActual);

// --- Piezas internas del combate, expuestas por si se necesitan
//     reutilizar o testear por separado ---

// HUD y procesamiento de estados
void mostrarHUDCombate(const Personaje &p, const Monstruo &m, int hpInicial, const std::string &etiqueta);
void procesarEstadosEnemigo(Monstruo &m, int hpInicial);
void procesarEstadosJugador(Personaje &p);

// Fases de jefe (aura de Valdrame, transformaciones, fases de Lancelot)
void manejarFasesJefe(Monstruo &m, Personaje &p, int y,
                       bool &fase2, bool &fase3, bool &faseValdrame2,
                       int hpInicial, int &turnosAbsorcion);

// Turnos
// turnoJugador recibe turnosAbsorcion por referencia porque la Fase 2
// de Lancelot (Escamas del Rey) necesita descontar los usos restantes
// cuando el jugador ataca.
bool turnoJugador(Personaje &p, Monstruo &m, int &turnosAbsorcion);
bool turnoEnemigo(Personaje &p, Monstruo &m, bool faseValdrame2, bool fase3Lancelot);

// Resolución
void resolverFinDeCombate(Personaje &p, Monstruo &m, int y, bool huidaExitosa, bool monstruoHuyo);