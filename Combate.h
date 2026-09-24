#pragma once
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include <optional>
#include <string>

// Turnos especializados, separados en sus propios archivos.
// Se incluyen aqui para que cualquier archivo que ya haga
// #include "combate.h" siga viendo turnoJugador/turnoEnemigo/
// manejarFasesJefe sin tener que tocar sus includes.
#include "FasesJefe.h"
#include "TurnoJugador.h"
#include "TurnoEnemigo.h"

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

// Devuelven true si el objetivo debe perder su turno (Paralisis,
// Evasion, Congelacion, Desorientacion Arcana).
bool procesarEstadosEnemigo(Monstruo &m, int hpInicial);
bool procesarEstadosJugador(Personaje &p);

// Descuenta la duracion de los buffs de elixires activos del jugador
// y revierte su bono al expirar (ver BuffActivo en Personajes.h).
void procesarBuffsJugador(Personaje &p);

// Resolución
void resolverFinDeCombate(Personaje &p, Monstruo &m, int y, bool huidaExitosa, bool monstruoHuyo);