#include "catalogoObjetos.h"

// =========================================================
// CATALOGO DE ARMAS POR ZONA
// Formato: {ID, Nombre, Poder, Rareza, EfectoId, Clase, Zona, Precio}
// EfectoId: 0 = ninguno, -1 = aleatorio (solo Raros)
// =========================================================
std::vector<Arma> listaArmas = {

    // --- ZONA 1 | Aldea en las Sombras (Poder: 20-33 | Precio: 50-160) ---

    // Guerrero
    {1,  "Espada Oxidada",        20, "Comun", 0,  "Guerrero", 1, 50},
    {2,  "Hacha Mellada",         22, "Comun", 0,  "Guerrero", 1, 60},
    {3,  "Maza del Miliciano",    23, "Comun", 0,  "Guerrero", 1, 70},
    {4,  "Espada Ancha",          24, "Comun", 0,  "Guerrero", 1, 75},
    {5,  "Mandoble de Acero",     30, "Raro",  -1, "Guerrero", 1, 150},
    {6,  "Hacha del Carnicero",   31, "Raro",  -1, "Guerrero", 1, 140},

    // Mago
    {7,  "Varita Astillada",      20, "Comun", 0,  "Mago", 1, 55},
    {8,  "Cetro de Cobre",        22, "Comun", 0,  "Mago", 1, 65},
    {9,  "Baculo de Roble",       23, "Comun", 0,  "Mago", 1, 70},
    {10, "Varita del Aprendiz",   25, "Comun", 0,  "Mago", 1, 80},
    {11, "Cetro de las Sombras",  33, "Raro",  -1, "Mago", 1, 160},
    {12, "Baculo de Clarividencia",33, "Raro", -1, "Mago", 1, 160},

    // Cazador
    {13, "Arco de Caza",          20, "Comun", 0,  "Cazador", 1, 50},
    {14, "Ballesta Ligera",       22, "Comun", 0,  "Cazador", 1, 60},
    {15, "Arco Corto",            23, "Comun", 0,  "Cazador", 1, 70},
    {16, "Trabuco Viejo",         24, "Comun", 0,  "Cazador", 1, 80},
    {17, "Arco Veloz",            30, "Raro",  -1, "Cazador", 1, 150},
    {18, "Ballesta de Precision", 31, "Raro",  -1, "Cazador", 1, 150},

    // --- ZONA 2 | Bosque Oscuro (Poder: 35-45 | Precio: 250-460) ---

    // Guerrero
    {19, "Estoque de Plata",      35, "Comun", 0,  "Guerrero", 2, 250},
    {20, "Maza del Hereje",       37, "Comun", 0,  "Guerrero", 2, 270},
    {21, "Hacha de Onix",         39, "Comun", 0,  "Guerrero", 2, 290},
    {22, "Espada Mortal",         39, "Comun", 0,  "Guerrero", 2, 300}, // Guiño a FFVII
    {23, "Sed de Sangre",         45, "Raro",  -1, "Guerrero", 2, 450},
    {24, "Mandoble Profanador",   45, "Raro",  -1, "Guerrero", 2, 450},
    {25, "Maza Mutiladora",       46, "Raro",  -1, "Guerrero", 2, 450},

    // Mago
    {26, "Varita de la Bruja",    37, "Comun", 0,  "Mago", 2, 270},
    {27, "Cetro de Hueso",        39, "Comun", 0,  "Mago", 2, 290},
    {28, "Baculo del Cuervo",     39, "Comun", 0,  "Mago", 2, 300},
    {29, "Ojo de Reptil",         40, "Comun", 0,  "Mago", 2, 300},
    {30, "Baculo del Nigromante", 45, "Raro",  -1, "Mago", 2, 450},
    {31, "Cetro del Lich",        45, "Raro",  -1, "Mago", 2, 450},
    {32, "Varita del Oraculo",    46, "Raro",  -1, "Mago", 2, 450},

    // Cazador
    {33, "Arco del Acechador",    35, "Comun", 0,  "Cazador", 2, 250},
    {34, "Ballesta de Hueso",     37, "Comun", 0,  "Cazador", 2, 270},
    {35, "Trabuco del Bandido",   39, "Comun", 0,  "Cazador", 2, 290},
    {36, "Arco de Mithril",       40, "Comun", 0,  "Cazador", 2, 300},
    {37, "Colmillo de Fuego",     45, "Raro",  -1, "Cazador", 2, 460},
    {38, "Rifle Corrupto",        45, "Raro",  -1, "Cazador", 2, 460}, 
    {39, "Arco Sacro",            46, "Raro",  -1, "Cazador", 2, 460},

    // --- ZONA 3 | Pantano Siniestro (Poder: 48-69 | Precio: 680-1200) ---

    // Guerrero
    {40, "Hacha del Verdugo",     50, "Comun", 0,  "Guerrero", 3, 700},
    {41, "Espada del Caido",      52, "Comun", 0,  "Guerrero", 3, 700},
    {42, "Maza del Oso Ciego",    55, "Comun", 0,  "Guerrero", 3, 750},
    {43, "Aullasangre",           54, "Comun", 0,  "Guerrero", 3, 720}, // Guiño a WoW
    {44, "Mazo Rompehuesos",      65, "Raro",  -1, "Guerrero", 3, 1100},
    {45, "Anduril",               65, "Raro",  -1, "Guerrero", 3, 1100}, // Señor de los Anillos
    {46, "Enma",                  68, "Raro",  -1, "Guerrero", 3, 1100}, // Zoro!

    // Mago
    {47, "Cetro del Pantano",     50, "Comun", 0,  "Mago", 3, 700},
    {48, "Baculo del Fango",      52, "Comun", 0,  "Mago", 3, 700},
    {49, "Varita Vudu",           52, "Comun", 0,  "Mago", 3, 700},
    {50, "Cetro Runico",          53, "Comun", 0,  "Mago", 3, 720},
    {51, "Vara de las Almas",     68, "Raro",  -1, "Mago", 3, 1200}, 
    {52, "Cetro de Rubi",         68, "Raro",  -1, "Mago", 3, 1200},
    {53, "Atiesh",                69, "Raro",  -1, "Mago", 3, 1200}, // Guiño a WoW

    // Cazador
    {54, "Arco del Cazarrecompensas",48, "Comun", 0, "Cazador", 3, 680},
    {55, "Ballesta Pesada",       50, "Comun", 0,  "Cazador", 3, 700}, 
    {56, "Arco Fantasma",         50, "Comun", 0,  "Cazador", 3, 700},
    {57, "Rifle de Francotirador",52, "Comun", 0,  "Cazador", 3, 700}, 
    {58, "Garras de Sombra",      64, "Raro",  -1, "Cazador", 3, 1050}, 
    {59, "Vendaval de Luz",       64, "Raro",  -1, "Cazador", 3, 1050},
    {60, "Thori'dal",             66, "Raro",  -1, "Cazador", 3, 1050}, // Guiño a WoW

    // --- ZONA 4 | Tierra Desolada (Poder: 95-120 | Precio: 1800-3000) ---

    // Guerrero
    {61, "Mandoble de Obsidiana", 95, "Comun", 0,  "Guerrero", 4, 1800},
    {62, "Hacha del Titan",       97, "Comun", 0,  "Guerrero", 4, 1800},
    {63, "Maza Quebrantamundos",  97, "Comun", 0,  "Guerrero", 4, 1800},
    {64, "Agonia de Escarcha",    99, "Comun", 0,  "Guerrero", 4, 1900}, // Guiño a Frostmourne
    {65, "Acero Demoniaco",       110, "Raro", -1, "Guerrero", 4, 2800},
    {66, "Hacha del Rey Demonio", 110, "Raro", -1, "Guerrero", 4, 2800},
    {67, "Masamune",              115, "Raro", -1, "Guerrero", 4, 2800},

    // Mago
    {68, "Baston del Aposento",   105, "Comun", 0,  "Mago", 4, 2000}, // Guiño a Ainz Ooal Gown
    {69, "Cetro del Abismo",      105, "Comun", 0,  "Mago", 4, 2000}, 
    {70, "Varita del Rey Caido",  105, "Comun", 0,  "Mago", 4, 2000},
    {71, "Rosa de Hielo",         105, "Comun", 0,  "Mago", 4, 2000},
    {72, "Baculo del Vacío",      115, "Raro",  -1, "Mago", 4, 3000},
    {73, "Cetro Devora-Almas",    115, "Raro",  -1, "Mago", 4, 3000},
    {74, "Ojo de la Tormenta",    120, "Raro",  -1, "Mago", 4, 3000},

    // Cazador
    {75, "Arco de Hueso de Dragon",98, "Comun", 0,  "Cazador", 4, 1900},
    {76, "Cañon de Mano de Gilneas",99,"Comun", 0,  "Cazador", 4, 1900},
    {77, "Ballesta Abismal",      99, "Comun", 0,  "Cazador", 4, 1900}, 
    {78, "Brisaveloz",            100, "Comun", 0,  "Cazador", 4, 1900}, // Guiño a Windrunner
    {79, "Mordida del Leviatan",  113, "Raro",  -1, "Cazador", 4, 2900}, // Reemplazo del duplicado
    {80, "Lluvia de Estrellas",   113, "Raro",  -1, "Cazador", 4, 2900},
    {81, "Arco de la Muerte",     113, "Raro",  -1, "Cazador", 4, 2900}
};

