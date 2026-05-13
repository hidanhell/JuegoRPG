#include "monstruos.h"
#include <iostream>

// =========================================================
// SISTEMA DE ESTADISTICAS DINAMICAS (NUEVO)
// =========================================================

void Monstruo::actualizarEstadisticas() {
    // La vitalidad dicta el tope de vida (1 Vit = 10 HP)
    // Modificamos el hpMax, pero no el hp actual para no curarlo accidentalmente
    // si sus stats cambian a mitad del combate.
    hpMax = vitalidad * 10;
    
    // La fuerza bruta dicta que tan fuerte pega (1 Str = 2 Ataque)
    ataque = fuerza * 2;
    
    // La destreza dicta que tan rapido se mueve (1 Dex = 3 Velocidad)
    velocidad = destreza * 3;

    // Nota: La Defensa la mantenemos independiente de momento para que 
    // cuadre perfecto con tu lista actual de enemigos y no romperles el balance.
}

// =========================================================
// MOSTRAR INFORMACION
// =========================================================

void Monstruo::mostrarInfo() const {
    // --- Cabecera con Nombre y Nivel ---
    std::cout << "\n========================================" << std::endl;
    std::cout << " [ " << (esRaro ? "★ RARO ★ " : "") << nombre << " ]" << std::endl;
    std::cout << " Nivel: " << nivelEnemigo << " | Zona: " << zona << std::endl;
    
    // --- Estadisticas Vitales ---
    std::cout << " HP: " << hp << " / " << hpMax 
              << " | Atk: " << ataque 
              << " | Def: " << defensa 
              << " | Vel: " << velocidad << std::endl;

    // --- Atributos de Combate (NUEVO) ---
    // Esto te ayudara a ver si el escalado de Fuerza/Destreza es correcto
    std::cout << " STR: " << fuerza 
              << " | DEX: " << destreza 
              << " | INT: " << inteligencia 
              << " | VIT: " << vitalidad << std::endl;

    std::cout << " Recompensa Exp: " << expAlMorir << std::endl;
    std::cout << "========================================" << std::endl;
}