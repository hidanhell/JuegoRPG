#pragma once
#include "Armas.h"
#include "Artefactos.h"
#include <vector>

// --- Listas globales de items ---
extern std::vector<Arma> listaArmas;
extern std::vector<Artefacto> listaArtefactos;

// --- Funciones auxiliares de filtrado ---
std::vector<Arma> obtenerArmasZona(int zona, const std::string& clase);
std::vector<Artefacto> obtenerArtefactosZona(int zona, const std::string& clase);

// --- Armas legendarias (definidas en catalogoObjetos.cpp) ---
// Cada una corresponde a una clase especifica, disponibles en el cofre antes de Lancelot
extern Arma excalibur;      // Guerrero
extern Arma bastonDragon;   // Mago
extern Arma arcoAlma;       // Cazador
extern Arma armaLancelot;   // Reservada (jefe final)