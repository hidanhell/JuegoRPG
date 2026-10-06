#include "TurnoEnemigo.h"
#include "HabilidadesMonstruo.h"
#include "Efectos.h"
#include "Recursos.h"
#include "Rng.h"
#include <iostream>
#include <algorithm>

using namespace std;

extern std::vector<Efecto> listaEfectos;

// La espada de Lancelot (armaLancelot, definida en catalogoObjetos.cpp)
extern Arma armaLancelot;

// -------------------------------------------------------------------
// NOTA: declaracion temporal de aplicarEfectoLegendario (definida en
// Efectos.cpp). Se usa aqui para la Fase 3 de Lancelot. Es la misma
// funcion que usa TurnoJugador.cpp para el proc de armas legendarias.
// -------------------------------------------------------------------
extern void aplicarEfectoLegendario(const Efecto &efecto, int &hpObjetivo, int ataqueBase, int &hpAtacante, int hpMaxAtacante, bool &turnoPerdido);

// =========================================================
// TURNO DEL ENEMIGO INTELIGENTE
// =========================================================
// Devuelve true si el monstruo huyo exitosamente.
bool turnoEnemigo(Personaje &p, Monstruo &m, bool faseValdrame2, bool fase3Lancelot) {
    bool usoHabilidad = false;
    bool monstruoHuyo = false;
    const bool buffAtaqueActivoAlInicio = m.turnosBuffAtaque > 0;

    if (!m.esJefe && (m.esRaro || m.esElite) && !m.habilidadesIds.empty()) {
        if (m.nombre == "ARZOBISPO VALDRAME") {
            int idHab = faseValdrame2 ? 402 : 401;
            ejecutarHabilidadMonstruo(m, p, idHab);
            usoHabilidad = true;
        }
        else if (Rng::get().probabilidad(40)) {
            int indexHab = Rng::get().entre(0, static_cast<int>(m.habilidadesIds.size()) - 1);
            int idHab = m.habilidadesIds[static_cast<size_t>(indexHab)];
            bool huyo = ejecutarHabilidadMonstruo(m, p, idHab);
            if (huyo) monstruoHuyo = true;
            usoHabilidad = true;
        }
    }

    if (!monstruoHuyo) {
        // NOTA: la Calavera de Valdrame se movio a procesarReliquiasTurno()
        // (ver Efectos.cpp), como reliquia pasiva de fin de ronda (10%) en
        // vez de vivir aqui con un 5% en el turno del enemigo. Antes ambas
        // versiones coexistian y se hubieran duplicado (15% en total); se
        // elimino esta copia para quedarnos solo con la de fin de ronda.

        if (!usoHabilidad) {
            // -------------------------------------------------------
            // PARCHE 5: Escudo del compañero (Pacto de Sangre)
            // -------------------------------------------------------
            if (p.turnosEscudoCompanero > 0) {
                p.turnosEscudoCompanero--;
                cout << "[COMPANERO] Tu bestia absorbe el golpe de " << m.nombre << "!" << endl;
                // No se aplica dano al jugador este turno
            } else {
                // Calculo de dano normal
                double factorBase = (double)m.ataque / (p.defensaBase + p.artefactoEquipado.defensa);
                int dEnemigo = static_cast<int>((static_cast<double>(p.hpMax) * 0.05) * factorBase);
                dEnemigo = max(1, dEnemigo);

                // NUEVO: Hombreras del Coloso (reliquia 215) -- reduce 10% el
                // dano fisico recibido, solo Guerrero.
                if (p.clase == Clase::Guerrero && p.tieneReliquia("Hombreras del Coloso")) {
                    dEnemigo = max(1, static_cast<int>(static_cast<float>(dEnemigo) * 0.90f));
                }

                // =========================================================
                // NUEVO: CRITICO DE MONSTRUO + Escudo del Destino (reliquia 212)
                // Antes no existia ningun sistema de critico para enemigos, asi
                // que "los enemigos no pueden asestarte criticos" no tenia nada
                // que bloquear. Se agrega un critico base de 10% (x1.5 dano)
                // para el ataque normal del enemigo, con los mismos numeros de
                // referencia que ya usa el critico del jugador (5+bonusCritico,
                // x1.5). Si el jugador porta el Escudo del Destino, la
                // probabilidad se fuerza a 0 antes de tirar el dado.
                //
                // ALCANCE: esto solo cubre el ataque normal del enemigo aqui
                // abajo. Las habilidades especiales de monstruo (Grito de
                // Guerra, Escupitajo Acido, etc. en HabilidadesMonstruo.cpp)
                // no tienen critico propio y por lo tanto tampoco lo bloquea
                // el Escudo -- si se quiere extender ahi, es un cambio aparte.
                // =========================================================
                bool tieneEscudoDestino = p.tieneReliquia("Escudo del Destino");
                int chanceCriticoEnemigo = tieneEscudoDestino ? 0 : 10;
                bool esCriticoEnemigo = Rng::get().probabilidad(chanceCriticoEnemigo);
                if (esCriticoEnemigo) {
                    dEnemigo = static_cast<int>(static_cast<float>(dEnemigo) * 1.5f);
                    cout << "[CRITICO ENEMIGO] ¡" << m.nombre << " encuentra un punto debil!" << endl;
                }

                p.hp -= dEnemigo;
                cout << "[ENE] " << m.nombre << " (Nvl " << m.nivelEnemigo << ") te golpea por " << dEnemigo << " de dano." << endl;

                // -------------------------------------------------------
                // FIX: Generacion de recurso al recibir dano (Ira del Guerrero).
                // Antes esta llamada no existia y "recibioDano" nunca
                // se pasaba en true en ningun lado del proyecto.
                // -------------------------------------------------------
                generarRecursoPorTurno(p, false, true);

                if (p.tieneReliquia("Amuleto de Sangre") && m.hp > 0) {
                    if (Rng::get().probabilidad(20)) {
                        m.hp -= 10;
                        cout << "[RELIQUIA] Amuleto de Sangre contraataca por 10 de dano!" << endl;
                    }
                }

                p.usarPocionAuto();
            } // Fin del else (sin escudo)
        }
    }

    // =========================================================
    // NUEVO: FASE 3 DE LANCELOT — LA ESPADA DEL DIOS DEL CAOS
    // armaLancelot (ID 1003, efectoId 204 "Dios del Caos") estaba
    // definida desde el catalogo pero nunca se conectaba a nada.
    // aplicarEfectoLegendario() solo se activa si quien la porta
    // tiene <=40% HP — y la Fase 3 de Lancelot ya se dispara al 25%,
    // asi que aqui siempre calificara mientras esa fase este activa.
    // Se aplica sin importar si el companero absorbio el golpe fisico,
    // porque representa a la espada actuando por su cuenta, poseida
    // por la sombra de Angra — no al golpe normal de Lancelot.
    // =========================================================
    if (m.esJefe && fase3Lancelot && p.hp > 0) {
        auto efLeg = obtenerEfectoPorId(armaLancelot.efectoId);
        if (efLeg) {
            bool turnoPerdido = false;
            cout << "\n[ARMA LEGENDARIA] La Espada del Dios del Caos vibra con energia corrupta..." << endl;
            aplicarEfectoLegendario(*efLeg, p.hp, m.ataque, m.hp, m.hpMax, turnoPerdido);

            if (turnoPerdido) {
                // Se aplica como Paralisis normal (ID 4) para que se procese
                // igual que cualquier otro estado, en vez de inventar un
                // mecanismo nuevo de "perder turno" que no existe hoy.
                for (auto &e : listaEfectos) {
                    if (e.id == 4) {
                        agregarEfectoActivo(p.efectos, e);
                        cout << "[PELIGRO] El Caos te paraliza momentaneamente!" << endl;
                        break;
                    }
                }
            }
        }
    }

    if (buffAtaqueActivoAlInicio && m.turnosBuffAtaque > 0) {
        m.turnosBuffAtaque--;
        if (m.turnosBuffAtaque == 0) {
            m.ataque -= m.bonusAtaqueBuff;
            m.bonusAtaqueBuff = 0;
            cout << "[BUFF] El ataque de " << m.nombre << " vuelve a la normalidad." << endl;
        }
    }

    return monstruoHuyo;
}