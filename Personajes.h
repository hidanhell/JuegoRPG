#pragma once
#include <string>
#include <vector>
using namespace std;

// BLOQUE M4: PERSONAJE
struct Arma {
    string nombre;
    int atk;
};

struct Escudo {
    string nombre;
    int defensa;
};

struct Personaje {
    string nombre, clase, nombreAtaque;
    int hp, hpMax, nivel, exp, ataqueBase, defensaBase, velocidadBase, posX, posY, oro;
    Arma armaEquipada;
    Escudo escudoEquipado;
    vector<string> inventario;
    vector<string> reliquias;
    bool usadaPluma;
    int bonusCritico;

    Personaje(string n, int tipo);

    bool tieneReliquia(const string &r);
    void usarPocionAuto();
    void subirNivel();
    void reaparecer();
};

