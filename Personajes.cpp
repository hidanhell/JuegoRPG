#include "personajes.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include "Utilidades.h"
using namespace std;

// BLOQUE M4: PERSONAJES

Personaje::Personaje(string n, int tipo) {
    nombre = n; nivel = 1; exp = 0; posX = 0; posY = 0; oro = 10;
    escudoEquipado = {"Ninguno", 0};
    armaEquipada = {"Manos", 2};
    usadaPluma = false;
    bonusCritico = 0;

    if (tipo == 1) {
        clase = "Guerrero"; nombreAtaque = "Hachazo Brutal";
        hpMax = 150; ataqueBase = 20; defensaBase = 28; velocidadBase = 10;
    } else if (tipo == 2) {
        clase = "Mago"; nombreAtaque = "Explosion Ignea";
        hpMax = 100; ataqueBase = 26; defensaBase = 4; velocidadBase = 18;
    } else {
        clase = "Cazador"; nombreAtaque = "Flecha Perforante";
        hpMax = 110; ataqueBase = 20; defensaBase = 6; velocidadBase = 28;
    }
    hp = hpMax;
}

bool Personaje::tieneReliquia(const string &r) {
    for (auto &rel : reliquias) {
        if (rel == r) return true;
    }
    return false;
}

void Personaje::usarPocionAuto() {
    if (inventario.empty()) return;
    int idx = -1;

    for(size_t i = 0; i < inventario.size(); i++) {
        if (hp < (hpMax * 0.35) && inventario[i] == "Pocion Alta") { idx = (int)i; break; }
        if (hp < (hpMax * 0.60) && inventario[i] == "Pocion Media") { idx = (int)i; break; }
        if (hp < (hpMax * 0.85) && inventario[i] == "Pocion Baja") { idx = (int)i; break; }
    }

    if (idx != -1) {
        string pNom = inventario[idx];
        float mod = (pNom == "Pocion Alta") ? 0.9f : (pNom == "Pocion Media") ? 0.6f : 0.3f;
        int heal = (int)(hpMax * mod);
        hp = min(hpMax, hp + heal);
        inventario.erase(inventario.begin() + idx);
        cout << "\n[SISTEMA] ¡Uso automatico de " << pNom << "! Recuperaste " << heal << " HP." << endl;
        cout << "[SISTEMA] HP actual: " << hp << "/" << hpMax << endl;
    }
}

void Personaje::subirNivel() {
    int expNecesaria = nivel * 50;
    while (exp >= expNecesaria) {
        exp -= expNecesaria;
        nivel++;
        hpMax += 50;
        hp = hpMax;
        ataqueBase += 7;
        defensaBase += 4;
        velocidadBase += 3;
        expNecesaria = nivel * 50;
        cout << "\n----------------------------------------" << endl;
        cout << ">>> ¡FELICIDADES! HAS SUBIDO AL NIVEL " << nivel << " <<<" << endl;
        cout << "Tus fuerzas aumentan considerablemente." << endl;
        cout << "----------------------------------------" << endl;
    }
}

void Personaje::reaparecer() {
    cout << "\n****************************************" << endl;
    cout << "   HAS CAIDO EN COMBATE... " << endl;
    cout << "****************************************" << endl;
    cout << "Despiertas con el cuerpo magullado en la Aldea." << endl;
    hp = hpMax;
    posY = 0;
    posX = 0;
    if (oro > 15) {
        cout << "Has perdido 15 de oro en el camino de regreso." << endl;
        oro -= 15;
    } else {
        oro = 0;
    }
    system("pause");
}

