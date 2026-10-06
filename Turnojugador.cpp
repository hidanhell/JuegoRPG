#include "TurnoJugador.h"
#include "Utilidades.h"
#include "Efectos.h"
#include "Habilidades.h"
#include "Consumibles.h"
#include "Rng.h"
#include "Recursos.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// -------------------------------------------------------------------
// NOTA: declaracion temporal de aplicarEfectoLegendario (definida en
// Efectos.cpp). Se usa aqui para procesar los efectos de las armas
// legendarias (Excalibur/Baculo/Arco, IDs 201-203) cuando proc-ean en
// el ataque basico del jugador. Es la misma funcion que ya usa
// TurnoEnemigo.cpp para la Fase 3 de Lancelot.
// -------------------------------------------------------------------
extern void aplicarEfectoLegendario(const Efecto &efecto, int &hpObjetivo, int ataqueBase, int &hpAtacante, int hpMaxAtacante, bool &turnoPerdido);

// =========================================================
// TURNO DEL JUGADOR — Menu tactico completo
// =========================================================
bool turnoJugador(Personaje &p, Monstruo &m, int &turnosAbsorcion) {
    bool huidaExitosa = false;
    bool accionValida = false;
    const int agotamientoAlInicio = p.turnosAgotado;

    while (!accionValida && m.hp > 0 && p.hp > 0) {
        cout << "\n--- TU TURNO ---" << endl;

        // -------------------------------------------------------
        // Menú con alerta de agotamiento
        // -------------------------------------------------------
        cout << "1. " << p.nombreAtaque;
        if (p.turnosAgotado > 0)
            cout << " | 2. Habilidades (AGOTADO " << p.turnosAgotado << "T)";
        else
            cout << " | 2. Habilidades";
        cout << " | 3. Inventario | 4. Huir" << endl;
        // -------------------------------------------------------
        cout << "Seleccion: ";
        int opc;
        if (!(cin >> opc)) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "[!] Entrada invalida. Escribe un numero del 1 al 4." << endl;
            continue; // Vuelve a mostrar el menu, no consume turno
        }

        if (opc == 1) { // ATAQUE BÁSICO
            int dTotal = p.ataqueBase + p.armaEquipada.poder;
            bool esCritico = Rng::get().probabilidad(5 + p.bonusCritico);

            // NUEVO: Carcaj Inagotable (reliquia 217) -- los ataques basicos
            // del Cazador ignoran 20% de la defensa enemiga. Solo afecta el
            // calculo del ataque basico, no habilidades ni ultimates.
            int defensaEfectiva = m.defensa;
            if (p.clase == Clase::Cazador && p.tieneReliquia("Carcaj Inagotable")) {
                defensaEfectiva = static_cast<int>(static_cast<float>(defensaEfectiva) * 0.80f);
            }
            int danoReal = max(2, dTotal - defensaEfectiva);

            if (esCritico) {
                danoReal = static_cast<int>(static_cast<float>(danoReal) * 1.5f);
                cout << "[CRITICO] Golpe devastador" << endl;
                if (p.armaEquipada.rareza == Rareza::Elite) {
                    aplicarGritoDelCaido(m, p);
                }
            }

            // -------------------------------------------------------
            // Escamas del Rey (absorcion de dano)
            // -------------------------------------------------------
            bool absorbidoPorEscamas = false;
            if (turnosAbsorcion > 0) {
                turnosAbsorcion--;
                absorbidoPorEscamas = true;
                m.hp = min(m.hpMax, m.hp + danoReal);
                cout << "[ESCAMAS DEL REY] Lancelot absorbe tu golpe y lo convierte en "
                     << danoReal << " HP para el!" << endl;
                if (turnosAbsorcion > 0)
                    cout << "[PELIGRO] Le quedan " << turnosAbsorcion << " ataque(s) mas de absorcion." << endl;
                else
                    cout << "[INFO] Las escamas de Lancelot se han estabilizado. Tu proximo golpe sera normal." << endl;
            } else {
                m.hp -= danoReal;
                cout << "[TI] " << p.nombreAtaque << " causa " << danoReal << " de dano." << endl;
            }

            if (!absorbidoPorEscamas && p.tieneReliquia("Colmillo de Vampiro")) {
                int roboVida = static_cast<int>(static_cast<float>(danoReal) * 0.15f);
                p.hp = min(p.hpMax, p.hp + roboVida);
                cout << "[RELIQUIA] Recuperas " << roboVida << " HP." << endl;
            }

            // --- Artefacto: Efecto de estado del arma (solo si el enemigo sigue vivo) ---
            if (p.armaEquipada.efectoId > 0 && m.hp > 0) {
                if (Rng::get().probabilidad(30)) {
                    auto efOpt = obtenerEfectoPorId(p.armaEquipada.efectoId);
                    if (efOpt && efOpt->id != 0) {
                        // FIX: los efectos legendarios (201-203, armas Excalibur/Baculo/Arco)
                        // no se pueden procesar como un estado comun via Efecto::aplicar(),
                        // porque su logica (dano segun ataque + autocuracion) vive solo en
                        // aplicarEfectoLegendario() -- la misma funcion que ya usa Lancelot.
                        // Antes se empujaban a m.efectosActivos y Efecto::aplicar() los
                        // ignoraba en el 'default' (case no definido = no pasa nada), asi que
                        // las armas legendarias del jugador nunca tenian efecto real en combate.
                        if (efOpt->id >= 201 && efOpt->id <= 203) {
                            bool turnoPerdidoLeg = false;
                            aplicarEfectoLegendario(*efOpt, m.hp, dTotal, p.hp, p.hpMax, turnoPerdidoLeg);
                        } else {
                            agregarEfectoActivo(m.efectosActivos, *efOpt);
                            cout << "[ESTADO] Aplicaste " << efOpt->nombre << "." << endl;
                        }
                    }
                }
            }

            // NUEVO: Gema del Eco (reliquia 216) -- 15% de probabilidad de un
            // segundo impacto para el Mago. Simplificado a proposito: el
            // segundo golpe es dano plano (mismo calculo base, sin volver a
            // tirar critico ni proc de arma) para no encadenar dos robos de
            // vida / paralisis por el mismo click. Se omite si el golpe fue
            // absorbido por las Escamas del Rey, para no curar gratis a Lancelot.
            if (p.clase == Clase::Mago && p.tieneReliquia("Gema del Eco") && !absorbidoPorEscamas && m.hp > 0) {
                if (Rng::get().probabilidad(15)) {
                    int danoEco = max(2, dTotal - m.defensa);
                    m.hp -= danoEco;
                    cout << "[RELIQUIA] Gema del Eco resuena y repite el hechizo por " << danoEco << " de dano adicional!" << endl;
                }
            }

            // -------------------------------------------------------
            // Generación de recurso por ataque básico (Ira / Enfoque)
            // -------------------------------------------------------
            generarRecursoPorTurno(p, true, false);

            // -------------------------------------------------------
            // (Flecha del Juicio Final)
            // -------------------------------------------------------
            p.ultimoTurnoAtaco = true;

            accionValida = true; // El ataque siempre consume el turno
        }
        else if (opc == 2) {
            // -------------------------------------------------------
            // Bloqueo de habilidades por agotamiento
            // -------------------------------------------------------
            if (p.turnosAgotado > 0) {
                cout << "[!] Estas agotado por Colapso Solar. No puedes usar habilidades este turno." << endl;
                // No consume turno: el menu se vuelve a mostrar
            } else {
                // --- MENÚ DE HABILIDADES ACTIVAS Y ULTIMATES ---
                cout << "\n=== HABILIDADES ===" << endl;
                vector<int> activas;
                for (int id : p.habilidadesIds)
                {
                    auto hOpt = obtenerHabilidadPorId(id);
                    if (!hOpt) continue;
                    Habilidad h = *hOpt;
                    if (h.tipo == "Activa" || h.tipo == "Ultimate")
                    {
                        activas.push_back(id);
                        if (h.tipo == "Ultimate")
                            cout << activas.size() << ". *** " << h.nombre << " *** - " << h.descripcion << endl;
                        else
                            cout << activas.size() << ". " << h.nombre << " - " << h.descripcion << endl;
                    }
                }

                if (activas.empty()) {
                    cout << "[!] No tienes habilidades activas disponibles." << endl;
                } else {
                    cout << "Seleccion: ";
                    int sel;
                    if (!(cin >> sel)) {
                        cin.clear();
                        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        cout << "[!] Entrada invalida." << endl;
                    }
                    else if (sel > 0 && sel <= (int)activas.size()) {
                        int idSeleccionado = activas[static_cast<size_t>(sel - 1)];
                        auto hOpt = obtenerHabilidadPorId(idSeleccionado);
                        if (hOpt) {
                            // Validamos si es una Ultimate (IDs 300-399) o Activa normal (IDs 100-199)
                            if (hOpt->tipo == "Ultimate") {
                                if (puedeUsarUltimate(p)) {
                                    // La Ultimate requiere la barra llena y consume todo el recurso.
                                    int costoTotal = p.recursoActual;
                                    if (gastarRecurso(p, costoTotal)) {
                                        ejecutarUltimate(p, m, idSeleccionado);
                                        p.ultimoTurnoAtaco = false;
                                        accionValida = true;
                                    }
                                } else {
                                    if (p.recursoActual < p.recursoMax) {
                                        cout << "[!] Necesitas llenar tu recurso de clase para usar la Ultimate." << endl;
                                    } else if (idSeleccionado == 331 && !p.ultimoTurnoAtaco) {
                                        cout << "[!] Debes usar un ataque basico el turno anterior para cargar esta Ultimate." << endl;
                                    } else {
                                        cout << "[!] Ya has usado tu Ultimate o no cumples los requisitos." << endl;
                                    }
                                }
                            } else {
                                // Habilidad activa normal (Costo estándar de recurso: ej. 25)
                                int costoRecurso = 25;
                                if (gastarRecurso(p, costoRecurso)) {
                                    ejecutarHabilidad(p, m, idSeleccionado);
                                    p.ultimoTurnoAtaco = false;
                                    accionValida = true;
                                }
                            }
                        }
                    } else {
                        cout << "[-] Seleccion invalida." << endl;
                    }
                }
            } // Cierre del else de agotamiento
        }
        else if (opc == 3) { // MENÚ DE INVENTARIO
            p.ultimoTurnoAtaco = false;
            bool enInventario = true;
            bool usoObjeto = false;

            while (enInventario) {
                limpiarPantalla();
                cout << "=== INVENTARIO DE CONSUMIBLES (" << p.inventario.size() << "/10) ===" << endl;

                if (p.inventario.empty()) {
                    cout << "\n[-] No tienes objetos en el inventario." << endl;
                    cout << "1. Volver" << endl;
                } else {
                    for (size_t j = 0; j < p.inventario.size(); ++j) {
                        cout << j + 1 << ". " << p.inventario[j].nombre
                             << " - " << p.inventario[j].descripcion << endl;
                    }
                    cout << p.inventario.size() + 1 << ". Volver" << endl;
                }

                cout << "\nSeleccion: ";
                int selInv;
                if (!(cin >> selInv)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

                if (selInv == (int)p.inventario.size() + 1 || p.inventario.empty()) {
                    enInventario = false;
                }
                else if (selInv > 0 && selInv <= (int)p.inventario.size()) {
                    Consumible& obj = p.inventario[static_cast<size_t>(selInv - 1)];

                    if (obj.curacion > 0) {
                        p.hp = std::min(p.hpMax, p.hp + obj.curacion);
                        cout << "\n[-] Usaste " << obj.nombre << ". Recuperas " << obj.curacion << " HP." << endl;
                    }

                    // FIX: antes buffAtaque/buffDefensa/buffVelocidad sumaban siempre a
                    // fuerza/vitalidad/destreza sin importar la clase, y ademas nunca
                    // expiraban. Ahora se suma directo al stat de combate correspondiente
                    // (ataqueBase/defensaBase/velocidadBase, que es lo que las formulas de
                    // combate realmente usan) Y se registra en p.buffsActivos para que
                    // procesarBuffsJugador() lo revierta al terminar su duracion.
                    if (obj.buffAtaque > 0 || obj.buffDefensa > 0 || obj.buffVelocidad > 0) {
                        p.ataqueBase    += obj.buffAtaque;
                        p.defensaBase   += obj.buffDefensa;
                        p.velocidadBase += obj.buffVelocidad;

                        BuffActivo buff;
                        buff.nombre = obj.nombre;
                        buff.bonusAtaque = obj.buffAtaque;
                        buff.bonusDefensa = obj.buffDefensa;
                        buff.bonusVelocidad = obj.buffVelocidad;
                        buff.duracion = std::max(1, obj.duracion); // minimo 1 turno de seguridad
                        p.buffsActivos.push_back(buff);

                        cout << "[!] " << obj.nombre << " activo por " << buff.duracion << " turno(s). "
                             << "ATQ:" << p.ataqueBase << " DEF:" << p.defensaBase << " VEL:" << p.velocidadBase << endl;
                    }

                    // Antidoto -- limpia todos los efectos de estado activos del jugador.
                    if (obj.curaEstados) {
                        if (!p.efectos.empty()) {
                            p.efectos.clear();
                            cout << "[ANTIDOTO] Todos tus efectos de estado han sido eliminados." << endl;
                        } else {
                            cout << "[ANTIDOTO] No tenias ningun efecto de estado activo." << endl;
                        }
                    }

                    // Tonico de Voluntad -- restaura recurso de clase de forma instantanea.
                    if (obj.recursoRestaurado > 0) {
                        p.recursoActual = std::min(p.recursoMax, p.recursoActual + obj.recursoRestaurado);
                        cout << "[RECURSO] Recuperas " << obj.recursoRestaurado << " de " << p.tipoRecurso
                             << ". (Actual: " << p.recursoActual << "/" << p.recursoMax << ")" << endl;
                    }

                    // Bombas de alquimista -- dano directo al enemigo + efecto garantizado.
                    if (obj.danoDirecto > 0) {
                        m.hp -= obj.danoDirecto;
                        if (m.hp < 0) m.hp = 0;
                        cout << "[BOMBA] " << obj.nombre << " inflige " << obj.danoDirecto << " de dano directo a " << m.nombre << "." << endl;

                        if (obj.efectoAlUsar > 0 && m.hp > 0) {
                            auto efOpt = obtenerEfectoPorId(obj.efectoAlUsar);
                            if (efOpt) {
                                agregarEfectoActivo(m.efectosActivos, *efOpt);
                                cout << "[BOMBA] " << m.nombre << " queda afectado por " << efOpt->nombre << "." << endl;
                            }
                        }
                    }

                    // Amuleto de Huida -- escape garantizado, sin tirar probabilidad.
                    if (obj.huidaGarantizada) {
                        huidaExitosa = true;
                        cout << "[AMULETO] " << obj.nombre << " te envuelve en un destello y escapas del combate al instante!" << endl;
                    }

                    p.inventario.erase(p.inventario.begin() + (selInv - 1));
                    enInventario = false;
                    usoObjeto = true;
                    esperarTecla();
                }
            }
            if (usoObjeto) accionValida = true;
        }
        else if (opc == 4) { // HUIR
            p.ultimoTurnoAtaco = false;
            int chanceHuida = 40 + ((p.velocidadBase - m.velocidad) * 3);
            chanceHuida = max(10, min(chanceHuida, 90));

            cout << "\n[HUIDA] Intentas escapar... (Probabilidad: " << chanceHuida << "%)" << endl;
            if (Rng::get().probabilidad(chanceHuida)) {
                cout << "[HUIDA] ¡Lograste escapar!" << endl;
                huidaExitosa = true;
                esperarTecla();
            } else {
                cout << "[HUIDA] ¡Fallaste al escapar! " << m.nombre << " te bloquea el paso." << endl;
                esperarTecla();
            }
            accionValida = true;
        }
    }

    if (accionValida && agotamientoAlInicio > 0 && p.turnosAgotado > 0) {
        p.turnosAgotado--;
    }
    return huidaExitosa;
}