#include "Consumibles.h"

std::vector<Consumible> listaConsumibles = {
    // --- POCIONES DE VIDA (Escaladas por Zona) ---
    // ID, Nombre, Desc, Cura, Atk, Def, Vel, Dur, Zona, Precio
    {1, "Pocion Baja", "Cura levemente", 50, 0, 0, 0, 0, 1, 20},
    {2, "Pocion Media", "Cura moderadamente", 150, 0, 0, 0, 0, 2, 60},
    {3, "Pocion Alta", "Cura gran parte de la vida", 400, 0, 0, 0, 0, 3, 150},
    {4, "Pocion de Dragon", "Restaura toda la salud", 2000, 0, 0, 0, 0, 4, 400},

    // --- ELIXIRES DE ESTADISTICAS (Buffs temporales) ---
    {10, "Elixir de Furia", "+15 Ataque (3 turnos)", 0, 15, 0, 0, 3, 1, 100},
    {11, "Elixir de Caparazon", "+15 Defensa (3 turnos)", 0, 0, 15, 0, 3, 1, 100},
    {12, "Elixir de Viento", "+15 Velocidad (3 turnos)", 0, 0, 0, 15, 3, 1, 100},
    {13, "Destilado de Heroe", "Buff completo (5 turnos)", 100, 25, 25, 25, 5, 4, 500}
};