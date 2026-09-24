#include "combate.h"
#include "Utilidades.h"
#include "loot.h"
#include <iostream>
#include <algorithm>
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include "Rng.h"

using namespace std;

/// =========================================================
// BLOQUE M1: ENEMIGOS Y COMBATE DETALLADO 1.4
// =========================================================
// -------------------------------------------------------------------
// NOTA DE ARQUITECTURA:
// Este archivo se separo en varios modulos para que cada parte del
// combate se pueda leer y tocar por separado:
//   - FasesJefe.cpp     -> turno especial / jefes (Valdrame, Lancelot)
//   - TurnoJugador.cpp  -> menu tactico del jugador
//   - TurnoEnemigo.cpp  -> IA del enemigo
// Aqui en Combate.cpp queda solo el orquestador: generacion de
// enemigos, HUD, procesamiento de estados/buffs, resolucion final del
// combate, y el loop principal (iniciarCombate) que llama a todo lo
// demas en orden. La logica interna de cada bloque es la misma que ya
// tenias; solo se movio de archivo.
// -------------------------------------------------------------------

extern std::vector<Monstruo> listaMonstruos;

// Le avisa a este archivo que 'generarHorda' existe en otro lado (monstruosdatos.cpp)
extern std::vector<Monstruo> generarHorda(int zonaDeseada);

// NUEVO 1.31: Le avisa que generarValdrame existe en monstruosdatos.cpp
// Es el evento fijo del Arzobispo en Y=60
extern Monstruo generarValdrame();

