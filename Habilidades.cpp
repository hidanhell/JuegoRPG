#include "Habilidades.h"
#include "Personajes.h"
#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <optional>

extern std::vector<Habilidad> listaHabilidadesGlobal;
// CORRECCIÓN: listaEfectos no estaba declarada como extern en este archivo
extern std::vector<Efecto> listaEfectos;

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

        // -----------------------------------------------
        // GUERRERO — ACTIVAS NIVEL 10
        // -----------------------------------------------

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

        // -----------------------------------------------
        // MAGO — ACTIVAS NIVEL 10
        // -----------------------------------------------

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

        // -----------------------------------------------
        // CAZADOR — ACTIVAS NIVEL 10
        // -----------------------------------------------

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
            std::cout << "[HABILIDAD] " << hab.nombre << " no tiene logica definida aun.\n";
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
        case 201: // Paladín
            pj.defensaBase = static_cast<int>(pj.defensaBase * 1.40);
            pj.vitalidad   = static_cast<int>(pj.vitalidad   * 1.40);
            pj.hpMax       = static_cast<int>(pj.hpMax       * 1.40);
            pj.hp = pj.hpMax;
            break;

        case 202: // Berserker
            pj.ataqueBase    = static_cast<int>(pj.ataqueBase    * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        case 211: // Mago Fuego
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.vitalidad    = static_cast<int>(pj.vitalidad    * 1.35);
            pj.hpMax        = static_cast<int>(pj.hpMax        * 1.35);
            pj.hp = pj.hpMax;
            break;

        case 212: // Mago Hielo
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.defensaBase  = static_cast<int>(pj.defensaBase  * 1.35);
            break;

        case 221: // Puntería
            pj.destreza      = static_cast<int>(pj.destreza      * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        case 222: // Bestias
            pj.destreza  = static_cast<int>(pj.destreza  * 1.35);
            pj.vitalidad = static_cast<int>(pj.vitalidad * 1.35);
            pj.hpMax     = static_cast<int>(pj.hpMax     * 1.35);
            pj.hp = pj.hpMax;
            break;

        default:
            std::cout << "[SUBCLASE] No definida aun.\n";
            break;
    }

    std::cout << "[SUBCLASE] " << hab.nombre << " activada: " << hab.descripcion << "\n";
}

// ============================================
// BLOQUE: HABILIDADES DE OBJETOS / ARMAS ELITE
// ============================================

// GRITO DEL CAIDO — Armas Elite de Valdrame
// Se activa automaticamente al hacer un critico con rareza "Elite"
// Efecto: Espectro baja 5% HP actual del enemigo + 40% chance de paralisis
void aplicarGritoDelCaido(Monstruo& m, Personaje& /*p*/) {
    std::cout << "[ESPECTRO] De tu arma surge un espectro que lanza un grito desgarrador!" << std::endl;

    int dano = std::max(1, static_cast<int>(static_cast<float>(m.hp) * 0.05f));
    m.hp -= dano;
    std::cout << "[ESPECTRO] El grito arranca " << dano << " HP de " << m.nombre << "!" << std::endl;

    if (rand() % 100 < 40) {
        for (auto& ef : listaEfectos) {
            if (ef.id == 4) {
                m.efectosActivos.push_back(ef);
                std::cout << "[ESPECTRO] " << m.nombre << " queda paralizado por el terror!" << std::endl;
                break;
            }
        }
    }
}