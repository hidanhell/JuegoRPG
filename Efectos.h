#ifndef EFECTOS_H
#define EFECTOS_H

#include <string>
#include <optional>

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
    void aplicar(int &hp, int hpMax, bool &turnoPerdido) const;
};

// Busca un efecto por ID. Devuelve std::nullopt si no existe.
std::optional<Efecto> obtenerEfectoPorId(int id);

#endif