// -------------------------------------------------------------------
// Funcion auxiliar de zona. Antes esta cadena de if/else vivia repetida
// dentro de generarEnemigo. Se deja aqui, dentro de combate.cpp, sin
// tocar otros archivos que tambien repiten esta logica (loot.cpp,
// main.cpp) — eso queda pendiente para cuando se decida unificarlo en
// un modulo de Zonas.
// -------------------------------------------------------------------
int obtenerZonaPorY(int y) {
    return (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : (y <= 240) ? 4 : 5;
}

// CORRECCIÓN: retorna std::optional<Monstruo> para coincidir con combate.h
std::optional<Monstruo> generarEnemigo(int y) {
    // 1. Determinar zona
    int zona = obtenerZonaPorY(y);

    // Siempre regeneramos la horda de la zona actual con la función que tienes en monstruosdatos.cpp
    listaMonstruos = generarHorda(zona);

    // Centralizamos la aleatoriedad del combate en el RNG único del proyecto.
    bool esElite = Rng::get().probabilidad(25);

    std::vector<Monstruo> posibles;
    for (const auto& m : listaMonstruos) {
        if (m.zona == zona) {
            // Filtro original: si es elite busca raros, si no, normales
            if ((esElite && m.esRaro) || (!esElite && !m.esRaro)) {
                posibles.push_back(m);
            }
        }
    }

    // --- CORRECCIÓN: Si el filtro estricto falló, buscamos CUALQUIERA de la misma zona ---
    if (posibles.empty()) {
        for (const auto& m : listaMonstruos) {
            if (m.zona == zona) {
                posibles.push_back(m);
            }
        }
    }

    // Caso especial: Si es Zona 5 (Lancelot), aseguramos que aparezca
    if (zona == 5 && posibles.empty()) {
        for (const auto& m : listaMonstruos) {
            if (m.zona == 5) posibles.push_back(m);
        }
    }

    if (!posibles.empty()) {
        int index = Rng::get().entre(0, static_cast<int>(posibles.size()) - 1);
        return posibles[static_cast<size_t>(index)];
    }

    // Retorno de seguridad: nullopt si no hay monstruo
    return std::nullopt;
}

// =========================================================
// HUD DE COMBATE
// =========================================================
void mostrarHUDCombate(const Personaje &p, const Monstruo &m, int hpInicial, const string &etiqueta) {
    cout << "====================================================" << endl;
    cout << "  " << p.nombre << " [" << p.hp << "/" << p.hpMax << " HP] | "
         << p.tipoRecurso << ": " << p.recursoActual << "/" << p.recursoMax << endl;
    cout << "  VS  " << etiqueta << m.nombre
         << " (Nvl " << m.nivelEnemigo << ") [" << m.hp << "/" << hpInicial << " HP]" << endl;
    if (!m.naturaleza.empty() && m.naturaleza != "Normal")
        cout << "  Naturaleza: " << m.naturaleza << endl;
    cout << "====================================================" << endl;
}

// =========================================================
// PROCESAMIENTO DE ESTADOS (Veneno, Quemadura, Paralisis, etc.)
//
// Devuelven true si el objetivo debe perder su turno: los efectos
// Paralisis (4), Evasion (5), Congelacion (6) y Desorientacion Arcana
// (7) hacen su roll de probabilidad dentro de Efecto::aplicar() y
// antes ese resultado se descartaba -- ahora se propaga hasta
// iniciarCombate() para que realmente bloquee el turno.
// =========================================================
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

// Igual que arriba, pero ademas contempla el Talisman de la Pureza
// (reliquia 211): si el jugador la porta, Efecto::aplicar() bloquea el
// efecto por completo (ver Efectos.cpp) y aqui solo evitamos duplicar
// el aviso en pantalla con el mensaje generico "[ESTADO EN TI]".
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

// =========================================================
// PROCESAMIENTO DE BUFFS TEMPORALES (elixires)
// Cada buff activo (ver BuffActivo en Personajes.h) se descuenta un
// turno aqui, y al llegar a 0 se resta exactamente el mismo bono que
// se sumo al usarlo, dejando al personaje en su valor original.
// =========================================================
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

// =========================================================
// RESOLUCION DE COMBATE (victoria, derrota, huida, loot)
// =========================================================
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

// =========================================================
// NOTA DE LORE:
// El combate representa el instante en que la calma del explorador
// se rompe: cada turno alterna entre la tactica fria del jugador
// (elegir arma, habilidad, objeto o huida) y el instinto crudo del
// enemigo. Los jefes (Valdrame, Lancelot) rompen ese ritmo a
// proposito con fases — Valdrame pasa de la magia oscura al golpe
// fisico sagrado; Lancelot pasa de la defensa de escamas a la furia
// del Caos — para que el jugador sienta que el combate "cambia de
// forma" bajo sus pies, no solo que el enemigo pega mas fuerte.
// =========================================================

// =========================================================
// ORQUESTADOR PRINCIPAL DEL COMBATE
// Llama, en orden, a cada uno de los bloques de arriba (y a los que
// ahora viven en FasesJefe.cpp, TurnoJugador.cpp y TurnoEnemigo.cpp).
// La logica de que se llama, cuando, y con que condiciones de corte
// es identica a la version anterior — solo que ahora cada paso vive
// en su propio archivo.
// =========================================================
void iniciarCombate(Personaje &p, int y) {
    int turnosAbsorcion = 0; // Controla la absorcion de la Fase 2 de Lancelot (ver turnoJugador)

    // =========================================================
    // Combate Valdrame
    // =========================================================
    // Valdrame solo debe aparecer en la Iglesia (Y = 60) cuando el evento aún no fue completado.
    Monstruo errorMonstruo{"Error", 1, 1, 1, 1, 0, 1, 1, false};
    Monstruo m = (y == 60 && !p.valdrameDerrotado) ? generarValdrame() : generarEnemigo(y).value_or(errorMonstruo);

    int hpInicial = m.hp; // Mantengo tu variable para el HUD y las fases
    bool fase2 = false;
    bool fase3 = false;
    bool huidaExitosa = false; // Variable necesaria para la opción de huir
    bool monstruoHuyo = false; // NUEVO 1.31: Para cuando el enemigo decide escapar

    // NUEVO 1.31: Flag de fase del Arzobispo — empieza en false, se activa al 50% HP
    bool faseValdrame2 = false;

    string etiqueta = m.esJefe ? "[JEFE] " : m.esElite ? "[ELITE] " : m.esRaro ? "[RARO] " : "";
    string tituloCombate = string("COMBATE CONTRA: ") + etiqueta + m.nombre;
    mostrarCabecera(tituloCombate);

    // --- Reacción visual de Naturaleza del Monstruo ---
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

    // Lore de introduccion del Arzobispo al entrar en combate
    if (m.nombre == "ARZOBISPO VALDRAME") {
        cout << "\nUna figura encapuchada bloquea el camino..." << endl;
        cout << "\"El caos de Angra me ha dado un poder que ningun mortal puede comprender.\"" << endl;
        cout << "\"Yo soy el Senor de los Muertos. Tu alma sera mi ofrenda.\"" << endl;
        esperarTecla();
    }

    while (p.hp > 0 && m.hp > 0) {
        // -------------------------------------------------------
        // PARCHE 1: ULTIMATES NIVEL 20 - Tick de estados
        // -------------------------------------------------------
        // Tickear debuff de armadura del enemigo (Ultima Bestia)
        if (m.turnosDebuffDefensa > 0) {
            m.turnosDebuffDefensa--;
            if (m.turnosDebuffDefensa == 0) {
                m.defensa = m.defensaOriginal;
                cout << "[DEBUFF] La armadura de " << m.nombre << " se ha recuperado." << endl;
            }
        }

        // Tickear agotamiento del jugador (Colapso Solar)
        if (p.turnosAgotado > 0) {
            p.turnosAgotado--;
        }
        // -------------------------------------------------------

        limpiarPantalla();

        // HUD de combate
        mostrarHUDCombate(p, m, hpInicial, etiqueta);

        // Procesador de Estados sobre el Enemigo
        // Captura si el enemigo quedo paralizado/congelado/etc, para
        // poder bloquear su turno mas abajo en vez de solo imprimir el mensaje.
        bool enemigoParalizado = procesarEstadosEnemigo(m, hpInicial);

        // Procesar estados sobre el Jugador (mismo caso que arriba)
        bool jugadorParalizado = procesarEstadosJugador(p);

        // Descontar duracion de los buffs de elixires activos del jugador.
        procesarBuffsJugador(p);

        // Reliquias pasivas de fin de ronda (Corazon de Fenix cura al
        // jugador, Calavera de Valdrame drena al enemigo). Ver Efectos.cpp.
        if (m.hp > 0) {
            procesarReliquiasTurno(p.hp, p.hpMax, p.reliquias, m.hp);
        }

        if (m.hp <= 0 || p.hp <= 0) break;

        // Aura de Valdrame + transformaciones de fase (Valdrame y Lancelot)
        manejarFasesJefe(m, p, y, fase2, fase3, faseValdrame2, hpInicial, turnosAbsorcion);

        // --- Turnos ---
        bool esTurnoJugador = (p.velocidadBase >= m.velocidad);

        for (int i = 0; i < 2; i++) {
            if (m.hp <= 0 || p.hp <= 0 || huidaExitosa || monstruoHuyo) break;

            if ((i == 0 && esTurnoJugador) || (i == 1 && !esTurnoJugador)) {
                // Si el jugador quedo paralizado/congelado/desorientado este
                // turno, pierde su accion en vez de poder actuar normalmente.
                if (jugadorParalizado) {
                    cout << "\n[ESTADO] ¡Estas paralizado y no puedes actuar este turno!" << endl;
                    esperarTecla();
                } else {
                    huidaExitosa = turnoJugador(p, m, turnosAbsorcion);
                }
                if (m.hp <= 0) break;
            } else {
                // Mismo bloqueo pero para el enemigo.
                if (enemigoParalizado) {
                    cout << "\n[ESTADO] " << m.nombre << " esta paralizado y no puede actuar!" << endl;
                    esperarTecla();
                } else {
                    monstruoHuyo = turnoEnemigo(p, m, faseValdrame2, fase3);
                }
                if (p.hp <= 0) break;
            }
        } // Fin del for de los 2 turnos

        if (huidaExitosa || monstruoHuyo) break;

        if (m.hp > 0 && p.hp > 0) {
            cout << "\nPresiona ENTER para el siguiente turno...";
            limpiarBuffer(); cin.get();
        }
    } // Fin del while de combate

    // --- Resultado del combate ---
    resolverFinDeCombate(p, m, y, huidaExitosa, monstruoHuyo);
}