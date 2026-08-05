#include "combate.h"
#include "Utilidades.h"
#include "loot.h"
#include "HabilidadesMonstruo.h" 
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include "catalogoObjetos.h"
#include "Consumibles.h"
#include "Habilidades.h"
#include "Rng.h"
#include "Recursos.h"

using namespace std;

/// =========================================================
// BLOQUE M1: ENEMIGOS Y COMBATE DETALLADO 1.4
// =========================================================
// -------------------------------------------------------------------
// NOTA DE ARQUITECTURA (agregado en esta revision):
// Este archivo se reorganizo en funciones mas pequenas para que cada
// parte del combate (HUD, estados, fases de jefe, turno del jugador,
// turno del enemigo, resolucion final) se pueda leer y tocar por
// separado. La logica interna de cada bloque es la misma que ya
// tenias; solo se movio a su propia funcion con nombre.
// -------------------------------------------------------------------

extern std::vector<Monstruo> listaMonstruos;
extern std::vector<Efecto> listaEfectos;
extern std::vector<Efecto> listaEfectosLegendarios;

// -------------------------------------------------------------------
// NUEVO: declaracion temporal de aplicarEfectoLegendario (definida en
// efectos.cpp). Hoy no esta expuesta en Efectos.h, asi que se declara
// aqui mismo para poder usarla en la Fase 3 de Lancelot. Cuando le
// toque su turno a Efectos.h en la revision, lo correcto seria mover
// esta declaracion ahi de forma permanente.
// -------------------------------------------------------------------
extern void aplicarEfectoLegendario(const Efecto &efecto, int &hpObjetivo, int ataqueBase,int &hpAtacante, int hpMaxAtacante, bool &turnoPerdido);

// La espada de Lancelot (armaLancelot, definida en catalogoObjetos.cpp)
extern Arma armaLancelot;

// Le avisa a este archivo que 'generarHorda' existe en otro lado (monstruosdatos.cpp)
extern std::vector<Monstruo> generarHorda(int zonaDeseada);

// NUEVO 1.31: Le avisa que generarValdrame existe en monstruosdatos.cpp
// Es el evento fijo del Arzobispo en Y=60
extern Monstruo generarValdrame();

// -------------------------------------------------------------------
// NUEVO (esta revision): funcion auxiliar de zona.
// Antes esta cadena de if/else vivia repetida dentro de generarEnemigo.
// Se deja aqui, dentro de combate.cpp, sin tocar otros archivos que
// tambien repiten esta logica (loot.cpp, main.cpp) — eso queda
// pendiente para cuando se decida unificarlo en un modulo de Zonas.
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
// (extraido del bloque que imprimia el encabezado cada turno)
// =========================================================
void mostrarHUDCombate(const Personaje &p, const Monstruo &m, int hpInicial, const string &etiqueta) {
    cout << "====================================================" << endl;
    // NUEVO: Agregamos la visualización del recurso actual
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
// Logica intacta, solo separada en sus propias funciones.
// =========================================================
void procesarEstadosEnemigo(Monstruo &m, int hpInicial) {
    for (auto it = m.efectosActivos.begin(); it != m.efectosActivos.end(); ) {
        bool saltarTurno = false;
        it->aplicar(m.hp, hpInicial, saltarTurno);
        cout << "[ESTADO EN ENEMIGO] " << it->nombre << " afecta a " << m.nombre << "." << endl;
        it->duracion--;
        if (it->duracion <= 0) {
            cout << "[INFO] El estado " << it->nombre << " ha expirado." << endl;
            it = m.efectosActivos.erase(it);
        } else {
            ++it;
        }
    }
}

void procesarEstadosJugador(Personaje &p) {
    for (auto it = p.efectos.begin(); it != p.efectos.end(); ) {
        bool saltarTurno = false;
        it->aplicar(p.hp, p.hpMax, saltarTurno);
        cout << "[ESTADO EN TI] " << it->nombre << " te esta afectando." << endl;
        it->duracion--;
        if (it->duracion <= 0) {
            cout << "[INFO] Tu estado de " << it->nombre << " ha terminado." << endl;
            it = p.efectos.erase(it);
        } else {
            ++it;
        }
    }
}

// =========================================================
// FASES DE JEFE: Aura de Valdrame + transformaciones + fases de Lancelot
// Se agrupan aqui porque las tres cosas dependen de "quien es el
// jefe" (m.nombre / y) y de en que punto de HP esta. Logica identica
// a la que tenias, solo con nombre propio.
// =========================================================
void manejarFasesJefe(Monstruo &m, Personaje &p, int y,
                       bool &fase2, bool &fase3, bool &faseValdrame2,
                       int hpInicial, int &turnosAbsorcion) {

    // =========================================================
    // EL AURA DEL SEÑOR DE LOS MUERTOS (VALDRAME)
    // =========================================================
    if (m.nombre == "ARZOBISPO VALDRAME" && p.hp > 0) {
        int auraDrenado = max(1, static_cast<int>(static_cast<float>(p.hp) * 0.05f));
        p.hp -= auraDrenado;
        m.hp = min(hpInicial, m.hp + auraDrenado);
        cout << "\n[AMBIENTE] La presencia de Valdrame esta drenando tu energia..." << endl;
        cout << "[BOSS] El Senor de los Muertos absorbe " << auraDrenado << " HP de tu fuerza vital." << endl;
    }

    // =========================================================
    // NUEVO 1.31: LOGICA DE FASES — ARZOBISPO VALDRAME
    // =========================================================
    if (m.nombre == "ARZOBISPO VALDRAME" && !faseValdrame2 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.5f)) {
        faseValdrame2 = true;
        m.ataque = static_cast<int>(static_cast<float>(m.fuerza) * 2.0f);
        cout << "\n[TRANSFORMACION] El Arzobispo grita: 'EL PUNIO SANTO ME GUIA!'" << endl;
        cout << "[MECANICA] Valdrame abandona la magia oscura y desata su poder fisico sagrado." << endl;
        cout << "[PELIGRO] Sus proximos golpes seran devastadores. Preparate." << endl;
        system("pause");
    }

    //Combate vs Lancelot el Rey Dragon
    // --- FASE 2: EL ESCUDO DEL REY (Activa al 50% de HP) ---
    if (y >= 241 && !fase2 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.5f)) {
        fase2 = true;
        m.defensa = static_cast<int>(static_cast<float>(m.defensa) * 1.30f);
        turnosAbsorcion = 2;
        cout << "\n  LANCELOT CAMBIA SUS ESCAMAS  Defensa aumentada." << endl;
        cout << "[MECANICA] Las escamas del Rey brillan con la magia corrupta de Angra." << endl;
        cout << "[PELIGRO] --Lancelot absorbera el dano de tus proximos--" << turnosAbsorcion << " ataques y lo convertira en vida!" << endl;
        system("pause");
    }

    // --- FASE 3: LA FURIA DE ANGRA (Activa al 25% de HP) ---
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
        system("pause");
    }
}

