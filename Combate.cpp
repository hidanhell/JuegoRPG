#include "combate.h"
#include "Utilidades.h"
#include "loot.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "Personajes.h"

using namespace std;

// =========================================================
// BLOQUE M1: ENEMIGOS Y COMBATE DETALLADO (v1.18)
// =========================================================

Monstruo generarEnemigo(int y) {
    int zona = (y <= 60) ? 1 : (y <= 90) ? 2 : (y <= 120) ? 3 : 4;
    bool esElite = (rand() % 100 < 25); // 25% de probabilidad de raro

    // Jefe Final
    if (y >= 150) return {"LANCELOT (EL REY DRAGON)", 2800, 650, 225, 55, 0, 10, false};

    // Enemigos normales y élite por zona,(hp, ataque, defensa, velocidad, expAlMorir, nivelEnemigo)
    if (zona == 1) return esElite ? Monstruo{"Tarantula Gigante", 120, 42, 15, 25, 60, 1, true} 
                                  : Monstruo{"Rata Gigante", 60, 22, 8, 20, 30, 1, false};
    if (zona == 2) return esElite ? Monstruo{"Ent Obscuro", 150, 62, 20, 40, 110, 2, true} 
                                  : Monstruo{"Serpiente Gigante", 110, 42, 15, 20, 60, 2, false};
    if (zona == 3) return esElite ? Monstruo{"Gorgona", 350, 102, 40, 45, 220, 3, true} 
                                  : Monstruo{"Orco de Pantano", 250, 82, 30, 25, 130, 3, false};
    return esElite ? Monstruo{"Gran Demonio", 650, 210, 65, 50, 650, 4, true} 
                   : Monstruo{"Caballero de Hierro", 480, 190, 55, 35, 400, 4, false};
}

void iniciarCombate(Personaje &p, int y) {
    Monstruo m = generarEnemigo(y);
    int hpInicial = m.hp;

    bool fase2 = false;
    bool fase3 = false;

    mostrarCabecera("COMBATE CONTRA: " + m.nombre + (m.esRaro ? " [RARO]" : ""));

    while (p.hp > 0 && m.hp > 0) {
        // --- FASES DEL JEFE FINAL ---
        if (y >= 150 && !fase2 && m.hp <= (hpInicial * 0.5)) {
            fase2 = true;
            m.defensa = (int)(m.defensa * 1.30); //Incrementa 30% su Defensa
            cout << "\n¡LANCELOT CAMBIA SUS ESCAMAS! Ahora brillan como diamante, su defensa aumenta." << endl;
            system("pause");
        }
        if (y >= 150 && !fase3 && m.hp <= (hpInicial * 0.25)) {
            fase3 = true;
            m.ataque = (int)(m.ataque * 1.30); //Incrementa 30% su Ataque
            m.velocidad = (int)(m.velocidad * 1.30); //Incrementa 30% su Velocidad
            cout << "\n¡LANCELOT RUGE CON FURIA! Su espada arde con fuego eterno, su poder aumenta." << endl;
            system("pause");
        }

        // --- Turnos ---
        bool turnoJugador = (p.velocidadBase >= m.velocidad);
        for (int i = 0; i < 2; i++) {
            if ((i == 0 && turnoJugador) || (i == 1 && !turnoJugador)) {
                // Turno del Jugador
                int dTotal = p.ataqueBase + p.armaEquipada.atk;
                bool esCritico = (rand() % 100 < (5 + p.bonusCritico));
                int danoReal = max(2, dTotal - m.defensa);

                if (esCritico) {
                    danoReal = (int)(danoReal * 1.5);
                    cout << "[CRITICO] ¡Golpe devastador!" << endl;
                }

                if (p.tieneReliquia("Colmillo de Vampiro")) {
                    int roboVida = (int)(danoReal * 0.15);
                    p.hp = min(p.hpMax, p.hp + roboVida);
                    cout << "[RELIQUIA] El Colmillo de Vampiro te cura " << roboVida << " HP." << endl;
                }

                m.hp -= danoReal;
                cout << "[TI] " << p.nombreAtaque << " causa " << danoReal << " de dano." << endl;
                if (m.hp <= 0) break;
            } else {
                // Turno del Enemigo
                // Fórmula de daño escalada: depende de ataque, defensa y HP máximo del jugador
                double factorBase = (double)m.ataque / (p.defensaBase + p.escudoEquipado.defensa);
                int dEnemigo = (int)((p.hpMax * 0.05) * factorBase); // 5% del HP máximo escalado
                dEnemigo = max(1, dEnemigo);

                p.hp -= dEnemigo;
                cout << "[ENE] " << m.nombre << " te golpea por " << dEnemigo << " de dano." << endl;
                p.usarPocionAuto();

            if (p.hp <= 0 && p.tieneReliquia("Pluma del Fénix") && !p.usadaPluma) {
                p.hp = (int)(p.hpMax * 0.3);
                p.usadaPluma = true;
                cout << "[RELIQUIA] ¡La Pluma del Fénix te revive con " << p.hp << " HP!" << endl;
}

if (p.hp <= 0) break;

            }
        }

        if (m.hp > 0 && p.hp > 0) {
    cout << ">> " << p.nombre << endl;
    mostrarBarraHP(p.hp, p.hpMax);

    cout << ">> " << m.nombre << endl;
    mostrarBarraHP(m.hp, hpInicial);

    cout << "Presiona ENTER para el siguiente turno...";
    limpiarBuffer(); cin.get();
}

    }

    // --- Resultado del combate ---
    if (m.hp <= 0 && y < 150) {
        int oroGanado = (m.nivelEnemigo * 35) + (rand() % 25);
        cout << "\n¡ENEMIGO DERROTADO!" << endl;
        cout << "Ganaste " << oroGanado << " de Oro y " << m.expAlMorir << " puntos de Experiencia." << endl;
        p.oro += oroGanado;
        p.exp += m.expAlMorir;
        gestionarLoot(p, y, m.esRaro);
        p.subirNivel();
        system("pause");
    } else if (m.hp <= 0 && y >= 150) {
        cout << "\n¡LANCELOT HA CAIDO! El cielo se aclara por primera vez en décadas." << endl;
        cout << "Entre las cenizas recoges la ESCAMA DEL REY DRAGON, símbolo eterno de tu victoria." << endl;
        system("pause");
    } else if (p.hp <= 0) {
        p.reaparecer();
    }  
}