#include "monstruos.h"
#include "HabilidadesMonstruo.h"
#include <vector>
#include <ctime>
#include <cstdlib>
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
    // Aplicamos static_cast para evitar warnings de precision de float a int
    if (n.statSube == "ATQ") m.ataque       = static_cast<int>(m.ataque       * n.multSube);
    if (n.statSube == "DEF") m.defensa      = static_cast<int>(m.defensa      * n.multSube);
    if (n.statSube == "VEL") m.velocidad    = static_cast<int>(m.velocidad    * n.multSube);
    if (n.statSube == "STR") m.fuerza       = static_cast<int>(m.fuerza       * n.multSube);
    if (n.statSube == "INT") m.inteligencia = static_cast<int>(m.inteligencia * n.multSube);

    if (n.statBaja == "ATQ") m.ataque       = static_cast<int>(m.ataque       * n.multBaja);
    if (n.statBaja == "DEF") m.defensa      = static_cast<int>(m.defensa      * n.multBaja);
    if (n.statBaja == "VEL") m.velocidad    = static_cast<int>(m.velocidad    * n.multBaja);
    if (n.statBaja == "STR") m.fuerza       = static_cast<int>(m.fuerza       * n.multBaja);
    if (n.statBaja == "INT") m.inteligencia = static_cast<int>(m.inteligencia * n.multBaja);

    m.naturaleza = n.nombre;
}

// =========================================================
// PLANTILLA Y CONSTRUCCION
// =========================================================

struct PlantillaMonstruo {
    std::string nombre;
    int baseSTR, baseDEX, baseINT, baseVIT;
    bool esRaro;
    bool esJefe;
};

int obtenerNivelAleatorio(int zona) {
    int minLvl = (zona - 1) * 5 + 1;
    int maxLvl = zona * 5;
    if (zona >= 5) return 30; // Nivel maximo para el Rey Dragon
    return minLvl + (rand() % (maxLvl - minLvl + 1));
}

Monstruo construirMonstruo(PlantillaMonstruo p, int zona) {
    int nivel = obtenerNivelAleatorio(zona);
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

    // Naturaleza aleatoria (Lancelot tiene naturaleza Divina fija)
    if (!p.esJefe) {
        aplicarNaturaleza(m, listaNaturalezas[rand() % 5]);
    } else {
        m.naturaleza = "Divina";
    }

    // Habilidades segun rareza
    asignarHabilidadesMonstruo(m);

    return m;
}

// =========================================================
// GENERACION DE HORDA POR ZONA
// =========================================================

std::vector<Monstruo> generarHorda(int zonaDeseada) {
    std::vector<PlantillaMonstruo> plantillas;

    // Zona 1: La Aldea Sombría
    if (zonaDeseada == 1) {
        plantillas = {
            {"Rata Gigante",         8,  10,  2,  8,  false, false},
            {"Perro Rabioso",        10, 10,  2,  10, false, false},
            {"Aldeano Zombie",       12, 5,   2,  12, false, false},
            {"Cuervo del Mal",       6,  15,  5,  6,  false, false},
            {"Sombra Fugaz",         5,  18,  10, 5,  false, false},
            {"Cucaracha de Plaga",   4,  8,   1,  15, false, false},
            {"Tarantula Gigante",    15, 15,  5,  15, true,  false},
            {"Carnicero Loco",       20, 5,   2,  20, true,  false},
            {"Espiritu de la Aldea", 5,  10,  25, 10, true,  false}
        };
    }
    // Zona 2: El Bosque de Musgo Corrupto
    else if (zonaDeseada == 2) {
        plantillas = {
            {"Serpiente Gigante", 15, 18, 5,  15, false, false},
            {"Lobo de Musgo",     18, 15, 2,  16, false, false},
            {"Ninfa Corrupta",    10, 20, 20, 12, false, false},
            {"Oso Cicatrizado",   25, 8,  2,  25, false, false},
            {"Avispa de Eter",    8,  25, 15, 8,  false, false},
            {"Raiz Viviente",     15, 5,  10, 20, false, false},
            {"Ent Obscuro",       30, 10, 15, 30, true,  false},
            {"Fauces de Madera",  35, 12, 5,  35, true,  false},
            {"Driada Oscura",     12, 25, 35, 15, true,  false}
        };
    }
    // Zona 3: Los Pantanos del Olvido
    else if (zonaDeseada == 3) {
        plantillas = {
            {"Orco de Pantano",     30, 12, 5,  30, false, false},
            {"Esqueleto Guerrero",  28, 15, 2,  25, false, false},
            {"Gusano de Fango",     25, 10, 5,  35, false, false},
            {"Fuego Fatuo",         5,  30, 40, 10, false, false},
            {"Rana de Acido",       15, 20, 20, 15, false, false},
            {"Cazador de Cabezas",  35, 25, 5,  20, false, false},
            {"Gorgona",             35, 25, 45, 40, true,  false},
            {"Hidra de Lodo",       50, 15, 20, 60, true,  false},
            {"Caminante del Fango", 40, 20, 30, 50, true,  false}
        };
    }
    // Zona 4: El Bastion de Ceniza
    else if (zonaDeseada == 4) {
        plantillas = {
            {"Caballero de Hierro",  50, 20, 10, 50, false, false},
            {"Dragon Joven",         55, 30, 40, 55, false, false},
            {"Mago Obscuro",         15, 25, 70, 30, false, false},
            {"Gargola de Obsidiana", 45, 35, 20, 50, false, false},
            {"Bestia del Caos",      60, 40, 30, 45, false, false},
            {"Inquisidor Caido",     40, 30, 50, 40, false, false},
            {"Gran Demonio",         80, 50, 60, 80, true,  false},
            {"Lord de la Ceniza",    70, 40, 90, 70, true,  false},
            {"Wyvern de Sangre",     90, 60, 40, 70, true,  false}
        };
    }
    // Zona Final: El Trono del Rey Dragon
    else if (zonaDeseada == 5) {
        plantillas = {{"LANCELOT (EL REY DRAGON)", 400, 300, 200, 250, false, true}};
    }

    std::vector<Monstruo> listaResultante;
    for (const auto& p : plantillas) {
        listaResultante.push_back(construirMonstruo(p, zonaDeseada));
    }
    return listaResultante;
}

std::vector<Monstruo> listaMonstruos;