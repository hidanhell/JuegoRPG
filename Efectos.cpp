#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

extern std::vector<Efecto> listaEfectos;
extern std::vector<Efecto> listaEfectosLegendarios;

std::optional<Efecto> obtenerEfectoPorId(int id) {
    for (const auto& e : listaEfectos) {
        if (e.id == id) return e;
    }
    for (const auto& e : listaEfectosLegendarios) {
        if (e.id == id) return e;
    }
    return std::nullopt;
}

void agregarEfectoActivo(std::vector<Efecto>& efectos, const Efecto& nuevo) {
    auto existente = std::find_if(efectos.begin(), efectos.end(), [&nuevo](const Efecto& efecto) {
        return efecto.id == nuevo.id;
    });
    if (existente == efectos.end()) {
        efectos.push_back(nuevo);
        return;
    }

    existente->duracion = std::max(existente->duracion, nuevo.duracion);
    existente->intensidad = std::max(existente->intensidad, nuevo.intensidad);
}

// --- Constructor ---
Efecto::Efecto(int i, std::string n, std::string t, int d, int in)
    : id(i), nombre(std::move(n)), tipo(std::move(t)), duracion(d), intensidad(in) {}

void Efecto::mostrarInfo() const {
    std::cout << "ID: " << id
              << " | Efecto: " << nombre
              << " | Tipo: " << tipo
              << " | Duración: " << duracion << " turnos"
              << " | Intensidad: " << intensidad
              << '\n';
}

// --- Aplicación de Efectos Comunes (IDs 1-7) ---
void Efecto::aplicar(int &hp, int hpMax, bool &turnoPerdido, bool inmune) const {
    if (inmune && (id >= 1 && id <= 7)) {
        std::cout << "[RELIQUIA] ¡El Talismán de la Pureza anula los efectos de " << nombre << "!\n";
        return;
    }

    int dmg = 0;

    switch (id) {
        case 1: // Quemadura
            dmg = static_cast<int>(hpMax * (intensidad / 100.0));
            hp -= dmg;
            std::cout << "[EFECTO] Quemadura causa " << dmg << " de daño.\n";
            break;

        case 2: // Veneno
            dmg = static_cast<int>(hpMax * (intensidad / 100.0));
            hp -= dmg;
            std::cout << "[EFECTO] Veneno causa " << dmg << " de daño.\n";
            break;

        case 3: // Sangrado
            dmg = intensidad;
            hp -= dmg;
            std::cout << "[EFECTO] Sangrado causa " << dmg << " de daño.\n";
            break;

        case 4: // Parálisis
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Parálisis te impide actuar este turno.\n";
            }
            break;

        case 5: // Evasión
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] ¡El ataque fue evadido!\n";
            }
            break;

        case 6: // Congelación
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Congelación te inmoviliza este turno.\n";
            }
            break;

        case 7: // Desorientación Arcana
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true; 
                std::cout << "[EFECTO] La energía arcana te desorienta, fallas tu acción.\n";
            }
            break;

        default:
            break;
    }
}

// --- Aplicación de Efectos Legendarios (IDs 201-204) ---
void aplicarEfectoLegendario(const Efecto &efecto, int &hpObjetivo, int ataqueBase, int &hpAtacante, int hpMaxAtacante, bool &turnoPerdido) {
    if (hpAtacante > (hpMaxAtacante * 0.4)) return;

    int dmg = 0;

    switch (efecto.id) {
        case 201: // Corte del Rey Dragón
            dmg = static_cast<int>(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Corte del Rey Dragón inflige " << dmg << " de daño.\n";
            break;

        case 202: // Supernova
            dmg = static_cast<int>(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Supernova explota causando " << dmg << " de daño.\n";
            break;

        case 203: // Flecha Divina
            dmg = static_cast<int>(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Flecha Divina atraviesa causando " << dmg << " de daño.\n";
            break;

        case 204: // Dios del Caos
            if (Rng::get().probabilidad(efecto.intensidad)) {
                turnoPerdido = true;
                std::cout << "[LEGENDARIO] Dios del Caos paraliza al objetivo.\n";
            }
            if (Rng::get().probabilidad(efecto.intensidad)) {
                int curacion = static_cast<int>(hpMaxAtacante * 0.15);
                hpAtacante += curacion;
                if (hpAtacante > hpMaxAtacante) hpAtacante = hpMaxAtacante;
                std::cout << "[LEGENDARIO] Dios del Caos restaura " << curacion << " de vida.\n";
            }
            break;

        default:
            break;
    }
}

// --- Gestión pasiva de reliquias al final del turno ---
void procesarReliquiasTurno(int &hp, int hpMax, const std::vector<std::string>& reliquias, int &hpEnemigo) {
    for (const auto& rel : reliquias) {
        if (rel == "Corazon de Fenix" && hp > 0) {
            int curacion = static_cast<int>(hpMax * 0.05);
            if (curacion < 1) curacion = 1;
            hp += curacion;
            if (hp > hpMax) hp = hpMax;
            std::cout << "[RELIQUIA] El Corazón de Fénix regenera " << curacion << " HP.\n";
        }
        else if (rel == "Calavera de Valdrame") {
            int robo = static_cast<int>(hpEnemigo * 0.05);
            if (robo < 1) robo = 1;
            hpEnemigo -= robo;
            if (hpEnemigo < 0) hpEnemigo = 0;
            hp += robo;
            if (hp > hpMax) hp = hpMax;
            std::cout << "[RELIQUIA] La Calavera de Valdrame absorbe " << robo << " HP del enemigo.\n";
        }
    }
}