// =========================================================
// TURNO DEL JUGADOR — Menu tactico completo
// =========================================================
bool turnoJugador(Personaje &p, Monstruo &m, int &turnosAbsorcion) {
    bool huidaExitosa = false;
    bool accionValida = false;

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
            int danoReal = max(2, dTotal - m.defensa);

            if (esCritico) {
                danoReal = static_cast<int>(static_cast<float>(danoReal) * 1.5f);
                cout << "[CRITICO] Golpe devastador" << endl;
                if (p.armaEquipada.rareza == Rareza::Elite) {
                    aplicarGritoDelCaido(m, p);
                }
                if (p.tieneReliquia("Colmillo de Vampiro")) {
                    int roboVida = static_cast<int>(static_cast<float>(danoReal) * 0.15f);
                    p.hp = min(p.hpMax, p.hp + roboVida);
                    cout << "[RELIQUIA] Recuperas " << roboVida << " HP." << endl;
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

            // --- Artefacto: Efecto de estado del arma (solo si el enemigo sigue vivo) ---
            if (p.armaEquipada.efectoId > 0 && m.hp > 0) {
                if (Rng::get().probabilidad(30)) {
                    auto efOpt = obtenerEfectoPorId(p.armaEquipada.efectoId);
                    if (efOpt && efOpt->id != 0) {
                        m.efectosActivos.push_back(*efOpt);
                        cout << "[ESTADO] Aplicaste " << efOpt->nombre << "." << endl;
                    }
                }
            }

            (void)absorbidoPorEscamas;

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
            // Trackeo inactivo
            // -------------------------------------------------------
            p.ultimoTurnoAtaco = false;

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
                                    // FIX: la ultimate consume TODO el recurso actual,
                                    // no un costo fijo. Antes era gastarRecurso(p, 50).
                                    int costoTotal = p.recursoActual;
                                    if (gastarRecurso(p, costoTotal)) {
                                        ejecutarUltimate(p, m, idSeleccionado);
                                        accionValida = true;
                                    }
                                } else {
                                    cout << "[!] Ya has usado tu Ultimate en esta partida o no cumples los requisitos." << endl;
                                }
                            } else {
                                // Habilidad activa normal (Costo estándar de recurso: ej. 25)
                                int costoRecurso = 25;
                                if (gastarRecurso(p, costoRecurso)) {
                                    ejecutarHabilidad(p, m, idSeleccionado);
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
                system("cls");
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
                    if (obj.buffAtaque > 0) {
                        p.fuerza += obj.buffAtaque;
                        cout << "[!] Fuerza aumentada a: " << p.fuerza << endl;
                    }
                    if (obj.buffDefensa > 0) {
                        p.vitalidad += (obj.buffDefensa / 2);
                        cout << "[!] Vitalidad aumentada a: " << p.vitalidad << endl;
                    }
                    if (obj.buffVelocidad > 0) {
                        p.destreza += obj.buffVelocidad;
                        cout << "[!] Destreza aumentada a: " << p.destreza << endl;
                    }

                    p.inventario.erase(p.inventario.begin() + (selInv - 1));
                    enInventario = false;
                    usoObjeto = true;
                    system("pause");
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
                system("pause");
            } else {
                cout << "[HUIDA] ¡Fallaste al escapar! " << m.nombre << " te bloquea el paso." << endl;
                system("pause");
            }
            accionValida = true;
        }
    }

    return huidaExitosa;
}

