#ifndef HABILIDADES_H
#define HABILIDADES_H

#include <vector>
#include <string>
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"

// ==========================
// ESTRUCTURA DE HABILIDAD
// ==========================
struct Habilidad {
    int id;
    std::string nombre;
    std::string descripcion;
    int poderBase;
    int duracion;
    std::string tipo;   // "Pasiva", "Activa", "Ultimate", "ArmaElite"
    int efectoId;       // ID del efecto en listaEfectos (0 si no aplica)
};

// ==========================
// CATÁLOGO GLOBAL
// ==========================
extern std::vector<Habilidad> listaHabilidadesGlobal;

// ==========================
// FUNCIONES
// ==========================
Habilidad obtenerHabilidadPorId(int id);
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad);
void aplicarSubclase(Personaje &pj, int idHabilidad);

// ==========================
// HABILIDADES DE ARMAS ELITE
// ==========================
void aplicarGritoDelCaido(Monstruo& m, Personaje& p);

#endif