// =========================================================
// CATALOGO DE ARTEFACTOS POR ZONA
// Formato: {ID, Nombre, Defensa, Rareza, EfectoId, Clase, Zona, Precio}
// =========================================================

std::vector<Artefacto> listaArtefactos = {
    // --- ZONA 1 | Aldea en las Sombras ---
    {501, "Escudo de Madera",      5, "Comun", 0,  "Guerrero", 1, 40},
    {502, "Grimorio Viejo",        4, "Comun", 0,  "Mago",     1, 40},
    {503, "Capa Desgastada",       3, "Comun", 0,  "Cazador",  1, 35},
    {504, "Escudo de Cobre",       7, "Raro",  -1, "Guerrero", 1, 100},
    {505, "Grimorio de Sombras",   8, "Raro",  -1, "Mago",     1, 110},
    {506, "Capa del Acechador",    6, "Raro",  -1, "Cazador",  1, 95},

    // --- ZONA 2 | Bosque Oscuro ---
    {507, "Escudo de Hierro",     12, "Comun", 0,  "Guerrero", 2, 200},
    {508, "Grimorio Maldito",     10, "Comun", 0,  "Mago",     2, 190},
    {509, "Capa de Hueso",         9, "Comun", 0,  "Cazador",  2, 180},
    {510, "Escudo de Bronce",     15, "Raro",  -1, "Guerrero", 2, 350},
    {511, "Grimorio Negro",       16, "Raro",  -1, "Mago",     2, 380},
    {512, "Capa de Fuego",        14, "Raro",  -1, "Cazador",  2, 340},

    // --- ZONA 3 | Pantano Siniestro ---
    {513, "Escudo de Acero",      20, "Comun", 0,  "Guerrero", 3, 600},
    {514, "Grimorio del Pantano", 18, "Comun", 0,  "Mago",     3, 550},
    {515, "Capa del Cazador",     17, "Comun", 0,  "Cazador",  3, 500},
    {516, "Escudo de Plata",      25, "Raro",  -1, "Guerrero", 3, 1000},
    {517, "Grimorio del Oraculo", 28, "Raro",  -1, "Mago",     3, 1100},
    {518, "Capa de Sombra",       24, "Raro",  -1, "Cazador",  3, 950},

    // --- ZONA 4 | Tierra Desolada ---
    {519, "Escudo de Obsidiana",  30, "Comun", 0,  "Guerrero", 4, 1500},
    {520, "Grimorio del Caos",    26, "Comun", 0,  "Mago",     4, 1400},
    {521, "Capa de Dragon",       25, "Comun", 0,  "Cazador",  4, 1300},
    {522, "Escudo de Mithril",    40, "Raro",  -1, "Guerrero", 4, 2500},
    {523, "Grimorio del Nigromante",45,"Raro", -1, "Mago",     4, 2800},
    {524, "Capa del Demonio",     42, "Raro",  -1, "Cazador",  4, 2600}
};

// =========================================================
// ARMAS LEGENDARIAS
// Disponibles en el cofre divino antes de Lancelot
// Cada una corresponde a una clase y tiene efecto legendario propio
// =========================================================
Arma excalibur(1000,    "Excalibur, Asesina de Dragones", 325, "Legendaria", 201, "Guerrero", 4, 5000);
Arma bastonDragon(1001, "Baculo del Rey Dragon",          300, "Legendaria", 202, "Mago",     4, 5000);
Arma arcoAlma(1002,     "Arco Alma de Dragon",            300, "Legendaria", 203, "Cazador",  4, 5000);
Arma armaLancelot(1003, "Espada del Dios del Caos",       350, "Legendaria", 204, "Guerrero", 4, 7000);