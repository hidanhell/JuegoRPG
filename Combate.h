#pragma once
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"

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
Monstruo generarEnemigo(int zonaActual);

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

// --- Función auxiliar de efectos (accesible desde motor.cpp via este header) ---
Efecto obtenerEfectoPorId(int id);