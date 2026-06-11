#include "HabilidadesMonstruo.h"
#include "Monstruos.h"
#include "Personajes.h"
#include "Efectos.h"
#include <iostream>
#include "Rng.h"
#include <algorithm> // Para usar max y min sin problemas

using namespace std;

// =========================================================
// CATALOGO DE HABILIDADES DE MONSTRUOS
// =========================================================
vector<HabilidadMonstruo> listaHabilidadesMonstruo = {
    {301, "Golpe Venenoso",   "Ataca e inyecta veneno",          "Fisico",  2, 1.0f},
    {302, "Mordida Sangrienta","Muerde causando sangrado",        "Fisico",  3, 1.2f},
    {303, "Grito de Guerra",  "Se bufa el ataque por 2 turnos",  "Buff",    0, 0.0f},
    {304, "Huida",             "Intenta escapar del combate",      "Huida",   0, 0.0f},
    {305, "Escupitajo Acido", "Escupe acido que quema",           "Magico",  1, 0.8f},
    {306, "Paralizante",      "Lanza una descarga paralizante",   "Estado",  4, 0.0f}
};

std::optional<HabilidadMonstruo> obtenerHabilidadMonstruo(int id) {
    for (auto &h : listaHabilidadesMonstruo)
        if (h.id == id) return h;
    return std::nullopt;
}

void asignarHabilidadesMonstruo(Monstruo &m) {
    if (!m.esRaro && !m.esJefe) return; 

    bool esMagico = (m.inteligencia > m.fuerza);
    bool esCobarde = (m.velocidad > m.ataque * 2);

    if (esCobarde) {
        m.habilidadesIds.push_back(Rng::get().probabilidad(50) ? 301 : 304);
    } else if (esMagico) {
        m.habilidadesIds.push_back(Rng::get().probabilidad(50) ? 305 : 306);
    } else {
        m.habilidadesIds.push_back(Rng::get().probabilidad(50) ? 302 : 303);
    }
}

// Vinculamos la lista de efectos externa
extern std::vector<Efecto> listaEfectos;

bool ejecutarHabilidadMonstruo(Monstruo &m, Personaje &p, int idHabilidad) {
    auto habOpt = obtenerHabilidadMonstruo(idHabilidad);
    if (!habOpt) return false;
    HabilidadMonstruo hab = *habOpt;

    cout << "[ENE] " << m.nombre << " usa " << hab.nombre << "!" << endl;

    // --- HUIDA ---
    if (hab.tipo == "Huida") {
        // CORRECCIÓN: Convertimos el resultado de la multiplicación a int antes de comparar
        if (m.hp <= static_cast<int>(m.hpMax * 0.30f)) {
            int chanceHuida = 40 + (m.velocidad / 5);
            chanceHuida = max(20, min(chanceHuida, 80));

            if (Rng::get().probabilidad(chanceHuida)) {
                cout << "[ENE] " << m.nombre << " huye cobardemente!" << endl;
                cout << "[INFO] El combate termina sin recompensa." << endl;
                m.hp = 0; 
                return true; 
            } else {
                cout << "[ENE] " << m.nombre << " intento huir pero fallo!" << endl;
                return false;
            }
        } else {
            // Si tiene mucha vida, ataque básico
            int dano = max(1, m.ataque - p.defensaBase);
            p.hp -= dano;
            cout << "[ENE] No era el momento... " << m.nombre << " ataca por " << dano << " de dano." << endl;
            return false;
        }
    }

    // --- BUFF: GRITO DE GUERRA ---
    if (hab.tipo == "Buff") {
        // CORRECCIÓN: Usamos static_cast para evitar el warning de precisión
        m.ataque = static_cast<int>(m.ataque * 1.20f);
        cout << "[ENE] El ataque de " << m.nombre << " aumenta ferozmente!" << endl;
        return false;
    }

    // --- HABILIDADES CON DANO ---
    if (hab.multiDano > 0.0f) {
        // CORRECCIÓN: Aseguramos que el cálculo intermedio sea int
        int danoBase = static_cast<int>(static_cast<float>(m.ataque) * hab.multiDano);
        int danoReal = max(1, danoBase - p.defensaBase);
        p.hp -= danoReal;
        cout << "[ENE] " << hab.nombre << " causa " << danoReal << " de dano." << endl;
    }

    // --- APLICAR EFECTO DE ESTADO ---
    if (hab.efectoId > 0) {
        for (auto &e : listaEfectos) {
            if (e.id == hab.efectoId) {
                p.efectos.push_back(e);
                cout << "[ESTADO] Quedaste afectado por " << e.nombre << "!" << endl;
                break;
            }
        }
    }

    return false;
}