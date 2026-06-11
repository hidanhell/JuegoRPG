#include "monstruos.h"
#include "HabilidadesMonstruo.h"
#include <vector>
#include <ctime>
#include "Rng.h"
#include <iostream>
#include <algorithm> // Para usar std::max y std::min si fuera necesario

using namespace std;

// =========================================================
// SISTEMA DE NATURALEZAS
// =========================================================

struct Naturaleza {
    std::string nombre;
    float multSube;
    float multBaja;
    std::string statSube;
    std::string statBaja;
};

static const Naturaleza listaNaturalezas[5] = {
    {"Feroz",   1.10f, 0.90f, "ATQ", "DEF"},
    {"Robusto", 1.10f, 0.90f, "DEF", "VEL"},
    {"Veloz",   1.10f, 0.90f, "VEL", "ATQ"},
    {"Sabio",   1.10f, 0.90f, "INT", "STR"},
    {"Salvaje", 1.10f, 0.90f, "STR", "INT"}
};

void aplicarNaturaleza(Monstruo &m, const Naturaleza &n) {
    if (n.statSube == "ATQ") m.ataque       = static_cast<int>(static_cast<float>(m.ataque)       * n.multSube);
    if (n.statSube == "DEF") m.defensa      = static_cast<int>(static_cast<float>(m.defensa)      * n.multSube);
    if (n.statSube == "VEL") m.velocidad    = static_cast<int>(static_cast<float>(m.velocidad)    * n.multSube);
    if (n.statSube == "STR") m.fuerza       = static_cast<int>(static_cast<float>(m.fuerza)       * n.multSube);
    if (n.statSube == "INT") m.inteligencia = static_cast<int>(static_cast<float>(m.inteligencia) * n.multSube);

    if (n.statBaja == "ATQ") m.ataque       = static_cast<int>(static_cast<float>(m.ataque)       * n.multBaja);
    if (n.statBaja == "DEF") m.defensa      = static_cast<int>(static_cast<float>(m.defensa)      * n.multBaja);
    if (n.statBaja == "VEL") m.velocidad    = static_cast<int>(static_cast<float>(m.velocidad)    * n.multBaja);
    if (n.statBaja == "STR") m.fuerza       = static_cast<int>(static_cast<float>(m.fuerza)       * n.multBaja);
    if (n.statBaja == "INT") m.inteligencia = static_cast<int>(static_cast<float>(m.inteligencia) * n.multBaja);

    m.naturaleza = n.nombre;
}

// =========================================================
// PLANTILLA Y CONSTRUCCION
// =========================================================

struct PlantillaMonstruo {
    std::string nombre;
    int baseSTR, baseDEX, baseINT, baseVIT;
    bool esRaro;
    bool esElite; // Categoria nueva para mini-bosses con habilidades unicas
    bool esJefe;
};

int obtenerNivelAleatorio(int zona) {
    int minLvl = (zona - 1) * 5 + 1;
    int maxLvl = zona * 5;
    if (zona >= 5) return 30; // Nivel maximo para el Rey Dragon
    return Rng::get().entre(minLvl, maxLvl);
}

Monstruo construirMonstruo(PlantillaMonstruo p, int zona, int nivelFijo = -1) {
    // Si se pasa un nivel fijo (para eventos especiales como Valdrame), lo usamos
    // Si no, generamos el nivel aleatoriamente segun la zona
    int nivel = (nivelFijo > 0) ? nivelFijo : obtenerNivelAleatorio(zona);

    float multiRaro = p.esRaro ? 1.5f : 1.0f;

    // Calculo de stats base con escalado por nivel y rareza
    int str  = static_cast<int>(static_cast<float>(p.baseSTR + (nivel * 3)) * multiRaro);
    int dex  = static_cast<int>(static_cast<float>(p.baseDEX + (nivel * 2)) * multiRaro);
    int intel= static_cast<int>(static_cast<float>(p.baseINT + (nivel * 3)) * multiRaro);
    int vit  = static_cast<int>(static_cast<float>(p.baseVIT + (nivel * 4)) * multiRaro);

    int hp  = vit * 10;
    int atk = (str > intel) ? (str * 2) : (intel * 2);
    // Ajuste de defensa para que no sea impenetrable pero sea un reto
    int def = static_cast<int>(static_cast<float>(str) * 0.8f + static_cast<float>(nivel * 2));
    int vel = dex + (nivel * 2);
    int exp = static_cast<int>(static_cast<float>(nivel * 40) * multiRaro);

    Monstruo m(p.nombre, hp, atk, def, vel, exp, nivel, zona, p.esRaro, p.esJefe);
    m.fuerza       = str;
    m.destreza     = dex;
    m.inteligencia = intel;
    m.vitalidad    = vit;
    m.esElite      = p.esElite; // Propagamos el flag de elite al monstruo

    // Naturaleza aleatoria (Lancelot tiene naturaleza Divina fija)
    if (!p.esJefe) {
        aplicarNaturaleza(m, listaNaturalezas[Rng::get().entre(0, 4)]);
    } else {
        m.naturaleza = "Divina";
    }
    // Los Elites no tienen naturaleza — son unicos y sus stats son fijos por diseño

    // Habilidades segun rareza y tipo
    asignarHabilidadesMonstruo(m);

    return m;
}

