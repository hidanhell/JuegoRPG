#ifndef MONSTRUOS_H
#define MONSTRUOS_H

#include <string>
#include <vector>
#include "Efectos.h"    

struct Monstruo {
    // --- Identidad ---
    std::string nombre;
    std::string naturaleza; // Feroz, Robusto, Veloz, Sabio, Salvaje, Divina
    int nivelEnemigo;
    int zona;
    bool esRaro;
    bool esJefe;

    // --- Atributos Base ---
    int fuerza;
    int destreza;
    int inteligencia;
    int vitalidad;

    // --- Estadisticas Derivadas ---
    int hp, hpMax;
    int ataque;
    int defensa;
    int velocidad;
    int expAlMorir;

    // --- Estados y Habilidades ---
    std::vector<Efecto> efectosActivos;
    std::vector<int> habilidadesIds;

    Monstruo(std::string n = "-", int h = 0, int a = 0, int d = 0, int v = 0,
             int exp = 0, int lvl = 1, int z = 1, bool raro = false, bool jefe = false)
        : nombre(n), naturaleza("Normal"), nivelEnemigo(lvl), zona(z),
          esRaro(raro), esJefe(jefe),
          fuerza(a/2), destreza(v/3), inteligencia(0), vitalidad(h/10), 
          hp(h), hpMax(h), ataque(a), defensa(d), velocidad(v),
          expAlMorir(exp) {}

    void mostrarInfo() const;
    void actualizarEstadisticas();
    bool estaVivo() const { return hp > 0; }
};

#endif