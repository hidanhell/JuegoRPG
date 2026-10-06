#ifndef HABILIDADES_H
#define HABILIDADES_H

#include <vector>
#include <string>
#include <optional>
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"

// =========================================================
// ESTRUCTURA DE HABILIDAD
// =========================================================
struct Habilidad {
    int id;
    std::string nombre;
    std::string descripcion;
    int poderBase;
    int duracion;
    std::string tipo;   // "Pasiva", "Activa", "Ultimate", "ArmaElite"
    int efectoId;       // ID del efecto en listaEfectos (0 si no aplica)
};

// Catálogo global de habilidades
extern std::vector<Habilidad> listaHabilidadesGlobal;

// =========================================================
// CONSULTAS Y EJECUCIÓN GENERAL
// =========================================================
std::optional<Habilidad> obtenerHabilidadPorId(int id);

// Ejecución de habilidades activas y pasivas de subclase
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad);
void aplicarSubclase(Personaje &pj, int idHabilidad);

// =========================================================
// ULTIMATES NIVEL 20
// =========================================================
int obtenerUltimateIdDePersonaje(const Personaje &pj);
bool puedeUsarUltimate(const Personaje &pj);
void ejecutarUltimate(Personaje &pj, Monstruo &m, int idHabilidad);

// =========================================================
// HABILIDADES DE ARMAS ELITE
// =========================================================
void aplicarGritoDelCaido(Monstruo &m, Personaje &p);

#endif // HABILIDADES_H