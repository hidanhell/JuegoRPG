#include "catalogoObjetos.h"

// =========================================================
// CATALOGO DE ARMAS POR ZONA
// Formato: {ID, Nombre, Poder, Rareza, EfectoId, Clase, Zona, Precio, HabilidadId}
// EfectoId:    0 = ninguno, -1 = aleatorio (solo Raros)
// HabilidadId: 0 = ninguna, solo armas Elite llevan ID de habilidad especial
// =========================================================
std::vector<Arma> listaArmas = {

    // --- ELITE VALDRAME | Zona 1 (Loot exclusivo del Arzobispo) ---
    // EfectoId 4 = Paralisis | HabilidadId 301 = Grito del Caido (se activa al criticar)
    {82, "Filo del Sepulcro",       80, "Elite", 4, "Guerrero", 1, 0, 301},
    {83, "Baculo de las Almas",     80, "Elite", 4, "Mago",     1, 0, 301},
    {84, "Arco del Ultimo Aliento", 80, "Elite", 4, "Cazador",  1, 0, 301},

    // --- ZONA 1 | Aldea en las Sombras (Poder: 20-33 | Precio: 50-160) ---

    // Guerrero
    {1,  "Espada Oxidada",          20, "Comun", 0,  "Guerrero", 1, 50,  0},
    {2,  "Hacha Mellada",           22, "Comun", 0,  "Guerrero", 1, 60,  0},
    {3,  "Maza del Miliciano",      23, "Comun", 0,  "Guerrero", 1, 70,  0},
    {4,  "Espada Ancha",            24, "Comun", 0,  "Guerrero", 1, 75,  0},
    {5,  "Mandoble de Acero",       30, "Raro",  -1, "Guerrero", 1, 150, 0},
    {6,  "Hacha del Carnicero",     31, "Raro",  -1, "Guerrero", 1, 140, 0},

    // Mago
    {7,  "Varita Astillada",        20, "Comun", 0,  "Mago", 1, 55,  0},
    {8,  "Cetro de Cobre",          22, "Comun", 0,  "Mago", 1, 65,  0},
    {9,  "Baculo de Roble",         23, "Comun", 0,  "Mago", 1, 70,  0},
    {10, "Varita del Aprendiz",     25, "Comun", 0,  "Mago", 1, 80,  0},
    {11, "Cetro de las Sombras",    33, "Raro",  -1, "Mago", 1, 160, 0},
    {12, "Baculo de Clarividencia", 33, "Raro",  -1, "Mago", 1, 160, 0},

    // Cazador
    {13, "Arco de Caza",            20, "Comun", 0,  "Cazador", 1, 50,  0},
    {14, "Ballesta Ligera",         22, "Comun", 0,  "Cazador", 1, 60,  0},
    {15, "Arco Corto",              23, "Comun", 0,  "Cazador", 1, 70,  0},
    {16, "Trabuco Viejo",           24, "Comun", 0,  "Cazador", 1, 80,  0},
    {17, "Arco Veloz",              30, "Raro",  -1, "Cazador", 1, 150, 0},
    {18, "Ballesta de Precision",   31, "Raro",  -1, "Cazador", 1, 150, 0},

    // --- ZONA 2 | Bosque Oscuro (Poder: 35-45 | Precio: 250-460) ---

    // Guerrero
    {19, "Estoque de Plata",        35, "Comun", 0,  "Guerrero", 2, 250, 0},
    {20, "Maza del Hereje",         37, "Comun", 0,  "Guerrero", 2, 270, 0},
    {21, "Hacha de Onix",           39, "Comun", 0,  "Guerrero", 2, 290, 0},
    {22, "Espada Mortal",           39, "Comun", 0,  "Guerrero", 2, 300, 0},
    {23, "Sed de Sangre",           45, "Raro",  -1, "Guerrero", 2, 450, 0},
    {24, "Mandoble Profanador",     45, "Raro",  -1, "Guerrero", 2, 450, 0},
    {25, "Maza Mutiladora",         46, "Raro",  -1, "Guerrero", 2, 450, 0},

    // Mago
    {26, "Varita de la Bruja",      37, "Comun", 0,  "Mago", 2, 270, 0},
    {27, "Cetro de Hueso",          39, "Comun", 0,  "Mago", 2, 290, 0},
    {28, "Baculo del Cuervo",       39, "Comun", 0,  "Mago", 2, 300, 0},
    {29, "Ojo de Reptil",           40, "Comun", 0,  "Mago", 2, 300, 0},
    {30, "Baculo del Nigromante",   45, "Raro",  -1, "Mago", 2, 450, 0},
    {31, "Cetro del Lich",          45, "Raro",  -1, "Mago", 2, 450, 0},
    {32, "Varita del Oraculo",      46, "Raro",  -1, "Mago", 2, 450, 0},

    // Cazador
    {33, "Arco del Acechador",      35, "Comun", 0,  "Cazador", 2, 250, 0},
    {34, "Ballesta de Hueso",       37, "Comun", 0,  "Cazador", 2, 270, 0},
    {35, "Trabuco del Bandido",     39, "Comun", 0,  "Cazador", 2, 290, 0},
    {36, "Arco de Mithril",         40, "Comun", 0,  "Cazador", 2, 300, 0},
    {37, "Colmillo de Fuego",       45, "Raro",  -1, "Cazador", 2, 460, 0},
    {38, "Rifle Corrupto",          45, "Raro",  -1, "Cazador", 2, 460, 0},
    {39, "Arco Sacro",              46, "Raro",  -1, "Cazador", 2, 460, 0},

    // --- ZONA 3 | Pantano Siniestro (Poder: 48-69 | Precio: 680-1200) ---

    // Guerrero
    {40, "Hacha del Verdugo",       50, "Comun", 0,  "Guerrero", 3, 700,  0},
    {41, "Espada del Caido",        52, "Comun", 0,  "Guerrero", 3, 700,  0},
    {42, "Maza del Oso Ciego",      55, "Comun", 0,  "Guerrero", 3, 750,  0},
    {43, "Aullasangre",             54, "Comun", 0,  "Guerrero", 3, 720,  0},
    {44, "Mazo Rompehuesos",        65, "Raro",  -1, "Guerrero", 3, 1100, 0},
    {45, "Anduril",                 65, "Raro",  -1, "Guerrero", 3, 1100, 0},
    {46, "Enma",                    68, "Raro",  -1, "Guerrero", 3, 1100, 0},

    // Mago
    {47, "Cetro del Pantano",       50, "Comun", 0,  "Mago", 3, 700,  0},
    {48, "Baculo del Fango",        52, "Comun", 0,  "Mago", 3, 700,  0},
    {49, "Varita Vudu",             52, "Comun", 0,  "Mago", 3, 700,  0},
    {50, "Cetro Runico",            53, "Comun", 0,  "Mago", 3, 720,  0},
    {51, "Vara de las Almas",       68, "Raro",  -1, "Mago", 3, 1200, 0},
    {52, "Cetro de Rubi",           68, "Raro",  -1, "Mago", 3, 1200, 0},
    {53, "Atiesh",                  69, "Raro",  -1, "Mago", 3, 1200, 0},

    // Cazador
    {54, "Arco del Cazarrecompensas",48, "Comun", 0, "Cazador", 3, 680,  0},
    {55, "Ballesta Pesada",          50, "Comun", 0, "Cazador", 3, 700,  0},
    {56, "Arco Fantasma",            50, "Comun", 0, "Cazador", 3, 700,  0},
    {57, "Rifle de Francotirador",   52, "Comun", 0, "Cazador", 3, 700,  0},
    {58, "Garras de Sombra",         64, "Raro",  -1, "Cazador", 3, 1050, 0},
    {59, "Vendaval de Luz",          64, "Raro",  -1, "Cazador", 3, 1050, 0},
    {60, "Thori'dal",                66, "Raro",  -1, "Cazador", 3, 1050, 0},

    // --- ZONA 4 | Tierra Desolada (Poder: 80-120 | Precio: 1500-3000) ---

    // Guerrero
    {61, "Mandoble de Obsidiana",   80,  "Comun", 0,  "Guerrero", 4, 1500, 0},
    {62, "Hacha del Titan",         82,  "Comun", 0,  "Guerrero", 4, 1500, 0},
    {63, "Maza Quebrantamundos",    83,  "Comun", 0,  "Guerrero", 4, 1500, 0},
    {64, "Agonia de Escarcha",      85,  "Comun", 0,  "Guerrero", 4, 1600, 0},
    {65, "Acero Demoniaco",         110, "Raro",  -1, "Guerrero", 4, 2800, 0},
    {66, "Hacha del Rey Demonio",   110, "Raro",  -1, "Guerrero", 4, 2800, 0},
    {67, "Masamune",                115, "Raro",  -1, "Guerrero", 4, 2800, 0},

    // Mago
    {68, "Baston del Aposento",     82,  "Comun", 0,  "Mago", 4, 1600, 0},
    {69, "Cetro del Abismo",        84,  "Comun", 0,  "Mago", 4, 1600, 0},
    {70, "Varita del Rey Caido",    85,  "Comun", 0,  "Mago", 4, 1600, 0},
    {71, "Rosa de Hielo",           86,  "Comun", 0,  "Mago", 4, 1600, 0},
    {72, "Baculo del Vacio",        115, "Raro",  -1, "Mago", 4, 3000, 0},
    {73, "Cetro Devora-Almas",      115, "Raro",  -1, "Mago", 4, 3000, 0},
    {74, "Ojo de la Tormenta",      120, "Raro",  -1, "Mago", 4, 3000, 0},

    // Cazador
    {75, "Arco de Hueso de Dragon", 80,  "Comun", 0,  "Cazador", 4, 1500, 0},
    {76, "Canon de Mano de Gilneas",82,  "Comun", 0,  "Cazador", 4, 1500, 0},
    {77, "Ballesta Abismal",        83,  "Comun", 0,  "Cazador", 4, 1500, 0},
    {78, "Brisaveloz",              85,  "Comun", 0,  "Cazador", 4, 1500, 0},
    {79, "Mordida del Leviatan",    113, "Raro",  -1, "Cazador", 4, 2900, 0},
    {80, "Lluvia de Estrellas",     113, "Raro",  -1, "Cazador", 4, 2900, 0},
    {81, "Arco de la Muerte",       113, "Raro",  -1, "Cazador", 4, 2900, 0}
};

