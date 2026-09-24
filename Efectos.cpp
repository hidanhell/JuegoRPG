#include "efectos.h"
#include "Rng.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

extern std::vector<Efecto> listaEfectos;
extern std::vector<Efecto> listaEfectosLegendarios;

std::optional<Efecto> obtenerEfectoPorId(int id) {
    for (const auto& e : listaEfectos)
        if (e.id == id) return e;
    for (const auto& e : listaEfectosLegendarios)
        if (e.id == id) return e;
    return std::nullopt;
}

// --- Constructor ---
Efecto::Efecto(int i, std::string n, std::string t, int d, int in)
    : id(i), nombre(n), tipo(t), duracion(d), intensidad(in) {}

void Efecto::mostrarInfo() const {
    std::cout << "ID: " << id
              << " | Efecto: " << nombre
              << " | Tipo: " << tipo
              << " | Duración: " << duracion << " turnos"
              << " | Intensidad: " << intensidad
              << std::endl;
}

// --- Aplicación de Efectos Comunes (IDs 1-7) ---
// NOTA: 'inmune' bloquea los IDs 1-7 por completo (Talisman de la Pureza).
// Esto es mas amplio que lo que dice el texto de la reliquia en
// ReliquiasDatos.cpp ("Veneno, Sangrado, Quemadura" = solo 1-3) -- decision
// de diseño pendiente de confirmar contigo: o se amplia el texto del item
// para reflejar que bloquea TODOS los estados, o se restringe esta condicion
// a (id >= 1 && id <= 3).
void Efecto::aplicar(int &hp, int hpMax, bool &turnoPerdido, bool inmune) const {
    
    // Si el jugador es inmune (por el Talisman de la Pureza), el efecto se bloquea
    if (inmune && (id >= 1 && id <= 7)) {
        std::cout << "[RELIQUIA] ¡El Talismán de la Pureza anula los efectos de " << nombre << "!\n";
        return;
    }

    int dmg = 0;

    switch (id) {
        case 1: // Quemadura
            dmg = (int)(hpMax * (intensidad / 100.0));
            hp -= dmg;
            std::cout << "[EFECTO] Quemadura causa " << dmg << " de daño.\n";
            break;

        case 2: // Veneno
            dmg = (int)(hpMax * (intensidad / 100.0));
            hp -= dmg;
            std::cout << "[EFECTO] Veneno causa " << dmg << " de daño.\n";
            break;

        case 3: // Sangrado
            dmg = intensidad;
            hp -= dmg;
            std::cout << "[EFECTO] Sangrado causa " << dmg << " de daño.\n";
            break;

        case 4: // Paralisis
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Paralisis te impide actuar este turno.\n";
            }
            break;

        case 5: // Evasion
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] ¡El ataque fue evadido!\n";
            }
            break;

        case 6: // Congelacion
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Congelación te inmoviliza este turno.\n";
            }
            break;

        case 7: // Desorientacion Arcana
            if (Rng::get().probabilidad(intensidad)) {
                turnoPerdido = true; 
                std::cout << "[EFECTO] La energía arcana te desorienta, fallas tu acción.\n";
            }
            break;

        default:
            // Caso 0 o IDs no definidos: no sucede nada
            break;
    }
}

// --- EFECTOS LEGENDARIOS ---
std::vector<Efecto> efectosLegendarios = {
    {201, "Corte del Rey Dragon", "Ofensivo", 1, 110},   // Excalibur
    {202, "Supernova", "Ofensivo", 1, 90},               // Bastón
    {203, "Flecha Divina", "Ofensivo", 1, 90},           // Arco
    {204, "Dios del Caos", "Estado", 1, 25}              // Lancelot
};

// --- Aplicación de Efectos Legendarios (IDs 201-204) ---
void aplicarEfectoLegendario(const Efecto &efecto, int &hpObjetivo, int ataqueBase, int &hpAtacante, int hpMaxAtacante, bool &turnoPerdido) {
    // Se activan solo si el atacante tiene <=40% de vida
    if (hpAtacante > (hpMaxAtacante * 0.4)) return;

    int dmg = 0;

    switch (efecto.id) {
        case 201: // Corte del Rey Dragon
            dmg = (int)(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Corte del Rey Dragon inflige " << dmg << " de daño.\n";
            break;

        case 202: // Supernova
            dmg = (int)(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Supernova explota causando " << dmg << " de daño.\n";
            break;

        case 203: // Flecha Divina
            dmg = (int)(ataqueBase * (efecto.intensidad / 100.0));
            hpObjetivo -= dmg;
            std::cout << "[LEGENDARIO] Flecha Divina atraviesa causando " << dmg << " de daño.\n";
            break;

        case 204: // Dios del Caos
            // Paralisis 25%
            if (Rng::get().probabilidad(efecto.intensidad)) {
                turnoPerdido = true;
                std::cout << "[LEGENDARIO] Dios del Caos paraliza al objetivo.\n";
            }
            // Curación 25%
            if (Rng::get().probabilidad(efecto.intensidad)) {
                int curacion = (int)(hpMaxAtacante * 0.15);
                hpAtacante += curacion;
                if (hpAtacante > hpMaxAtacante) hpAtacante = hpMaxAtacante;
                std::cout << "[LEGENDARIO] Dios del Caos restaura " << curacion << " de vida.\n";
            }
            break;

        default:
            break;
    }
}

// =========================================================
// GESTION PASIVA DE RELIQUIAS AL FINAL DEL TURNO
// =========================================================
void procesarReliquiasTurno(int &hp, int hpMax, const std::vector<std::string>& reliquias, int &hpEnemigo) {
    for (const auto& rel : reliquias) {
        if (rel == "Corazon de Fenix") {
            int curacion = (int)(hpMax * 0.05); // Cura el 5% de la vida maxima
            if (curacion < 1) curacion = 1;
            hp += curacion;
            if (hp > hpMax) hp = hpMax;
            std::cout << "[RELIQUIA] El Corazón de Fénix regenera " << curacion << " HP.\n";
        }
        else if (rel == "Calavera de Valdrame") {
            int robo = (int)(hpEnemigo * 0.10); // Drena el 10% de la vida actual del enemigo
            if (robo < 1) robo = 1;
            hpEnemigo -= robo;
            // FIX: sin este clamp, drenar un enemigo con poco HP restante
            // podia dejar hpEnemigo en negativo (ej. -1), lo cual se veria
            // mal en el HUD aunque el combate ya lo trate como "muerto".
            if (hpEnemigo < 0) hpEnemigo = 0;
            hp += robo;
            if (hp > hpMax) hp = hpMax;
            std::cout << "[RELIQUIA] La Calavera de Valdrame absorbe " << robo << " HP del enemigo.\n";
        }
    }
}