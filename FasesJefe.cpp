#include "FasesJefe.h"
#include "Utilidades.h"
#include <iostream>
#include <algorithm>

using namespace std;

// =========================================================
// TRANSFORMACIONES Y FASES DE JEFES
// =========================================================
void manejarFasesJefe(Monstruo &m, Personaje &p, int y,
                       bool &fase2, bool &fase3, bool &faseValdrame2,
                       int hpInicial, int &turnosAbsorcion) {

    // --- AURA PASIVA: ARZOBISPO VALDRAME ---
    if (m.nombre == "ARZOBISPO VALDRAME" && p.hp > 0) {
        int auraDrenado = max(1, static_cast<int>(static_cast<float>(p.hp) * 0.05f));
        p.hp -= auraDrenado;
        m.hp = min(hpInicial, m.hp + auraDrenado);
        cout << "\n[AMBIENTE] La presencia de Valdrame esta drenando tu energia..." << endl;
        cout << "[BOSS] El Senor de los Muertos absorbe " << auraDrenado << " HP de tu fuerza vital." << endl;
    }

    // --- FASE 2: ARZOBISPO VALDRAME (50% HP) ---
    if (m.nombre == "ARZOBISPO VALDRAME" && !faseValdrame2 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.5f)) {
        faseValdrame2 = true;
        m.ataque = static_cast<int>(static_cast<float>(m.fuerza) * 2.0f);
        cout << "\n[TRANSFORMACION] El Arzobispo grita: 'EL PUNIO SANTO ME GUIA!'" << endl;
        cout << "[MECANICA] Valdrame abandona la magia oscura y desata su poder fisico sagrado." << endl;
        cout << "[PELIGRO] Sus proximos golpes seran devastadores. Preparate." << endl;
        esperarTecla();
    }

    // --- FASE 2: LANCELOT - ESCUDO DEL REY (50% HP) ---
    if (y >= 241 && !fase2 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.5f)) {
        fase2 = true;
        m.defensa = static_cast<int>(static_cast<float>(m.defensa) * 1.30f);
        turnosAbsorcion = 2;
        cout << "\n  LANCELOT CAMBIA SUS ESCAMAS  Defensa aumentada." << endl;
        cout << "[MECANICA] Las escamas del Rey brillan con la magia corrupta de Angra." << endl;
        cout << "[PELIGRO] --Lancelot absorbera el dano de tus proximos--" << turnosAbsorcion << " ataques y lo convertira en vida!" << endl;
        esperarTecla();
    }

    // --- FASE 3: LANCELOT - FURIA DE ANGRA (25% HP) ---
    if (y >= 241 && !fase3 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.25f)) {
        fase3 = true;
        m.ataque = static_cast<int>(static_cast<float>(m.ataque) * 1.30f);
        m.velocidad = static_cast<int>(static_cast<float>(m.velocidad) * 1.30f);
        cout << "\n  LANCELOT RUGE CON FURIA  Ataque y Velocidad aumentados." << endl;
        cout << "[MECANICA] La sombra de Angra toma el control total del Rey para un golpe a traicion..." << endl;
        
        int danoEspecial = static_cast<int>(static_cast<float>(m.ataque) * 1.5f);
        p.hp -= danoEspecial;
        cout << "[ENE] --Lancelot ejecuta 'ALIENTO DEL VACIO' y te causa-- " << danoEspecial << " de dano directo!" << endl;
        
        if (p.hp > 0) {
            cout << "[SISTEMA] Has resistido el embate, pero estas gravemente herido." << endl;
        } else {
            cout << "[SISTEMA] El poder del Caos te ha abrumado..." << endl;
        }
        esperarTecla();
    }
}