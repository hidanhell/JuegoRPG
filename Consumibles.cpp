#include "Consumibles.h"

// =========================================================
// CATALOGO DE CONSUMIBLES
// Formato: {ID, Nombre, Desc, Cura, Atk, Def, Vel, Dur, Zona, Precio,
//          curaEstados, recursoRestaurado, danoDirecto, efectoAlUsar, huidaGarantizada}
// IDs de efectoAlUsar: 1 Quemadura | 2 Veneno | 3 Sangrado | 4 Paralisis | 6 Congelacion | 7 Desorientacion
// =========================================================
std::vector<Consumible> listaConsumibles = {
    // --- POCIONES DE VIDA ---
    {1, "Pocion Baja", "Cura levemente", 50, 0, 0, 0, 0, 1, 20},
    {2, "Pocion Media", "Cura moderadamente", 150, 0, 0, 0, 0, 2, 60},
    {3, "Pocion Alta", "Cura gran parte de la vida", 400, 0, 0, 0, 0, 3, 150},
    {4, "Pocion de Dragon", "Restaura toda la salud", 2000, 0, 0, 0, 0, 4, 400},

    // --- ELIXIRES DE ESTADISTICAS (Zonas 1-4) ---
    {10, "Elixir de Furia", "+15 Ataque (3 turnos)", 0, 15, 0, 0, 3, 1, 100},
    {11, "Elixir de Caparazon", "+15 Defensa (3 turnos)", 0, 0, 15, 0, 3, 1, 100},
    {12, "Elixir de Viento", "+15 Velocidad (3 turnos)", 0, 0, 0, 15, 3, 1, 100},
    {13, "Destilado de Heroe", "Buff completo (5 turnos)", 100, 25, 25, 25, 5, 4, 500},

    {14, "Elixir de Furia Mayor",        "+22 Ataque (3 turnos)",    0, 22, 0,  0,  3, 2, 180},
    {15, "Elixir de Caparazon Mayor",    "+22 Defensa (3 turnos)",   0, 0,  22, 0,  3, 2, 180},
    {16, "Elixir de Viento Mayor",       "+22 Velocidad (3 turnos)", 0, 0,  0,  22, 3, 2, 180},
    {17, "Elixir de Furia Superior",     "+30 Ataque (3 turnos)",    0, 30, 0,  0,  3, 3, 300},
    {18, "Elixir de Caparazon Superior", "+30 Defensa (3 turnos)",   0, 0,  30, 0,  3, 3, 300},
    {19, "Elixir de Viento Superior",    "+30 Velocidad (3 turnos)", 0, 0,  0,  30, 3, 3, 300},

    // --- LIMPIEZA DE ESTADOS ---
    {20, "Antidoto", "Elimina todos tus efectos de estado activos", 0, 0, 0, 0, 0, 1, 70, true},

    // --- RESTAURACION DE RECURSO (Ira / Mana / Enfoque) ---
    {21, "Tonico de Voluntad",       "Restaura 30 de tu recurso de clase", 0, 0, 0, 0, 0, 2, 140, false, 30},
    {22, "Tonico de Voluntad Mayor", "Restaura 55 de tu recurso de clase", 0, 0, 0, 0, 0, 3, 260, false, 55},

    // --- CONSUMIBLES OFENSIVOS ---
    {23, "Bomba Incendiaria", "35 de dano directo + Quemadura garantizada", 0, 0, 0, 0, 0, 2, 120, false, 0, 35, 1},
    {24, "Bomba Toxica", "30 de dano directo + Veneno garantizado", 0, 0, 0, 0, 0, 2, 120, false, 0, 30, 2},

    // --- UTILIDAD DE HUIDA ---
    {25, "Amuleto de Huida", "Garantiza escapar del combate al instante", 0, 0, 0, 0, 0, 1, 160, false, 0, 0, 0, true}
};