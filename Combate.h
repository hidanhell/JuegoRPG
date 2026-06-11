#pragma once
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include <optional>

// --- Control de Estado ---
enum class EstadoCombate {
    EN_PROGRESO,
    VICTORIA,
    DERROTA,
    HUIDA_EXITOSA
};

// --- Contexto Simplificado (1v1 con visión a futuro) ---
struct ContextoCombate {
    Personaje* jugador;
    Monstruo* enemigo;
    int zonaActual;
    int ronda;
};

// --- Generación de Enemigos ---
// Devuelve std::nullopt si no hay monstruos disponibles para la zona.
std::optional<Monstruo> generarEnemigo(int zonaActual);

// --- Núcleo de Combate ---
void iniciarCombate(Personaje &p, int zonaActual);

// --- Fases del Turno ---
void evaluarEstados(ContextoCombate &contexto);
EstadoCombate ejecutarTurnoJugador(ContextoCombate &contexto);
EstadoCombate ejecutarTurnoEnemigo(ContextoCombate &contexto);

// --- Sistema de Resolución y Habilidades ---
void resolverFinCombate(Personaje &p, ContextoCombate &contexto, EstadoCombate resultado);
void mostrarMenuCombate(const ContextoCombate &contexto);
void usarHabilidad(Personaje* lanzador, Monstruo* objetivo, int idHabilidad);