// =========================================================
// TURNO DEL ENEMIGO INTELIGENTE
// =========================================================
// Devuelve true si el monstruo huyo exitosamente.
bool turnoEnemigo(Personaje &p, Monstruo &m, bool faseValdrame2, bool fase3Lancelot) {
    bool usoHabilidad = false;
    bool monstruoHuyo = false;

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
        if (p.tieneReliquia("Calavera de Valdrame") && m.hp > 0) {
            int absorcion = max(1, static_cast<int>(static_cast<float>(m.hp) * 0.05f));
            m.hp -= absorcion;
            p.hp = min(p.hpMax, p.hp + absorcion);
            cout << "[RELIQUIA] Calavera de Valdrame: ¡Absorbes " << absorcion << " HP de " << m.nombre << "!" << endl;
        }

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
                        p.efectos.push_back(e);
                        cout << "[PELIGRO] El Caos te paraliza momentaneamente!" << endl;
                        break;
                    }
                }
            }
        }
    }

    return monstruoHuyo;
}

// =========================================================
// RESOLUCION DE COMBATE (victoria, derrota, huida, loot)
// =========================================================
void resolverFinDeCombate(Personaje &p, Monstruo &m, int y, bool huidaExitosa, bool monstruoHuyo) {
    if (huidaExitosa) return;

    if (monstruoHuyo) {
        cout << "\n[INFO] " << m.nombre << " ha huido cobardemente. No ganas recompensa." << endl;
        system("pause");
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
        system("pause");
    } else if (m.hp <= 0 && y >= 241) {
        cout << "\n--LANCELOT HA CAIDO-- El cielo se aclara por primera vez en decadas." << endl;
        cout << "Entre las cenizas recoges la ESCAMA DEL REY DRAGON, simbolo eterno de tu victoria." << endl;
        system("pause");
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
// Llama, en orden, a cada uno de los bloques de arriba. La logica de
// que se llama, cuando, y con que condiciones de corte es identica a
// la version anterior — solo que ahora cada paso tiene nombre propio.
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

    // --- NUEVO: Reacción visual de Naturaleza del Monstruo ---
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

    // NUEVO 1.31: Lore de introduccion del Arzobispo al entrar en combate
    if (m.nombre == "ARZOBISPO VALDRAME") {
        cout << "\nUna figura encapuchada bloquea el camino..." << endl;
        cout << "\"El caos de Angra me ha dado un poder que ningun mortal puede comprender.\"" << endl;
        cout << "\"Yo soy el Senor de los Muertos. Tu alma sera mi ofrenda.\"" << endl;
        system("pause");
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

        system("cls");

        // HUD de combate
        mostrarHUDCombate(p, m, hpInicial, etiqueta);

        // Procesador de Estados sobre el Enemigo (Lógica intacta)
        procesarEstadosEnemigo(m, hpInicial);

        // Procesar estados sobre el Jugador (Lógica intacta)
        procesarEstadosJugador(p);

        if (m.hp <= 0 || p.hp <= 0) break;

        // Aura de Valdrame + transformaciones de fase (Valdrame y Lancelot)
        manejarFasesJefe(m, p, y, fase2, fase3, faseValdrame2, hpInicial, turnosAbsorcion);

        // --- Turnos ---
        bool esTurnoJugador = (p.velocidadBase >= m.velocidad);

        for (int i = 0; i < 2; i++) {
            if (m.hp <= 0 || p.hp <= 0 || huidaExitosa || monstruoHuyo) break;

            if ((i == 0 && esTurnoJugador) || (i == 1 && !esTurnoJugador)) {
                huidaExitosa = turnoJugador(p, m, turnosAbsorcion);
                if (m.hp <= 0) break;
            } else {
                monstruoHuyo = turnoEnemigo(p, m, faseValdrame2, fase3);
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