// =========================================================
// CATALOGO DE ARTEFACTOS POR ZONA
// Formato: {ID, Nombre, Defensa, Rareza, EfectoId, Clase, Zona, Precio}
// =========================================================

std::vector<Artefacto> listaArtefactos = {
    // --- ZONA 1 | Aldea en las Sombras ---
    {501, "Escudo de Madera",         5, "Comun", 0,  "Guerrero", 1, 40},
    {502, "Grimorio Viejo",           4, "Comun", 0,  "Mago",     1, 40},
    {503, "Capa Desgastada",          3, "Comun", 0,  "Cazador",  1, 35},
    {504, "Escudo de Cobre",          7, "Raro",  -1, "Guerrero", 1, 100},
    {505, "Grimorio de Sombras",      8, "Raro",  -1, "Mago",     1, 110},
    {506, "Capa del Acechador",       6, "Raro",  -1, "Cazador",  1, 95},

    // --- ZONA 2 | Bosque Oscuro ---
    {507, "Escudo de Hierro",        12, "Comun", 0,  "Guerrero", 2, 200},
    {508, "Grimorio Maldito",        10, "Comun", 0,  "Mago",     2, 190},
    {509, "Capa de Hueso",            9, "Comun", 0,  "Cazador",  2, 180},
    {510, "Escudo de Bronce",        15, "Raro",  -1, "Guerrero", 2, 350},
    {511, "Grimorio Negro",          16, "Raro",  -1, "Mago",     2, 380},
    {512, "Capa de Fuego",           14, "Raro",  -1, "Cazador",  2, 340},

    // --- ZONA 3 | Pantano Siniestro ---
    {513, "Escudo de Acero",         20, "Comun", 0,  "Guerrero", 3, 600},
    {514, "Grimorio del Pantano",    18, "Comun", 0,  "Mago",     3, 550},
    {515, "Capa del Cazador",        17, "Comun", 0,  "Cazador",  3, 500},
    {516, "Escudo de Plata",         25, "Raro",  -1, "Guerrero", 3, 1000},
    {517, "Grimorio del Oraculo",    28, "Raro",  -1, "Mago",     3, 1100},
    {518, "Capa de Sombra",          24, "Raro",  -1, "Cazador",  3, 950},

    // --- ZONA 4 | Tierra Desolada ---
    {519, "Escudo de Obsidiana",     30, "Comun", 0,  "Guerrero", 4, 1500},
    {520, "Grimorio del Caos",       26, "Comun", 0,  "Mago",     4, 1400},
    {521, "Capa de Dragon",          25, "Comun", 0,  "Cazador",  4, 1300},
    {522, "Escudo de Mithril",       40, "Raro",  -1, "Guerrero", 4, 2500},
    {523, "Grimorio del Nigromante", 45, "Raro",  -1, "Mago",     4, 2800},
    {524, "Capa del Demonio",        42, "Raro",  -1, "Cazador",  4, 2600}
};

