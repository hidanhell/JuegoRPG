#pragma once
#include <string>
#include <vector>
#include "Armas.h"
#include "Artefactos.h" 
#include "Efectos.h"
#include "Consumibles.h"

using namespace std;

struct Personaje {
    // --- Identidad y Lore ---
    string nombre, clase, nombreAtaque;
    int nivel, exp, oro;
    int posX, posY;

    // --- Atributos Base ---
    int fuerza;       // STR: Afecta Dano Fisico (Guerrero)
    int destreza;     // DEX: Afecta Punteria, Criticos y Huida (Cazador)
    int inteligencia; // INT: Afecta Dano Magico (Mago)
    int vitalidad;    // VIT: Define el HP Maximo

    // --- Estadisticas de Combate ---
    int hp, hpMax;
    int ataqueBase, defensaBase, velocidadBase;
    int bonusCritico;

    // --- Equipamiento e Inventario ---
    Arma armaEquipada;
    Artefacto artefactoEquipado;
    vector<Consumible> inventario; // Bolsa de consumibles (pociones, elixires)
    vector<string> reliquias;      // Objetos unicos con efectos pasivos
    
    // --- Estados y Control ---
    bool usadaPluma;
    vector<Efecto> efectos; // Veneno, Quemadura, Sangrado, etc.

    // --- Registro de habilidades desbloqueadas ---
    vector<int> habilidadesIds; // IDs de habilidades que el personaje ha aprendido

    // --- Flags de progresion ---
    bool tieneHabilidad10 = false;   // Controla desbloqueo de nivel 10
    bool tieneSubclase15 = false;    // NUEVO: Controla desbloqueo de subclase nivel 15
    bool esBerserker = false;        // Modo berserker para el Guerrero al nivel 15

    // --- Funciones Principales ---
    Personaje(string n, int tipo);

    bool tieneReliquia(const string &r);
    void usarPocionAuto(); // Logica de supervivencia automatica
    void subirNivel();     // Escalado automatico de atributos y desbloqueo de habilidades
    void reaparecer();     // Reset al morir (vuelve a la aldea)
    
    // --- NUEVO METODO DE RPG ---
    // Recalcula HP Maximo, Ataque, Velocidad y Critico basandose en STR, DEX, INT y VIT
    void actualizarEstadisticas();
};