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
    {103, "Frenesi", "Golpes multiples que causan sangrado pero te paralizan.", 20, 2, "Activa", 0},

    // --- Mago activas (nivel 10) ---
    {111, "Bola de Fuego", "Invocas una esfera ignea que inflige dano basado en tu intelecto +40%. Puede causar quemadura.", 0, 2, "Activa", 1},
    {112, "Bola de Escarcha", "Lanzas un proyectil helado que inflige dano basado en tu intelecto +40%. Puede causar congelacion.", 0, 2, "Activa", 6},
    {113, "Rayo Arcano", "Descargas energia arcana que inflige dano basado en tu intelecto +40%. Puede causar desorientacion magica.", 0, 2, "Activa", 7},

    // --- Cazador activas (nivel 10) ---
    {121, "Flecha Venenosa", "Disparas una flecha impregnada de veneno que inflige dano basado en tu destreza +40%. Aplica veneno.", 25, 2, "Activa", 2},
    {122, "Disparo Preciso", "Un disparo critico que inflige gran dano basado en tu destreza + ataque. No aplica efectos.", 35, 2, "Activa", 0},
    {123, "Lluvia de Flechas", "Lanzas multiples flechas que infligen dano acumulado y pueden causar sangrado.", 30, 3, "Activa", 3},

    // --- Guerrero pasivas (nivel 15 - subclases) ---
    {201, "Paladin", "Incrementa defensa y vitalidad en 40%.", 0, 0, "Pasiva", 0},
    {202, "Berserker", "Incrementa ataque y velocidad en 40%.", 0, 0, "Pasiva", 0},

    // --- Mago pasivas (nivel 15 - subclases) ---
    {211, "Mago Fuego", "Incrementa intelecto y vitalidad en 35%.", 0, 0, "Pasiva", 0},
    {212, "Mago Hielo", "Incrementa intelecto y defensa en 35%.", 0, 0, "Pasiva", 0},

    // --- Cazador pasivas (nivel 15 - subclases) ---
    {221, "Punteria", "Incrementa destreza y velocidad en 40%.", 0, 0, "Pasiva", 0},
    {222, "Bestias", "Incrementa destreza y vitalidad en 35% y permite invocar companero.", 0, 0, "Pasiva", 0},

    // -------------------------------------------------------
    // HABILIDADES NIVEL 20 — ULTIMATES POR SUBCLASE
    // Tipo "Ultimate": las mas poderosas del juego por clase
    // -------------------------------------------------------

    // ID 301 — Paladin: Ira del Cielo
    // Dano: defensaBase * 2.5 + ataqueBase | Paralisis garantizada 2T | Autocura 15% hpMax
    {301, "Ira del Cielo",
          "Canalizas toda tu fe en un golpe descendente. Paralisis garantizada y te curas el 15% de tu vida maxima.",
          0, 0, "Ultimate", 4},

    // ID 302 — Berserker: Ultima Bestia
    // Dano: ataqueBase * 1.8 por golpe x 6-9 golpes
    // Si mata  -> recuperas HP perdido por autodano
    // Si no mata -> pierdes 10% HP propio + enemigo pierde 40% defensa por 2T
    {302, "Ultima Bestia",
          "Pierdes la razon. Solo queda el instinto y la sangre. Si no muere, algo en ti se rompe... y su armadura tambien. -40% defensa enemiga.",
          0, 0, "Ultimate", 3},

    // ID 311 — Mago Fuego: Colapso Solar
    // Dano: inteligencia * 3.5 | Quemadura garantizada 3T | Debuff propio: agotado (no puede usar habilidades 1T)
    {311, "Colapso Solar",
          "Invocas una estrella en miniatura. Quemadura garantizada 3 turnos, pero quedas agotado y no puedes usar habilidades el siguiente turno.",
          0, 0, "Ultimate", 1},

    // ID 312 — Mago Hielo: Absoluto Cero
    // Dano: inteligencia * 2.0 (x2 si ya esta congelado) | Congelacion garantizada 3T | -30% ATQ enemigo mientras dure
    {312, "Absoluto Cero",
          "El tiempo se detiene. Congelacion garantizada 3 turnos y reduce el ataque enemigo 30%. Si el enemigo ya esta congelado, el dano se duplica.",
          0, 0, "Ultimate", 6},

    // ID 321 — Punteria: Flecha del Juicio Final
    // Dano: destreza * 2.0 + ataqueBase * 1.5 | Ignora defensa | 50% veneno 3T | Requiere haber atacado el turno anterior
    {321, "Flecha del Juicio Final",
          "Cada combate anterior fue solo practica. Ignora toda la defensa del enemigo. Requiere haber atacado el turno anterior.",
          0, 0, "Ultimate", 2},

    // ID 322 — Bestias: Pacto de Sangre
    // Dano: destreza * 1.5 (tu) + destreza * 1.0 (companero) | Sangrado + Veneno garantizados | Companero absorbe el proximo golpe
    {322, "Pacto de Sangre",
          "Un silbido. Tu companero ataca junto a ti. Dos efectos simultaneos: sangrado y veneno. Tu companero absorbe el proximo golpe que recibirias.",
          0, 0, "Ultimate", 2},

    // -------------------------------------------------------
    // HABILIDADES DE ARMAS ELITE
    // Tipo "ArmaElite": se activan automaticamente al criticar
    // -------------------------------------------------------

    // ID 401 — Grito del Caido (armas Elite de Valdrame)
    // Espectro inflige 5% HP actual del enemigo + 40% chance paralisis
    {401, "Grito del Caido",
          "Un espectro surge del arma y lanza un grito que arranca el 5% del HP actual del enemigo. 40% de chance de paralizarlo.",
          0, 0, "ArmaElite", 4}
};