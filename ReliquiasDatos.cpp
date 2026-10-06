#include "Reliquias.h"

// =========================================================
// CATÁLOGO DE RELIQUIAS
// Formato: {ID, Nombre, Efecto, Rareza}
// =========================================================
std::vector<Reliquia> listaReliquias = {
    {201, "Colmillo de Vampiro",   "Roba 15% del dano infligido con ataques basicos",   "Raro"},
    {202, "Medallon del Guardian", "Defensa +10",                                       "Comun"},
    {203, "Anillo del Asesino",    "Critico +5%",                                       "Raro"},
    {204, "Garra del Berserker",   "Fuerza +8 (Solo Guerrero)",                         "Raro"},
    {205, "Ojo del Arcano",        "Inteligencia +8 (Solo Mago)",                       "Raro"},
    {206, "Pluma del Viento",      "Destreza +8 (Solo Cazador)",                        "Raro"},
    {207, "Botas del Relampago",   "Velocidad +10",                                     "Comun"},
    {208, "Amuleto de Sangre",     "20% de probabilidad de contraatacar por 10 de dano al recibir un ataque basico", "Epico"},
    {209, "Piedra del Alma",       "Oro ganado por combate +25%",                       "Raro"},
    {210, "Calavera de Valdrame",  "Absorbe 5% del HP actual del enemigo cada ronda",   "Epico"},
    
    // --- NUEVAS RELIQUIAS DE BALANCE Y UTILIDAD ---
    {211, "Talisman de la Pureza", "Anula quemadura, veneno, sangrado, evasion, paralisis, congelacion y desorientacion", "Epico"},
    {212, "Escudo del Destino",    "Anula los golpes criticos de los ataques basicos enemigos", "Epico"},
    {213, "Anillo de la Vitalidad","Salud Maxima +50",                                  "Comun"},
    {214, "Corazon de Fenix",      "Al inicio de cada ronda, si sigues con vida, recuperas 5% de HP maximo", "Epico"},
    
    // --- NUEVAS RELIQUIAS DE CLASE ---
    {215, "Hombreras del Coloso",  "Reduce dano fisico recibido 10% (Solo Guerrero)",   "Raro"},
    {216, "Gema del Eco",          "15% de probabilidad de repetir el dano de un ataque basico (Solo Mago)", "Raro"},
    {217, "Carcaj Inagotable",     "Ataques basicos ignoran 20% defensa (Solo Cazador)","Raro"}
};