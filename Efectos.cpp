#include "efectos.h"
#include <iostream>
#include <cstdlib>
#include <vector>

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

// --- Aplicación de Efectos Comunes (IDs 1-5) ---
void Efecto::aplicar(int &hp, int hpMax, bool &turnoPerdido) const {
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
            if (rand() % 100 < intensidad) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Paralisis te impide actuar este turno.\n";
            }
            break;

        case 5: // Evasion
            if (rand() % 100 < intensidad) {
                turnoPerdido = true;
                std::cout << "[EFECTO] ¡El ataque fue evadido!\n";
            }
            break;

        case 6: // Congelacion
            if (rand() % 100 < intensidad) {
                turnoPerdido = true;
                std::cout << "[EFECTO] Congelación te inmoviliza este turno.\n";
            }
            break;

        case 7: // Desorientacion Arcana
            if (rand() % 100 < intensidad) {
                turnoPerdido = true; // opción simple: perder turno
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
            if (rand() % 100 < efecto.intensidad) {
                turnoPerdido = true;
                std::cout << "[LEGENDARIO] Dios del Caos paraliza al objetivo.\n";
            }
            // Curación 25%
            if (rand() % 100 < efecto.intensidad) {
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