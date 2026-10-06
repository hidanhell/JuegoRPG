#include "Combate.h"
#include "IO.h"
#include "Utilidades.h"
#include "Loot.h"
#include "HabilidadesMonstruo.h"
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include "CatalogoObjetos.h"
#include "Consumibles.h"
#include "Habilidades.h"
#include "Rng.h"
#include "Recursos.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Invocación a dependencias externas
extern std::vector<Monstruo> listaMonstruos;
extern std::vector<Monstruo> generarHorda(int zonaDeseada);
extern Monstruo generarValdrame();

// Mapeo de profundidad Y a zonas del juego (1 a 5)
int obtenerZonaPorY(int y) {
    return (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : (y <= 240) ? 4 : 5;
}

// Generación aleatoria o filtrada de enemigo según posición
std::optional<Monstruo> generarEnemigo(int y) {
    int zona = obtenerZonaPorY(y);
    listaMonstruos = generarHorda(zona);

    bool esElite = Rng::get().probabilidad(25);
    std::vector<Monstruo> posibles;

    for (const auto& m : listaMonstruos) {
        if (m.zona == zona) {
            if ((esElite && m.esRaro) || (!esElite && !m.esRaro)) {
                posibles.push_back(m);
            }
        }
    }

    // Fallback: Si no hay coincidencias estrictas, buscar cualquiera de la zona
    if (posibles.empty()) {
        for (const auto& m : listaMonstruos) {
            if (m.zona == zona) posibles.push_back(m);
        }
    }

    if (!posibles.empty()) {
        int index = Rng::get().entre(0, static_cast<int>(posibles.size()) - 1);
        return posibles[static_cast<size_t>(index)];
    }

    return std::nullopt;
}

// Renderizado del HUD en pantalla
void mostrarHUDCombate(const Personaje &p, const Monstruo &m, int hpInicial, const string &etiqueta) {
    cout << "====================================================" << endl;
    cout << "  " << p.nombre << " [" << p.hp << "/" << p.hpMax << " HP] | "
         << p.tipoRecurso << ": " << p.recursoActual << "/" << p.recursoMax << endl;
    cout << "  VS  " << etiqueta << m.nombre
         << " (Nvl " << m.nivelEnemigo << ") [" << m.hp << "/" << hpInicial << " HP]" << endl;
    if (!m.naturaleza.empty() && m.naturaleza != "Normal") {
        cout << "  Naturaleza: " << m.naturaleza << endl;
    }
    cout << "====================================================" << endl;
}

// Aplicación de alteraciones de estado sobre el enemigo
bool procesarEstadosEnemigo(Monstruo &m, int hpInicial) {
    bool turnoBloqueado = false;
    for (auto it = m.efectosActivos.begin(); it != m.efectosActivos.end(); ) {
        bool saltarTurno = false;
        it->aplicar(m.hp, hpInicial, saltarTurno);
        cout << "[ESTADO EN ENEMIGO] " << it->nombre << " afecta a " << m.nombre << "." << endl;
        if (saltarTurno) turnoBloqueado = true;
        it->duracion--;
        if (it->duracion <= 0) {
            cout << "[INFO] El estado " << it->nombre << " ha expirado." << endl;
            it = m.efectosActivos.erase(it);
        } else {
            ++it;
        }
    }
    return turnoBloqueado;
}

// Aplicación de alteraciones de estado sobre el jugador (evalúa reliquias)
bool procesarEstadosJugador(Personaje &p) {
    bool turnoBloqueado = false;
    bool inmune = p.tieneReliquia("Talisman de la Pureza");
    for (auto it = p.efectos.begin(); it != p.efectos.end(); ) {
        bool saltarTurno = false;
        it->aplicar(p.hp, p.hpMax, saltarTurno, inmune);
        if (!inmune) {
            cout << "[ESTADO EN TI] " << it->nombre << " te esta afectando." << endl;
        }
        if (saltarTurno) turnoBloqueado = true;
        it->duracion--;
        if (it->duracion <= 0) {
            cout << "[INFO] Tu estado de " << it->nombre << " ha terminado." << endl;
            it = p.efectos.erase(it);
        } else {
            ++it;
        }
    }
    return turnoBloqueado;
}

// Descuento de consumibles y modificadores temporales
void procesarBuffsJugador(Personaje &p) {
    for (auto it = p.buffsActivos.begin(); it != p.buffsActivos.end(); ) {
        it->duracion--;
        if (it->duracion <= 0) {
            p.ataqueBase    -= it->bonusAtaque;
            p.defensaBase   -= it->bonusDefensa;
            p.velocidadBase -= it->bonusVelocidad;
            cout << "[BUFF] El efecto de " << it->nombre << " ha terminado." << endl;
            it = p.buffsActivos.erase(it);
        } else {
            ++it;
        }
    }
}

// Asignación de experiencia, oro, loot y muerte
void resolverFinDeCombate(Personaje &p, Monstruo &m, int y, bool huidaExitosa, bool monstruoHuyo) {
    if (huidaExitosa) return;

    if (monstruoHuyo) {
        cout << "\n[INFO] " << m.nombre << " ha huido cobardemente. No ganas recompensa." << endl;
        esperarTecla();
        return;
    }

    if (m.hp <= 0 && y < 241) {
        int oroGanado = (m.nivelEnemigo * 35) + Rng::get().entre(0, 24);
        if (p.tieneReliquia("Piedra del Alma")) {
            oroGanado = static_cast<int>(static_cast<float>(oroGanado) * 1.25f);
            cout << "[RELIQUIA] Piedra del Alma: +25% oro!" << endl;
        }

        cout << "\n--ENEMIGO DERROTADO--" << endl;
        if (m.nombre == "ARZOBISPO VALDRAME") {
            cout << "El Arzobispo cae de rodillas, su poder corrupto desvanecido." << endl;
            cout << "\"Angra... me ha abandonado...\"" << endl;
            cout << "Entre sus ropajes encuentras un objeto brillante." << endl;
        }
        cout << "Ganaste " << oroGanado << " de Oro y " << m.expAlMorir << " puntos de Experiencia." << endl;
        p.oro += oroGanado;
        p.exp += m.expAlMorir;

        if (m.nombre == "ARZOBISPO VALDRAME") {
            p.valdrameDerrotado = true;
            gestionarLootValdrame(p);
        } else {
            gestionarLoot(p, y, m.esRaro);
        }

        p.subirNivel();
        esperarTecla();
    } else if (m.hp <= 0 && y >= 241) {
        cout << "\n--LANCELOT HA CAIDO-- El cielo se aclara por primera vez en decadas." << endl;
        cout << "Entre las cenizas recoges la ESCAMA DEL REY DRAGON, simbolo eterno de tu victoria." << endl;
        esperarTecla();
    } else if (p.hp <= 0) {
        p.reaparecer();
    }
}

// Bucle principal de combate
void iniciarCombate(Personaje &p, int y) {
    IO::establecerModoMusica(ModoMusica::Combate);
    int turnosAbsorcion = 0;
    p.ultimoTurnoAtaco = false;
    p.turnosAgotado = 0;
    p.turnosEscudoCompanero = 0;

    Monstruo errorMonstruo{"Error", 1, 1, 1, 1, 0, 1, 1, false};
    Monstruo m = (y == 60 && !p.valdrameDerrotado) ? generarValdrame() : generarEnemigo(y).value_or(errorMonstruo);

    int hpInicial = m.hp;
    bool fase2 = false;
    bool fase3 = false;
    bool huidaExitosa = false;
    bool monstruoHuyo = false;
    bool faseValdrame2 = false;

    string etiqueta = m.esJefe ? "[JEFE] " : m.esElite ? "[ELITE] " : m.esRaro ? "[RARO] " : "";
    mostrarCabecera(string("COMBATE CONTRA: ") + etiqueta + m.nombre);

    if (m.naturaleza == "Feroz") {
        cout << "[!] ¡El enemigo ruge con furia! Sus ataques aumentan su ferocidad." << endl;
    } else if (m.naturaleza == "Robusto") {
        cout << "[!] ¡La piel del enemigo es dura como la roca! Su defensa es impenetrable." << endl;
    } else if (m.naturaleza == "Veloz") {
        cout << "[!] ¡El enemigo se mueve con agilidad pasmosa! Sus reflejos son letales." << endl;
    } else if (m.naturaleza == "Sabio") {
        cout << "[!] ¡Una energía arcana rodea al enemigo! Su poder mental es amenazante." << endl;
    } else if (m.naturaleza == "Salvaje") {
        cout << "[!] ¡El instinto salvaje desborda al enemigo! Su fuerza bruta no tiene freno." << endl;
    }

    if (m.nombre == "ARZOBISPO VALDRAME") {
        cout << "\nUna figura encapuchada bloquea el camino..." << endl;
        cout << "\"El caos de Angra me ha dado un poder que ningun mortal puede comprender.\"" << endl;
        cout << "\"Yo soy el Senor de los Muertos. Tu alma sera mi ofrenda.\"" << endl;
        esperarTecla();
    }

    while (p.hp > 0 && m.hp > 0) {
        if (m.turnosDebuffDefensa > 0) {
            m.turnosDebuffDefensa--;
            if (m.turnosDebuffDefensa == 0) {
                m.defensa = m.defensaOriginal;
                cout << "[DEBUFF] La armadura de " << m.nombre << " se ha recuperado." << endl;
            }
        }

        limpiarPantalla();
        mostrarHUDCombate(p, m, hpInicial, etiqueta);

        bool enemigoParalizado = procesarEstadosEnemigo(m, hpInicial);
        bool jugadorParalizado = procesarEstadosJugador(p);

        procesarBuffsJugador(p);

        if (m.hp > 0) {
            procesarReliquiasTurno(p.hp, p.hpMax, p.reliquias, m.hp);
        }

        if (m.hp <= 0 || p.hp <= 0) break;

        manejarFasesJefe(m, p, y, fase2, fase3, faseValdrame2, hpInicial, turnosAbsorcion);

        bool esTurnoJugador = (p.velocidadBase >= m.velocidad);

        for (int i = 0; i < 2; i++) {
            if (m.hp <= 0 || p.hp <= 0 || huidaExitosa || monstruoHuyo) break;

            if ((i == 0 && esTurnoJugador) || (i == 1 && !esTurnoJugador)) {
                if (jugadorParalizado) {
                    cout << "\n[ESTADO] ¡Estas paralizado y no puedes actuar este turno!" << endl;
                    esperarTecla();
                } else {
                    huidaExitosa = turnoJugador(p, m, turnosAbsorcion);
                }
                if (m.hp <= 0) break;
            } else {
                if (enemigoParalizado) {
                    cout << "\n[ESTADO] " << m.nombre << " esta paralizado y no puede actuar!" << endl;
                    esperarTecla();
                } else {
                    monstruoHuyo = turnoEnemigo(p, m, faseValdrame2, fase3);
                }
                if (p.hp <= 0) break;
            }
        }

        if (huidaExitosa || monstruoHuyo) break;

        if (m.hp > 0 && p.hp > 0) {
            cout << "\nPresiona ENTER para el siguiente turno...";
            esperarTecla();
        }
    }

    resolverFinDeCombate(p, m, y, huidaExitosa, monstruoHuyo);
    IO::establecerModoMusica(ModoMusica::Silencio);
}