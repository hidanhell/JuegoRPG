#ifndef EFECTOS_H
#define EFECTOS_H

#include <string>
#include <optional>
#include <vector>

// --- Efecto ---
struct Efecto {
    int id;                 // Identificador único
    std::string nombre;     // Nombre del efecto (Quemadura, Sangrado, Veneno, Parálisis)
    std::string tipo;       // Ofensivo, Defensivo, Estado
    int duracion;           // Turnos que dura
    int intensidad;         // Magnitud (% de HP, daño fijo o % de probabilidad)

    Efecto(int i = 0,
           std::string n = "-",
           std::string t = "Estado",
           int d = 0,
           int in = 0);

    void mostrarInfo() const;

    // Aplica el efecto sobre el objetivo.
    // 'inmune' (por defecto false) permite bloquear efectos 1-7 vía Talismán de la Pureza.
    void aplicar(int &hp, int hpMax, bool &turnoPerdido, bool inmune = false) const;
};

// Búsqueda de efecto por ID en catálogos
std::optional<Efecto> obtenerEfectoPorId(int id);
void agregarEfectoActivo(std::vector<Efecto>& efectos, const Efecto& nuevo);

// Procesamiento pasivo de reliquias al final de cada ronda
void procesarReliquiasTurno(int &hp, int hpMax, const std::vector<std::string>& reliquias, int &hpEnemigo);

#endif // EFECTOS_H