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

// Función para generar monstruos
Monstruo generarEnemigo(int y) {
    int zona = (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : 4;
    bool esElite = (rand() % 100 < 25); // 25% de probabilidad de élite

    // Jefe Final (zona 5)
    if (y >= 240) return {"LANCELOT (EL REY DRAGON)", 2800, 650, 225, 55, 0, 10, false};

    // Zona 1
    if (zona == 1) {
        if (esElite) {
            return Monstruo{"Tarantula Gigante", 120, 42, 15, 25, 60, 1, true};
        } else {
            std::vector<Monstruo> enemigosZona1 = //Se puede agregar mas facilmente Monstruos por zona
            {
                {"Rata", 60, 22, 8, 20, 30, 1, false},
                {"Perro Rabioso", 70, 25, 10, 18, 35, 1, false},
                {"Aldeano Zombie", 55, 20, 6, 30, 25, 1, false}
            };
            return enemigosZona1[static_cast<size_t>(rand()) % enemigosZona1.size()];

        }
    }

    // Zona 2
    if (zona == 2) {
        if (esElite) {
            return Monstruo{"Ent Obscuro", 150, 62, 20, 40, 110, 2, true};
        } else {
            std::vector<Monstruo> enemigosZona2 = {
                {"Serpiente Gigante", 110, 42, 15, 20, 60, 2, false},
                {"Lobo de Musgo", 95, 38, 12, 28, 55, 2, false},
                {"Ninfa del bosque", 95, 40, 12, 28, 55, 2, false}
            };
            return enemigosZona2[static_cast<size_t>(rand()) % enemigosZona2.size()];

        }
    }

    // Zona 3
    if (zona == 3) {
        if (esElite) {
            return Monstruo{"Gorgona", 350, 102, 40, 45, 220, 3, true};
        } else {
            std::vector<Monstruo> enemigosZona3 = {
                {"Orco de Pantano", 250, 82, 30, 25, 130, 3, false},
                {"Esqueleto Guerrero", 200, 75, 25, 30, 120, 3, false},
                {"Gusano del Pantano", 200, 83, 25, 30, 120, 3, false}
            };
            return enemigosZona3[static_cast<size_t>(rand()) % enemigosZona3.size()];

        }
    }

    // Zona 4
    if (zona == 4) {
        if (esElite) {
            return Monstruo{"Gran Demonio", 650, 210, 65, 50, 650, 4, true};
        } else {
            std::vector<Monstruo> enemigosZona4 = {
                {"Caballero de Hierro", 480, 190, 55, 35, 400, 4, false},
                {"Dragón Joven", 500, 180, 50, 40, 420, 4, false},
                {"Mago obscuro", 500, 185, 50, 40, 420, 4, false}
            };
            return enemigosZona4[static_cast<size_t>(rand()) % enemigosZona4.size()];

        }
    }

    // Fallback (no debería llegar aquí)
    return Monstruo{"Error", 1, 1, 1, 1, 0, 0, false};
}


//Funcion para iniciar el combate
void iniciarCombate(Personaje &p, int y) {
    Monstruo m = generarEnemigo(y);
    int hpInicial = m.hp;

    bool fase2 = false;
    bool fase3 = false;

    mostrarCabecera("COMBATE CONTRA: " + m.nombre + (m.esRaro ? " [RARO]" : ""));

    while (p.hp > 0 && m.hp > 0) {
        // --- COMBATE JEFE FINAL, SOLO SI SE LLEGA A LAS COORDENADAS CORRECTAS ---
        if (y >= 240 && !fase2 && m.hp <= (hpInicial * 0.5)) {
            fase2 = true;
            m.defensa = (int)(m.defensa * 1.30); //Incrementa 30% su Defensa
            cout << "\n¡LANCELOT CAMBIA SUS ESCAMAS! Ahora brillan como diamante, su defensa aumenta." << endl;
            system("pause");
        }
        if (y >= 240 && !fase3 && m.hp <= (hpInicial * 0.25)) {
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
    if (m.hp <= 0 && y < 240) {
        int oroGanado = (m.nivelEnemigo * 35) + (rand() % 25); //Oro ganado que depende del nivel del enemigo
        cout << "\n¡ENEMIGO DERROTADO!" << endl;
        cout << "Ganaste " << oroGanado << " de Oro y " << m.expAlMorir << " puntos de Experiencia." << endl;
        p.oro += oroGanado;
        p.exp += m.expAlMorir;
        gestionarLoot(p, y, m.esRaro);
        p.subirNivel();
        system("pause");

     // Este es es si ganas el combate final   
    } else if (m.hp <= 0 && y >= 240) {
        cout << "\n¡LANCELOT HA CAIDO! El cielo se aclara por primera vez en décadas." << endl;
        cout << "Entre las cenizas recoges la ESCAMA DEL REY DRAGON, símbolo eterno de tu victoria." << endl;
        system("pause");
    } else if (p.hp <= 0) {
        p.reaparecer();
    }  
}