// =========================================================
// FUNCION ESPECIAL: GENERAR AL ARZOBISPO VALDRAME
// Evento fijo — nivel 8, zona 1, Elite con fases propias
// Se llama desde combate.cpp cuando el jugador llega a Y=60
// =========================================================
Monstruo generarValdrame() {
    PlantillaMonstruo plantillaValdrame = {
        "ARZOBISPO VALDRAME",
        20,  // baseSTR — puede pegar fisico en fase 2
        10,  // baseDEX
        35,  // baseINT — predomina la magia oscura en fase 1
        30,  // baseVIT — resistente pero no imposible
        false, true, false // esRaro=false, esElite=true, esJefe=false
    };
    // Nivel fijo 8 — por encima del rango zona 1 (1-5) para que sea un reto real
    return construirMonstruo(plantillaValdrame, 1, 8);
}

// =========================================================
// GENERACION DE HORDA POR ZONA
// El Arzobispo ya NO esta aqui — es evento fijo manejado en combate.cpp
// =========================================================

std::vector<Monstruo> generarHorda(int zonaDeseada) {
    std::vector<PlantillaMonstruo> plantillas;

    // Zona 1: La Aldea Sombría
    if (zonaDeseada == 1) {
        plantillas = {
            // Comunes
            {"Rata Gigante",         8,  10,  2,  8,  false, false, false},
            {"Perro Rabioso",        10, 10,  2,  10, false, false, false},
            {"Aldeano Zombie",       12, 5,   2,  12, false, false, false},
            {"Cuervo del Mal",       6,  15,  5,  6,  false, false, false},
            {"Sombra Fugaz",         5,  18,  10, 5,  false, false, false},
            {"Cucaracha de Plaga",   4,  8,   1,  15, false, false, false},
            // Raros
            {"Tarantula Gigante",    15, 15,  5,  15, true,  false, false},
            {"Carnicero Loco",       20, 5,   2,  20, true,  false, false},
            {"Espiritu de la Aldea", 5,  10,  25, 10, true,  false, false}
            // Valdrame NO va aqui — es evento especial en Y=60
        };
    }
    // Zona 2: El Bosque de Musgo Corrupto
    else if (zonaDeseada == 2) {
        plantillas = {
            {"Serpiente Gigante", 15, 18, 5,  15, false, false, false},
            {"Lobo de Musgo",     18, 15, 2,  16, false, false, false},
            {"Ninfa Corrupta",    10, 20, 20, 12, false, false, false},
            {"Oso Cicatrizado",   25, 8,  2,  25, false, false, false},
            {"Avispa de Eter",    8,  25, 15, 8,  false, false, false},
            {"Raiz Viviente",     15, 5,  10, 20, false, false, false},
            {"Ent Obscuro",       30, 10, 15, 30, true,  false, false},
            {"Fauces de Madera",  35, 12, 5,  35, true,  false, false},
            {"Driada Oscura",     12, 25, 35, 15, true,  false, false}
        };
    }
    // Zona 3: Los Pantanos del Olvido
    else if (zonaDeseada == 3) {
        plantillas = {
            {"Orco de Pantano",     30, 12, 5,  30, false, false, false},
            {"Esqueleto Guerrero",  28, 15, 2,  25, false, false, false},
            {"Gusano de Fango",     25, 10, 5,  35, false, false, false},
            {"Fuego Fatuo",         5,  30, 40, 10, false, false, false},
            {"Rana de Acido",       15, 20, 20, 15, false, false, false},
            {"Cazador de Cabezas",  35, 25, 5,  20, false, false, false},
            {"Gorgona",             35, 25, 45, 40, true,  false, false},
            {"Hidra de Lodo",       50, 15, 20, 60, true,  false, false},
            {"Caminante del Fango", 40, 20, 30, 50, true,  false, false}
        };
    }
    // Zona 4: El Bastion de Ceniza
    else if (zonaDeseada == 4) {
        plantillas = {
            {"Caballero de Hierro",  50, 20, 10, 50, false, false, false},
            {"Dragon Joven",         55, 30, 40, 55, false, false, false},
            {"Mago Obscuro",         15, 25, 70, 30, false, false, false},
            {"Gargola de Obsidiana", 45, 35, 20, 50, false, false, false},
            {"Bestia del Caos",      60, 40, 30, 45, false, false, false},
            {"Inquisidor Caido",     40, 30, 50, 40, false, false, false},
            {"Gran Demonio",         80, 50, 60, 80, true,  false, false},
            {"Lord de la Ceniza",    70, 40, 90, 70, true,  false, false},
            {"Wyvern de Sangre",     90, 60, 40, 70, true,  false, false}
        };
    }
    // Zona Final: El Trono del Rey Dragon
    else if (zonaDeseada == 5) {
        plantillas = {{"LANCELOT (EL REY DRAGON)", 400, 300, 200, 250, false, false, true}};
    }

    std::vector<Monstruo> listaResultante;
    for (const auto& p : plantillas) {
        listaResultante.push_back(construirMonstruo(p, zonaDeseada));
    }
    return listaResultante;
}

std::vector<Monstruo> listaMonstruos;