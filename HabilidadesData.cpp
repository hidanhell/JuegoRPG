#include "Habilidades.h"
#include <vector>

// =========================================================
// CATÁLOGO GLOBAL DE HABILIDADES
// Formato: {ID, Nombre, Descripción, PoderBase, Duración, Tipo, EfectoId}
// Tipos: "Pasiva", "Activa", "Ultimate", "ArmaElite"
// =========================================================
std::vector<Habilidad> listaHabilidadesGlobal = {

    // --- GUERRERO: PASIVAS (NIVEL 5) ---
    {1, "Guardia de Hierro", "Tu brazo se convierte en muralla. +5 defensa.", 5, 0, "Pasiva", 0},
    {2, "Furia del Acero", "El acero vibra con tu furia. +5 ataque.", 5, 0, "Pasiva", 0},
    {3, "Voluntad Inquebrantable", "Tu cuerpo resiste mas alla del dolor. +20 vida.", 20, 0, "Pasiva", 0},

    // --- MAGO: PASIVAS (NIVEL 5) ---
    {11, "Muro de Hielo", "El hielo endurece tu piel. +5 defensa.", 5, 0, "Pasiva", 0},
    {12, "Llama Arcana", "Tu magia se intensifica. +5 inteligencia.", 5, 0, "Pasiva", 0},
    {13, "Robo Arcano", "Absorbes energia vital. +20 vida.", 20, 0, "Pasiva", 0},

    // --- CAZADOR: PASIVAS (NIVEL 5) ---
    {21, "Ojo de Halcon", "Tu mirada atraviesa la distancia. +5 destreza.", 5, 0, "Pasiva", 0},
    {22, "Paso Ligero", "Tus pasos son invisibles. +5 velocidad.", 5, 0, "Pasiva", 0},
    {23, "Instinto de Supervivencia", "Tu instinto te protege. +20 vida.", 20, 0, "Pasiva", 0},

    // --- GUERRERO: ACTIVAS (NIVEL 10) ---
    {101, "Embate con Escudo", "Golpeas con tu escudo causando dano y paralisis.", 40, 2, "Activa", 4},
    {102, "Tajo Sangriento", "Un corte brutal que inflige dano y sangrado.", 30, 2, "Activa", 3},
    {103, "Frenesi", "Golpes multiples que causan sangrado.", 20, 2, "Activa", 0},

    // --- MAGO: ACTIVAS (NIVEL 10) ---
    {111, "Bola de Fuego", "Invocas una esfera ignea basada en tu intelecto (+40%). Puede causar quemadura.", 0, 2, "Activa", 1},
    {112, "Bola de Escarcha", "Lanzas un proyectil helado basado en tu intelecto (+40%). Puede causar congelacion.", 0, 2, "Activa", 6},
    {113, "Rayo Arcano", "Descargas energia arcana basada en tu intelecto (+40%). Puede causar desorientacion.", 0, 2, "Activa", 7},

    // --- CAZADOR: ACTIVAS (NIVEL 10) ---
    {121, "Flecha Venenosa", "Disparas una flecha envenenada basada en tu destreza (+40%). Aplica veneno.", 25, 2, "Activa", 2},
    {122, "Disparo Preciso", "Un disparo crítico: 1.5x ataque base más destreza. No aplica efectos.", 35, 2, "Activa", 0},
    {123, "Lluvia de Flechas", "Lanzas multiples flechas que infligen dano acumulado y causan sangrado.", 30, 3, "Activa", 3},

    // --- GUERRERO: SUBCLASES PASIVAS (NIVEL 15) ---
    {201, "Paladin", "Incrementa defensa y vitalidad en 40%.", 0, 0, "Pasiva", 0},
    {202, "Berserker", "Incrementa ataque y velocidad en 40%.", 0, 0, "Pasiva", 0},

    // --- MAGO: SUBCLASES PASIVAS (NIVEL 15) ---
    {211, "Mago Fuego", "Incrementa intelecto y vitalidad en 35%.", 0, 0, "Pasiva", 0},
    {212, "Mago Hielo", "Incrementa intelecto y defensa en 35%.", 0, 0, "Pasiva", 0},

    // --- CAZADOR: SUBCLASES PASIVAS (NIVEL 15) ---
    {221, "Punteria", "Incrementa destreza y velocidad en 40%.", 0, 0, "Pasiva", 0},
    {222, "Bestias", "Incrementa destreza y vitalidad en 35% y permite invocar companero.", 0, 0, "Pasiva", 0},

    // --- GUERRERO: ULTIMATES (NIVEL 20) ---
    {311, "Ira del Cielo",
     "Inflige daño sagrado, aplica parálisis por 2 turnos (30% de probabilidad de perder la acción por turno) y cura 25% de tu HP máximo.",
     0, 0, "Ultimate", 4},

    {312, "Ultima Bestia",
     "Descargas 4 golpes brutales. Si el enemigo sobrevive, reduce su defensa un 40% durante 2 turnos.",
     0, 0, "Ultimate", 3},

    // --- MAGO: ULTIMATES (NIVEL 20) ---
    {321, "Colapso Solar",
     "Invocas una implosión solar con Quemadura garantizada. Quedas agotado el siguiente turno.",
     0, 0, "Ultimate", 1},

    {322, "Absoluto Cero",
     "Aplica congelación por 2 turnos (40% de probabilidad de perder la acción por turno), reduce 20% el ataque enemigo y duplica el daño si ya estaba congelado.",
     0, 0, "Ultimate", 6},

    // --- CAZADOR: ULTIMATES (NIVEL 20) ---
    {331, "Flecha del Juicio Final",
     "Disparo letal que ignora toda la defensa enemiga. Requiere haber atacado el turno anterior.",
     0, 0, "Ultimate", 2},

    {332, "Pacto de Sangre",
     "Ataque combinado con tus bestias. Sangrado y Veneno garantizados + interceptación del próximo ataque.",
     0, 0, "Ultimate", 2},

    // --- ARMAS ELITE ---
    {401, "Grito del Caido",
     "Un espectro arranca el 5% del HP actual del enemigo. 40% de probabilidad de paralizarlo.",
     0, 0, "ArmaElite", 4}
};