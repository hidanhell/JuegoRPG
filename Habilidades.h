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
    std::string tipo;   // "Pasiva" o "Activa"
    int efectoId;       // ID del efecto en listaEfectos (0 si no aplica)
};

// ==========================
// CATÁLOGO GLOBAL DE HABILIDADES
// ==========================
extern std::vector<Habilidad> listaHabilidadesGlobal;

// ==========================
// FUNCIONES DE HABILIDADES
// ==========================

// Buscar habilidad por ID
Habilidad obtenerHabilidadPorId(int id);

// Ejecutar habilidad activa en combate
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad);

// Aplicar subclase (nivel 15, pasivas permanentes)
void aplicarSubclase(Personaje &pj, int idHabilidad);

#endif
