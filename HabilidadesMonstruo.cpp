#include "HabilidadesMonstruo.h"
#include "Monstruos.h"
#include "Personajes.h"
#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <algorithm>

using namespace std;

// =========================================================
// CATÁLOGO DE HABILIDADES DE MONSTRUOS
// =========================================================
vector<HabilidadMonstruo> listaHabilidadesMonstruo = {
    {301, "Golpe Venenoso",     "Ataca e inyecta veneno",         "Fisico",     2, 1.0f},
    {302, "Mordida Sangrienta", "Muerde causando sangrado",        "Fisico",     3, 1.2f},
    {303, "Grito de Guerra",    "Incrementa ataque por 2 turnos", "Buff",       0, 0.0f},
    {304, "Huida",              "Intenta escapar del combate",     "Huida",      0, 0.0f},
    {305, "Escupitajo Acido",   "Escupe acido que quema",          "Magico",     1, 0.8f},
    {306, "Paralizante",        "Lanza una descarga paralizante",  "Estado",     4, 0.0f},
    
    // --- HABILIDADES DE JEFE: ARZOBISPO VALDRAME ---
    {401, "Senor de los Muertos", "Drena la esencia vital del enemigo",    "Vampirismo", 0, 0.0f},
    {402, "El Punio Santo",       "Golpe sagrado de poder divino oscuro",  "Sagrado",    0, 0.0f}
};

std::optional<HabilidadMonstruo> obtenerHabilidadMonstruo(int id) {
    for (auto &h : listaHabilidadesMonstruo) {
        if (h.id == id) return h;
    }
    return std::nullopt;
}

void asignarHabilidadesMonstruo(Monstruo &m) {
    if (!m.esRaro && !m.esJefe && !m.esElite) return; 

    // Arzobispo Valdrame
    if (m.nombre == "ARZOBISPO VALDRAME") {
        m.habilidadesIds.push_back(401); 
        m.habilidadesIds.push_back(402); 
        return;
    }

    // Asignación según zona del mapa
    if (m.zona == 1) {
        int habZona1 = Rng::get().probabilidad(50) ? 301 : 303;
        m.habilidadesIds.push_back(habZona1);
    } 
    else if (m.zona == 2) {
        int habZona2 = Rng::get().probabilidad(50) ? 302 : 304;
        m.habilidadesIds.push_back(habZona2);
    } 
    else if (m.zona >= 3) {
        int selector = Rng::get().entre(1, 3);
        if (selector == 1)      m.habilidadesIds.push_back(305);
        else if (selector == 2) m.habilidadesIds.push_back(306);
        else                    m.habilidadesIds.push_back(302);
    }
}

bool ejecutarHabilidadMonstruo(Monstruo &m, Personaje &p, int idHabilidad) {
    auto habOpt = obtenerHabilidadMonstruo(idHabilidad);
    if (!habOpt) return false;
    HabilidadMonstruo hab = *habOpt;

    cout << "[ENE] " << m.nombre << " usa " << hab.nombre << "!" << endl;

    // --- HUIDA ---
    if (hab.tipo == "Huida") {
        if (m.hp <= static_cast<int>(static_cast<float>(m.hpMax) * 0.30f)) {
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
            const double defensa = max(1, p.defensaBase + p.artefactoEquipado.defensa);
            int dano = max(1, static_cast<int>(p.hpMax * 0.05 * (m.ataque / defensa)));
            if (p.clase == Clase::Guerrero && p.tieneReliquia("Hombreras del Coloso")) {
                dano = max(1, static_cast<int>(dano * 0.90));
            }
            p.hp -= dano;
            cout << "[ENE] No era el momento... " << m.nombre << " ataca por " << dano << " de dano." << endl;
            return false;
        }
    }

    // --- BUFF ---
    if (hab.tipo == "Buff") {
        if (m.turnosBuffAtaque > 0) m.ataque -= m.bonusAtaqueBuff;
        m.bonusAtaqueBuff = max(1, static_cast<int>(static_cast<float>(m.ataque) * 0.20f));
        m.ataque += m.bonusAtaqueBuff;
        m.turnosBuffAtaque = 2;
        cout << "[ENE] El ataque de " << m.nombre << " aumenta ferozmente!" << endl;
        return false;
    }

    // --- VAMPIRISMO (Valdrame) ---
    if (hab.tipo == "Vampirismo") {
        if (Rng::get().probabilidad(50)) {
            int drenaje = static_cast<int>(static_cast<float>(p.hpMax) * 0.05f);
            drenaje = max(1, drenaje);
            p.hp -= drenaje;
            m.hp = min(m.hpMax, m.hp + drenaje);
            cout << "[ENE] Senor de los Muertos drena " << drenaje << " de tu esencia vital!" << endl;
            cout << "[ENE] El Arzobispo recupera " << drenaje << " HP." << endl;
        } else {
            int dano = max(1, m.ataque - p.defensaBase);
            p.hp -= dano;
            cout << "[ENE] " << m.nombre << " lanza magia oscura por " << dano << " de dano." << endl;
        }
        return false;
    }

    // --- SAGRADO (Valdrame Fase 2) ---
    if (hab.tipo == "Sagrado") {
        int dano = static_cast<int>(static_cast<float>(p.hpMax) * 0.30f);
        dano = max(1, dano);
        p.hp -= dano;
        cout << "[ENE] El Punio Santo golpea con poder divino corrupto por " << dano << " de dano!" << endl;
        return false;
    }

    // --- DAÑO GENÉRICO ---
    if (hab.multiDano > 0.0f) {
        int danoBase = static_cast<int>(static_cast<float>(m.ataque) * hab.multiDano);
        const double defensa = max(1, p.defensaBase + p.artefactoEquipado.defensa);
        int danoReal = max(1, static_cast<int>(p.hpMax * 0.05 * (danoBase / defensa)));
        if (hab.tipo == "Fisico" && p.clase == Clase::Guerrero && p.tieneReliquia("Hombreras del Coloso")) {
            danoReal = max(1, static_cast<int>(danoReal * 0.90));
        }
        p.hp -= danoReal;
        cout << "[ENE] " << hab.nombre << " causa " << danoReal << " de dano." << endl;
    }

    // --- APLICACIÓN DE ESTADO ---
    if (hab.efectoId > 0) {
        if (auto ef = obtenerEfectoPorId(hab.efectoId)) {
            agregarEfectoActivo(p.efectos, *ef);
            cout << "[ESTADO] Quedaste afectado por " << ef->nombre << "!" << endl;
        }
    }

    return false;
}