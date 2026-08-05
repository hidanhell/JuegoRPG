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
    //
    // CORRECCIÓN: estos IDs estaban desincronizados con el switch
    // de ejecutarUltimate() en Habilidades.cpp (que usa 311/312
    // para Guerrero, 321/322 para Mago y 331/332 para Cazador).
    // Antes este catalogo tenia 301/302/311/312/321/322, lo que
    // causaba que cada ultimate ejecutara la logica de OTRA clase
    // (ej. el Mago Fuego terminaba ejecutando "Ira del Cielo" del
    // Paladin). Se renumeraron para que coincidan exactamente con
    // los case del switch. Nombres, descripciones y efectoId no
    // se tocaron.
    // -------------------------------------------------------

    // ID 311 — Paladin: Ira del Cielo
    // Dano: fuerza*2.0 + vitalidad*1.5 + ataqueBase | Paralisis garantizada | Autocura 25% hpMax
    {311, "Ira del Cielo",
     "Canalizas toda tu fe en un golpe descendente. Paralisis garantizada y te curas una parte de tu vida maxima.",
     0, 0, "Ultimate", 4},

    // ID 312 — Berserker: Ultima Bestia
    // Dano: 4 golpes basados en fuerza/ataqueBase, penetran parcialmente defensa
    // Si sobrevive -> -40% defensa enemiga por 2 turnos
    {312, "Ultima Bestia",
     "Pierdes la razon. Solo queda el instinto y la sangre. Si no muere, algo en ti se rompe... y su armadura tambien. -40% defensa enemiga.",
     0, 0, "Ultimate", 3},

    // ID 321 — Mago Fuego: Colapso Solar
    // Dano: inteligencia * 3.5 | Quemadura garantizada | Debuff propio: agotado 1 turno
    {321, "Colapso Solar",
     "Invocas una estrella en miniatura. Quemadura garantizada 3 turnos, pero quedas agotado y no puedes usar habilidades el siguiente turno.",
     0, 0, "Ultimate", 1},

    // ID 322 — Mago Hielo: Absoluto Cero
    // Dano: inteligencia * 2.8 (x2 si ya esta congelado) | Congelacion garantizada | -20% ATQ enemigo
    {322, "Absoluto Cero",
     "El tiempo se detiene. Congelacion garantizada y reduce el ataque enemigo. Si el enemigo ya esta congelado, el dano se duplica.",
     0, 0, "Ultimate", 6},

    // ID 331 — Punteria: Flecha del Juicio Final
    // Dano: destreza*3.2 + ataqueBase | Ignora defensa por completo | Requiere haber atacado el turno anterior
    {331, "Flecha del Juicio Final",
     "Cada combate anterior fue solo practica. Ignora toda la defensa del enemigo. Requiere haber atacado el turno anterior.",
     0, 0, "Ultimate", 2},

    // ID 332 — Bestias: Pacto de Sangre
    // 3 golpes (Oso/Lobo/Leon) | Sangrado + Veneno garantizados | Companero absorbe el proximo golpe
    {332, "Pacto de Sangre",
     "Un silbido. Tus companeros atacan junto a ti. Sangrado y veneno garantizados. Uno de ellos absorbe el proximo golpe que recibirias.",
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