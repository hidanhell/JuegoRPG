#pragma once
#include "Armas.h"
#include "Artefactos.h"
#include <vector>
#include <optional>

// Catalogos globales de items
extern std::vector<Arma> listaArmas;
extern std::vector<Artefacto> listaArtefactos;

// Armas legendarias especiales
extern Arma excalibur;
extern Arma bastonDragon;
extern Arma arcoAlma;
extern Arma armaLancelot;

// Consultas y filtrado
std::vector<Arma> obtenerArmasZona(int zona, Clase clase);
std::vector<Artefacto> obtenerArtefactosZona(int zona, Clase clase);
std::optional<Arma> obtenerArmaPorId(int id);
std::optional<Artefacto> obtenerArtefactoPorId(int id);