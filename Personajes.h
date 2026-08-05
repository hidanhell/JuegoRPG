#pragma once
#include <string>
#include <vector>
#include "Enums.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Efectos.h"
#include "Consumibles.h"

struct Monstruo; // Declaracion anticipada para evitar include circular

struct Personaje {
    // --- Identidad y Lore ---
    std::string nombre;
    Clase clase;
    std::string nombreAtaque;
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

    //------Recursos de clase (Ira, Mana, Enfoque)------

    int recursoActual = 0;
    int recursoMax = 100;
    std::string tipoRecurso = "Ninguno";

    // --- Flags de combate para Ultimates (nivel 20) ---
    bool ultimateUsada          = false; // Controla que la ultimate solo se use 1 vez por partida
    bool ultimoTurnoAtaco       = false; // Flecha del Juicio Final (331): requiere haber atacado el turno anterior
    int  turnosAgotado          = 0;     // Colapso Solar (321): bloquea usar habilidades N turnos
    int  turnosEscudoCompanero  = 0;     // Pacto de Sangre (332): companero absorbe el proximo golpe

    // --- Equipamiento e Inventario ---
    Arma armaEquipada;
    Artefacto artefactoEquipado;
    std::vector<Consumible> inventario; // Bolsa de consumibles (pociones, elixires)
    std::vector<std::string> reliquias;      // Objetos unicos con efectos pasivos
    
    // --- Estados y Control ---
    bool usadaPluma;
    std::vector<Efecto> efectos; // Veneno, Quemadura, Sangrado, etc.

    // --- Registro de habilidades desbloqueadas ---
    std::vector<int> habilidadesIds; // IDs de habilidades que el personaje ha aprendido

    // --- Flags de progresion ---
    bool tieneHabilidad10  = false;  // Controla desbloqueo de nivel 10
    bool tieneSubclase15   = false;  // Controla desbloqueo de subclase nivel 15
    bool tieneUltimate20   = false;  // Controla desbloqueo de ultimate nivel 20
    bool esBerserker       = false;  // Modo berserker para el Guerrero al nivel 15
    bool valdrameDerrotado = false; // Evita repetir el evento del jefe de la iglesia
    bool subclaseAplicada  = false; // Para no aplicar el x1.40 dos veces si vuelves al menu

    // --- Funciones Principales ---
    Personaje(std::string n, int tipo);

    bool tieneReliquia(const std::string &r);
    void usarPocionAuto(); // Logica de supervivencia automatica
    void subirNivel();     // Escalado automatico de atributos y desbloqueo de habilidades
    void reaparecer();     // Reset al morir (vuelve a la aldea)
    void actualizarEstadisticas(); // Recalcula HP Maximo, Ataque, Velocidad y Critico
};

// --- Funciones Globales de Ultimates (FUERA del struct) ---
// Busca en habilidadesIds cuál ultimate (300-399) tiene aprendida el personaje.
// Devuelve -1 si no tiene ninguna.
int obtenerUltimateIdDePersonaje(const Personaje &pj);

// Valida si el personaje puede usar su ultimate ahora mismo.
// Hoy solo chequea "desbloqueada y no gastada"; cuando implementes
// mana/ira/enfoque, el chequeo de recurso se agrega aqui adentro.
bool puedeUsarUltimate(const Personaje &pj);

// Ejecuta la ultimate correspondiente. Tiene su propio switch,
// separado de ejecutarHabilidad(), porque las reglas son distintas
// (un solo uso por partida, consumo total de recurso, etc.)
void ejecutarUltimate(Personaje &pj, Monstruo &m, int idHabilidad);

