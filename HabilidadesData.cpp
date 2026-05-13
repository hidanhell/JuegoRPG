#include "Habilidades.h"
#include <vector>

// ==========================
// CATÁLOGO GLOBAL DE HABILIDADES
// ==========================
std::vector<Habilidad> listaHabilidadesGlobal = {
    // --- Guerrero pasivas (nivel 5) ---
    {1, "Guardia de Hierro", "Tu brazo se convierte en muralla. +5 defensa.", 5, 0, "Pasiva", 0},
    {2, "Furia del Acero", "El acero vibra con tu furia. +5 ataque.", 5, 0, "Pasiva", 0},
    {3, "Voluntad Inquebrantable", "Tu cuerpo resiste mas alla del dolor. +20 vida.", 20, 0, "Pasiva", 0},

    // --- Mago pasivas (nivel 5) ---
    {11, "Muro de Hielo", "El hielo endurece tu piel. +5 defensa.", 5, 0, "Pasiva", 0},
    {12, "Llama Arcana", "Tu magia se intensifica. +5 inteligencia.", 5, 0, "Pasiva", 0},
    {13, "Robo Arcano", "Absorbes energia vital. +20 vida.", 20, 0, "Pasiva", 0},

    // --- Cazador pasivas (nivel 5) ---
    {21, "Ojo de Halcon", "Tu mirada atraviesa la distancia. +5 destreza.", 5, 0, "Pasiva", 0},
    {22, "Paso Ligero", "Tus pasos son invisibles. +5 velocidad.", 5, 0, "Pasiva", 0},
    {23, "Instinto de Supervivencia", "Tu instinto te protege. +20 vida.", 20, 0, "Pasiva", 0},

    // --- Guerrero activas (nivel 10) ---
    {101, "Embate con Escudo", "Golpeas con tu escudo causando dano y paralisis.", 40, 2, "Activa", 4},
    {102, "Tajo Sangriento", "Un corte brutal que inflige dano y sangrado.", 30, 2, "Activa", 3},
    {103, "Frenesí", "Golpes multiples que causan sangrado pero te paralizan.", 20, 2, "Activa", 0},

    // --- Mago activas (nivel 10) ---
    {111, "Bola de Fuego", "Invocas una esfera ignea que inflige dano basado en tu intelecto +40%. Puede causar quemadura.", 0, 2, "Activa", 1},
    {112, "Bola de Escarcha", "Lanzas un proyectil helado que inflige dano basado en tu intelecto +40%. Puede causar congelación.", 0, 2, "Activa", 6},
    {113, "Rayo Arcano", "Descargas energia arcana que inflige dano basado en tu intelecto +40%. Puede causar desorientación mágica.", 0, 2, "Activa", 7},


    // --- Cazador activas (nivel 10) ---
    {121, "Flecha Venenosa", "Disparas una flecha impregnada de veneno que inflige dano basado en tu destreza +40%. Aplica veneno.", 25, 2, "Activa", 2},
    {122, "Disparo Preciso", "Un disparo crítico que inflige gran dano basado en tu destreza + ataque. No aplica efectos.", 35, 2, "Activa", 0},
    {123, "Lluvia de Flechas", "Lanzas múltiples flechas que infligen dano acumulado y pueden causar sangrado.", 30, 3, "Activa", 3},

    // --- Guerrero pasivas (nivel 15 - subclases) ---
    {201, "Paladin", "Incrementa defensa y vitalidad en 25%.", 0, 0, "Pasiva", 0},
    { 202, "Berserker", "Incrementa ataque y velocidad en 25%.", 0, 0, "Pasiva", 0},

    // --- Mago pasivas (nivel 15 - subclases) ---
    {211, "Mago Fuego", "Incrementa intelecto y vitalidad en 25%.", 0, 0, "Pasiva", 0},
    {212, "Mago Hielo", "Incrementa intelecto y defensa en 25%.", 0, 0, "Pasiva", 0},

    // --- Cazador pasivas (nivel 15 - subclases) ---
    {221, "Puntería", "Incrementa destreza y velocidad en 25%.", 0, 0, "Pasiva", 0},
    {222, "Bestias", "Incrementa destreza y defensa en 25% y permite invocar compañero.", 0, 0, "Pasiva", 0}

};