// =========================================================
// ARMAS LEGENDARIAS
// Disponibles en el cofre divino antes de Lancelot
// =========================================================
Arma excalibur(   1000, "Excalibur, Asesina de Dragones", 325, "Legendaria", 201, "Guerrero", 4, 5000, 0);
Arma bastonDragon(1001, "Baculo del Rey Dragon",          300, "Legendaria", 202, "Mago",     4, 5000, 0);
Arma arcoAlma(    1002, "Arco Alma de Dragon",            300, "Legendaria", 203, "Cazador",  4, 5000, 0);
Arma armaLancelot(1003, "Espada del Dios del Caos",       350, "Legendaria", 204, "Guerrero", 4, 7000, 0);

// =========================================================
// NUEVO: FILTRADO POR ZONA Y CLASE
// Estas dos funciones ya estaban declaradas en catalogoObjetos.h
// desde hace tiempo, pero nunca se habian implementado — loot.cpp
// filtraba a mano con su propio for/if en vez de usarlas. Se dejan
// listas y correctas aqui; migrar loot.cpp para que las use en vez
// de su filtrado manual queda pendiente para cuando revisemos ese
// archivo a fondo.
// =========================================================
std::vector<Arma> obtenerArmasZona(int zona, Clase clase) {
    std::vector<Arma> resultado;
    for (const auto& a : listaArmas) {
        if (a.zona == zona && a.clase == clase) {
            resultado.push_back(a);
        }
    }
    return resultado;
}
std::optional<Arma> obtenerArmaPorId(int id) {
    for (const auto& a : listaArmas) {
        if (a.id == id) return a;
    }
    return std::nullopt;
}

std::optional<Artefacto> obtenerArtefactoPorId(int id) {
    for (const auto& a : listaArtefactos) {
        if (a.id == id) return a;
    }
    return std::nullopt;
}
std::vector<Artefacto> obtenerArtefactosZona(int zona, Clase clase) {
    std::vector<Artefacto> resultado;
    for (const auto& a : listaArtefactos) {
        if (a.zona == zona && a.clase == clase) {
            resultado.push_back(a);
        }
    }
    return resultado;
}