#pragma once
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include "FasesJefe.h"
#include "TurnoJugador.h"
#include "TurnoEnemigo.h"
#include <optional>
#include <string>

// =========================================================
// GENERACIÓN Y UBICACIÓN
// =========================================================
int obtenerZonaPorY(int y);
std::optional<Monstruo> generarEnemigo(int zonaActual);

// =========================================================
// NÚCLEO Y FLUJO DE COMBATE
// =========================================================
void iniciarCombate(Personaje &p, int zonaActual);

// Componentes del ciclo de combate
void mostrarHUDCombate(const Personaje &p, const Monstruo &m, int hpInicial, const std::string &etiqueta);
bool procesarEstadosEnemigo(Monstruo &m, int hpInicial);
bool procesarEstadosJugador(Personaje &p);
void procesarBuffsJugador(Personaje &p);
void resolverFinDeCombate(Personaje &p, Monstruo &m, int y, bool huidaExitosa, bool monstruoHuyo);