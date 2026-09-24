#include "Reliquias.h"

// =========================================================
// CATÁLOGO DE RELIQUIAS
// Formato: {ID, Nombre, Efecto, Rareza}
// =========================================================
std::vector<Reliquia> listaReliquias = {
    {201, "Colmillo de Vampiro",   "Roba 15% HP al atacar",                             "Raro"},
    {202, "Medallon del Guardian", "Defensa +10",                                       "Comun"},
    {203, "Anillo del Asesino",    "Critico +5%",                                       "Raro"},
    {204, "Garra del Berserker",   "Fuerza +8 (Solo Guerrero)",                         "Raro"},
    {205, "Ojo del Arcano",        "Inteligencia +8 (Solo Mago)",                       "Raro"},
    {206, "Pluma del Viento",      "Destreza +8 (Solo Cazador)",                        "Raro"},
    {207, "Botas del Relampago",   "Velocidad +10",                                     "Comun"},
    {208, "Amuleto de Sangre",     "20% chance de contraatacar x10 dano",               "Epico"},
    {209, "Piedra del Alma",       "Oro ganado por combate +25%",                       "Raro"},
    {210, "Calavera de Valdrame",  "Senor de los Muertos: absorves 5% HP del enemigo cada turno permanentemente", "Epico"},
    
    // --- NUEVAS RELIQUIAS DE BALANCE Y UTILIDAD ---
    {211, "Talisman de la Pureza", "Inmunidad total a estados (Veneno, Sangrado, Quemadura)", "Epico"},
    {212, "Escudo del Destino",    "Los enemigos no pueden asestarte golpes criticos",  "Epico"},
    {213, "Anillo de la Vitalidad","Salud Maxima +50",                                  "Comun"},
    {214, "Corazon de Fenix",      "Regeneras 5% de HP Maximo al final de cada turno",  "Epico"},
    
    // --- NUEVAS RELIQUIAS DE CLASE ---
    {215, "Hombreras del Coloso",  "Reduce dano fisico recibido 10% (Solo Guerrero)",   "Raro"},
    {216, "Gema del Eco",          "15% chance de doble ataque/hechizo (Solo Mago)",    "Raro"},
    {217, "Carcaj Inagotable",     "Ataques basicos ignoran 20% defensa (Solo Cazador)","Raro"}
};