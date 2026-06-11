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
    {301, "Golpe Venenoso",    "Ataca e inyecta veneno",          "Fisico",     2, 1.0f},
    {302, "Mordida Sangrienta","Muerde causando sangrado",         "Fisico",     3, 1.2f},
    {303, "Grito de Guerra",   "Se bufa el ataque por 2 turnos",  "Buff",       0, 0.0f},
    {304, "Huida",             "Intenta escapar del combate",      "Huida",      0, 0.0f},
    {305, "Escupitajo Acido",  "Escupe acido que quema",           "Magico",     1, 0.8f},
    {306, "Paralizante",       "Lanza una descarga paralizante",   "Estado",     4, 0.0f},
    // === HABILIDADES UNICAS DEL ARZOBISPO VALDRAME ===
    // 401: Fase 1 — roba vida al jugador cada turno (5% HP max)
    // 402: Fase 2 — golpe fisico sagrado devastador (30% HP max del jugador)
    {401, "Senor de los Muertos", "Drena la esencia vital del enemigo",    "Vampirismo", 0, 0.0f},
    {402, "El Punio Santo",       "Golpe sagrado de poder divino oscuro",  "Sagrado",    0, 0.0f}
};

std::optional<HabilidadMonstruo> obtenerHabilidadMonstruo(int id) {
    for (auto &h : listaHabilidadesMonstruo)
        if (h.id == id) return h;
    return std::nullopt;
}

void asignarHabilidadesMonstruo(Monstruo &m) {
    if (!m.esRaro && !m.esJefe && !m.esElite) return; 

    // === MINI BOSS: ARZOBISPO VALDRAME ===
    // Recibe ambas habilidades; cual se usa depende de la fase (manejado en combate.cpp)
    if (m.nombre == "ARZOBISPO VALDRAME") {
        m.habilidadesIds.push_back(401); // Fase 1: Senor de los Muertos
        m.habilidadesIds.push_back(402); // Fase 2: El Punio Santo
        return;
    }

    // Logica general para Raros normales
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
            // Si tiene mucha vida, ataca normal en vez de huir
            int dano = max(1, m.ataque - p.defensaBase);
            p.hp -= dano;
            cout << "[ENE] No era el momento... " << m.nombre << " ataca por " << dano << " de dano." << endl;
            return false;
        }
    }

    // --- BUFF: GRITO DE GUERRA ---
    if (hab.tipo == "Buff") {
        m.ataque = static_cast<int>(static_cast<float>(m.ataque) * 1.20f);
        cout << "[ENE] El ataque de " << m.nombre << " aumenta ferozmente!" << endl;
        return false;
    }

    // =========================================================
    // NUEVO: VAMPIRISMO — SENOR DE LOS MUERTOS (ID 401)
    // Drena 5% del HP maximo del jugador y se lo suma al Arzobispo.
    // 50% de probabilidad de activarse en su turno para no ser injusto.
    // =========================================================
    if (hab.tipo == "Vampirismo") {
        if (rand() % 100 < 50) {
            int drenaje = static_cast<int>(static_cast<float>(p.hpMax) * 0.05f); // 5% HP max del jugador
            drenaje = max(1, drenaje);
            p.hp -= drenaje;
            m.hp = min(m.hpMax, m.hp + drenaje); // El Arzobispo se cura lo que drena
            cout << "[ENE] Senor de los Muertos drena " << drenaje << " de tu esencia vital!" << endl;
            cout << "[ENE] El Arzobispo recupera " << drenaje << " HP." << endl;
        } else {
            // Si no activa el vampirismo, ataca normal
            int dano = max(1, m.ataque - p.defensaBase);
            p.hp -= dano;
            cout << "[ENE] " << m.nombre << " lanza magia oscura por " << dano << " de dano." << endl;
        }
        return false;
    }

    // =========================================================
    // NUEVO: SAGRADO — EL PUNIO SANTO (ID 402)
    // Golpe fisico de fase 2. Causa 30% del HP maximo del jugador.
    // Se activa cuando el Arzobispo baja del 50% HP (manejado en combate.cpp).
    // =========================================================
    if (hab.tipo == "Sagrado") {
        int dano = static_cast<int>(static_cast<float>(p.hpMax) * 0.30f); // 30% HP max del jugador
        dano = max(1, dano);
        p.hp -= dano;
        cout << "[ENE] El Punio Santo golpea con poder divino corrupto por " << dano << " de dano!" << endl;
        return false;
    }

    // --- HABILIDADES CON DANO GENERICO ---
    if (hab.multiDano > 0.0f) {
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