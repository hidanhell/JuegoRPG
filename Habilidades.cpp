#include "Habilidades.h"
#include "Personajes.h"
#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <optional>

extern std::vector<Habilidad> listaHabilidadesGlobal;
// ==========================
// FUNCIÓN GLOBAL DE ACCESO
// ==========================
std::optional<Habilidad> obtenerHabilidadPorId(int id) {
    for (auto &h : listaHabilidadesGlobal)
        if (h.id == id) return h;
    return std::nullopt;
}

// ============================================
// EJECUCIÓN DE HABILIDADES PERSONAJES
// ============================================
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;
    int dano = 0;
    bool turnoPerdido = false;
    switch (hab.id) {
        // --- Guerrero ---
        case 101: // Embate con Escudo
            dano = pj.ataqueBase + static_cast<int>(pj.defensaBase * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(4)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 102: // Tajo Sangriento
            dano = pj.ataqueBase + static_cast<int>(pj.fuerza * 1.3);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 103: // Frenesí
        {
            int golpes = Rng::get().entre(2, 4);
            int danoTotal = 0;
            for (int i = 0; i < golpes; i++) {
                int golpe = static_cast<int>(pj.ataqueBase * 1.5);
                m.hp -= golpe;
                danoTotal += golpe;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " realiza " << golpes
                      << " golpes e inflige un total de " << danoTotal << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
        }
            break;

        // --- Mago ---
        case 111: // Bola de Fuego
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(1)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 112: // Bola de Escarcha
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(6)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 113: // Rayo Arcano
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(7)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        // --- Cazador ---
        case 121: // Flecha Venenosa
            dano = pj.destreza + static_cast<int>(pj.destreza * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(2)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 122: // Disparo Preciso
            dano = pj.ataqueBase + static_cast<int>(pj.destreza * 1.5);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige un golpe critico de " << dano << " de dano.\n";
            // No aplica efecto, es puro daño
            break;

        case 123: // Lluvia de Flechas
        {
            int impactos = Rng::get().entre(3, 5);
            int danoTotal = 0;
            for (int i = 0; i < impactos; i++) {
                int golpe = static_cast<int>(pj.destreza * 1.4);
                m.hp -= golpe;
                danoTotal += golpe;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " lanza " << impactos 
                      << " flechas e inflige un total de " << danoTotal << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
        }
            break;

        default:
            std::cout << "[HABILIDAD] " << hab.nombre << " no tiene logica definida aún.\n";
            break;
    }
}

// ============================================
// APLICAR SUBCLASE NIVEL 15 (Pasivas)
// ============================================
void aplicarSubclase(Personaje &pj, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;

    switch (hab.id) {
        // Guerrero
        case 201: // Paladín
            pj.defensaBase = static_cast<int>(pj.defensaBase * 1.40);
            pj.vitalidad = static_cast<int>(pj.vitalidad * 1.40);
            pj.hpMax = static_cast<int>(pj.hpMax * 1.40); 
            pj.hp = pj.hpMax; // Lo curamos para que sienta el poder de la clase
            break;

        case 202: // Berserker
            pj.ataqueBase = static_cast<int>(pj.ataqueBase * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        // Mago
        case 211: // Fuego   
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.vitalidad = static_cast<int>(pj.vitalidad * 1.35);
            pj.hpMax = static_cast<int>(pj.hpMax * 1.35);
            pj.hp = pj.hpMax;
            break;

        case 212: // Hielo
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.defensaBase = static_cast<int>(pj.defensaBase * 1.35);
            break;

        // Cazador
        case 221: // Puntería
            pj.destreza = static_cast<int>(pj.destreza * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        case 222: // Bestias
            pj.destreza = static_cast<int>(pj.destreza * 1.35);
            pj.vitalidad = static_cast<int>(pj.vitalidad * 1.35);
            pj.hpMax = static_cast<int>(pj.hpMax * 1.35);
            pj.hp = pj.hpMax;
            // Queda listo para la lógica de invocar al compañero (oso, tigre, jabalí).
            break;

        default:
            std::cout << "[SUBCLASE] No definida aún.\n";
            break;
    }

    std::cout << "[SUBCLASE] " << hab.nombre << " activada: " 
              << hab.descripcion << "\n";
}