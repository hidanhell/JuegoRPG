#include "Reliquias.h"

// =========================================================
// CATÁLOGO DE RELIQUIAS
// Formato: {ID, Nombre, Efecto, Rareza}
// =========================================================
std::vector<Reliquia> listaReliquias = {
    {201, "Colmillo de Vampiro",  "Roba 15% HP al atacar",            "Raro"},
    {202, "Medallon del Guardian","Defensa +10",                       "Comun"},
    {203, "Anillo del Asesino",   "Critico +5%",                       "Raro"},
    {204, "Garra del Berserker",  "Fuerza +8 (Solo Guerrero)",         "Raro"},
    {205, "Ojo del Arcano",       "Inteligencia +8 (Solo Mago)",       "Raro"},
    {206, "Pluma del Viento",     "Destreza +8 (Solo Cazador)",        "Raro"},
    {207, "Botas del Relampago",  "Velocidad +10",                     "Comun"},
    {208, "Amuleto de Sangre",    "20% chance de contraatacar x10 dano","Epico"},
    {209, "Piedra del Alma",      "Oro ganado por combate +25%",       "Raro"},
    {210, "Calavera de Valdrame", "Senor de los Muertos: absorves 5% HP del enemigo cada turno permanentemente", "Epico"},
};