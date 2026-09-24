#ifndef EFECTOS_H
#define EFECTOS_H

#include <string>
#include <optional>
#include <vector>


// --- Efecto ---
struct Efecto {
    int id;                 // Identificador único
    std::string nombre;     // Nombre del efecto (Quemadura, Sangrado, Veneno, Paralisis)
    std::string tipo;       // Ofensivo, Defensivo, Estado
    int duracion;           // Turnos que dura
    int intensidad;         // Nivel del efecto (ej: daño por turno, % de probabilidad)

    Efecto(int i = 0,
           std::string n = "-",
           std::string t = "Estado",
           int d = 0,
           int in = 0);

    void mostrarInfo() const;

    // Aplica el efecto sobre un objetivo (jugador o monstruo)
    // FIX: se agrega 'inmune' con valor por defecto (false) para no romper
    // ninguna llamada existente (Combate.cpp, Habilidades.cpp) que sigue
    // invocando aplicar() con solo 3 argumentos. Cuando inmune=true, los
    // efectos comunes (IDs 1-7) se bloquean por completo -- usado por el
    // Talisman de la Pureza (reliquia 211).
    void aplicar(int &hp, int hpMax, bool &turnoPerdido, bool inmune = false) const;
};

// Busca un efecto por ID. Devuelve std::nullopt si no existe.
std::optional<Efecto> obtenerEfectoPorId(int id);

// =========================================================
// NUEVO: reliquias con efecto PASIVO de fin de ronda (Corazon de Fenix,
// Calavera de Valdrame). Se llama una vez por ronda desde iniciarCombate
// (Combate.cpp), separado de Efecto::aplicar porque estas reliquias no
// son un "estado" con duracion -- son permanentes mientras se porten.
// =========================================================
void procesarReliquiasTurno(int &hp, int hpMax, const std::vector<std::string>& reliquias, int &hpEnemigo);

#endif