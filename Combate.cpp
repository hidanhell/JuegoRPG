#include "combate.h"
#include "Utilidades.h"
#include "loot.h"
#include "HabilidadesMonstruo.h" // NUEVO 1.31: Módulo de habilidades de enemigos
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include "Personajes.h"
#include "Monstruos.h"
#include "Efectos.h"
#include "catalogoObjetos.h"
#include "Consumibles.h"
#include "Habilidades.h"
using namespace std;

/// =========================================================
// BLOQUE M1: ENEMIGOS Y COMBATE DETALLADO 1.31
// =========================================================

extern std::vector<Monstruo> listaMonstruos; 
extern std::vector<Efecto> listaEfectos;
extern std::vector<Efecto> listaEfectosLegendarios;

// Le avisa a este archivo que 'generarHorda' existe en otro lado (monstruosdatos.cpp)
extern std::vector<Monstruo> generarHorda(int zonaDeseada); 

// NUEVO 1.31: Le avisa que generarValdrame existe en monstruosdatos.cpp
// Es el evento fijo del Arzobispo en Y=60
extern Monstruo generarValdrame();

// Función auxiliar para encontrar los datos de un efecto por su ID
Efecto obtenerEfectoPorId(int id) {
    for (const auto& e : listaEfectos) {
        if (e.id == id) return e;
    }
    for (const auto& e : listaEfectosLegendarios) {
        if (e.id == id) return e;
    }
    return Efecto(); 
}

Monstruo generarEnemigo(int y) {
    // 1. Determinar zona
    int zona = (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : (y <= 240) ? 4 : 5;

    // Siempre regeneramos la horda de la zona actual con la función que tienes en monstruosdatos.cpp
    listaMonstruos = generarHorda(zona);

    bool esElite = (rand() % 100 < 25); 

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
        return posibles[static_cast<size_t>(rand()) % posibles.size()];
    }

    // Retorno de seguridad
    return Monstruo{"Error", 1, 1, 1, 1, 0, 1, 1, false};
}

void iniciarCombate(Personaje &p, int y) {
    int turnosAbsorcion = 0; // Nueva variable para controlar la absorción de Lancelot
    
    // =========================================================
    // Combate Valdrame
    // =========================================================
    Monstruo m = (y >= 60 && y <= 62) ? generarValdrame() : generarEnemigo(y);
    
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
        cout << "====================================================" << endl;
        cout << " " << p.nombre << " [" << p.hp << "/" << p.hpMax << " HP]  VS  " 
             << etiqueta << m.nombre
             << " (Nvl " << m.nivelEnemigo << ") [" << m.hp << "/" << hpInicial << " HP]" << endl;
        if (!m.naturaleza.empty() && m.naturaleza != "Normal")
            cout << " Naturaleza: " << m.naturaleza << endl;
        cout << "====================================================" << endl;

        // Procesador de Estados sobre el Enemigo (Lógica intacta)
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

        // Procesar estados sobre el Jugador (Lógica intacta)
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

        if (m.hp <= 0 || p.hp <= 0) break;

        // =========================================================
        // NUEVO: EL AURA DEL SEÑOR DE LOS MUERTOS (VALDRAME)
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
            
            cout << "\n LANCELOT CAMBIA SUS ESCAMAS  Defensa aumentada." << endl;
            cout << "[MECANICA] Las escamas del Rey brillan con la magia corrupta de Angra." << endl;
            cout << "[PELIGRO] --Lancelot absorbera el dano de tus proximos--" << turnosAbsorcion << " ataques y lo convertira en vida!" << endl;
            system("pause");
        }

        // --- FASE 3: LA FURIA DE ANGRA (Activa al 25% de HP) ---
        if (y >= 241 && !fase3 && m.hp <= static_cast<int>(static_cast<float>(hpInicial) * 0.25f)) {
            fase3 = true;
            m.ataque   = static_cast<int>(static_cast<float>(m.ataque)   * 1.30f);
            m.velocidad = static_cast<int>(static_cast<float>(m.velocidad) * 1.30f);
            
            cout << "\n LANCELOT RUGE CON FURIA  Ataque y Velocidad aumentados." << endl;
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

        // --- Turnos ---
        bool turnoJugador = (p.velocidadBase >= m.velocidad);
        
        for (int i = 0; i < 2; i++) {
            if (m.hp <= 0 || p.hp <= 0 || huidaExitosa || monstruoHuyo) break;

            if ((i == 0 && turnoJugador) || (i == 1 && !turnoJugador)) {
                // --- Turno del Jugador (Menú Táctico) ---
                cout << "\n--- TU TURNO ---" << endl;
                
                // -------------------------------------------------------
                // PARCHE 2: Menú con alerta de agotamiento
                // -------------------------------------------------------
                cout << "1. " << p.nombreAtaque;
                if (p.turnosAgotado > 0)
                    cout << " | 2. Habilidades (AGOTADO " << p.turnosAgotado << "T)";
                else
                    cout << " | 2. Habilidades";
                cout << " | 3. Inventario | 4. Huir" << endl;
                // -------------------------------------------------------
                
                cout << "Seleccion: ";
                int opc; cin >> opc;

                if (opc == 1) { // ATAQUE BÁSICO
                    int dTotal = p.ataqueBase + p.armaEquipada.poder;
                    bool esCritico = (rand() % 100 < (5 + p.bonusCritico));
                    int danoReal = max(2, dTotal - m.defensa);

                    if (esCritico) {
                        danoReal = static_cast<int>(static_cast<float>(danoReal) * 1.5f);
                        cout << "[CRITICO] Golpe devastador" << endl;

                        if (p.armaEquipada.rareza == "Elite")
                                aplicarGritoDelCaido(m, p);
                    }

                    if (p.tieneReliquia("Colmillo de Vampiro")) {
                        int roboVida = static_cast<int>(static_cast<float>(danoReal) * 0.15f);
                        p.hp = min(p.hpMax, p.hp + roboVida);
                        cout << "[RELIQUIA] Recuperas " << roboVida << " HP." << endl;
                    }

                    m.hp -= danoReal;
                    cout << "[TI] " << p.nombreAtaque << " causa " << danoReal << " de dano." << endl;

                    if (p.armaEquipada.efectoId > 0 && m.hp > 0) {
                        if (rand() % 100 < 30) { 
                            Efecto ef = obtenerEfectoPorId(p.armaEquipada.efectoId);
                            if (ef.id != 0) {
                                m.efectosActivos.push_back(ef);
                                cout << "[ESTADO] Aplicaste " << ef.nombre << "-" << endl;
                            }
                        }
                    }
                    
                    // -------------------------------------------------------
                    // PARCHE 3: Tracker de ataque (Flecha del Juicio Final)
                    // -------------------------------------------------------
                    // Marca que el jugador ataco este turno
                    p.ultimoTurnoAtaco = true;
                    // -------------------------------------------------------
                } 

                else if (opc == 2) { 
                    // -------------------------------------------------------
                    // PARCHE 4: Trackeo inactivo
                    // -------------------------------------------------------
                    p.ultimoTurnoAtaco = false; 
                    
                    // -------------------------------------------------------
                    // PARCHE 3: Bloqueo de habilidades por agotamiento
                    // -------------------------------------------------------
                    if (p.turnosAgotado > 0) {
                        cout << "[!] Estas agotado por Colapso Solar. No puedes usar habilidades este turno." << endl;
                        i--; // no gasta turno
                    } else {
                        // --- MENÚ DE HABILIDADES ACTIVAS ---
                        cout << "\n=== HABILIDADES ACTIVAS ===" << endl;
                        vector<int> activas;
                        for (int id : p.habilidadesIds) 
                        {
                            Habilidad h = obtenerHabilidadPorId(id);
                            if (h.tipo == "Activa") 
                            {
                                activas.push_back(id);
                                cout << activas.size() << ". " << h.nombre << " - " << h.descripcion << endl;
                            }
                        }

                        if (activas.empty()) {
                            cout << "[!] No tienes habilidades activas disponibles." << endl;
                            i--; // no gastar turno
                        } else {
                            cout << "Seleccion: ";
                            int sel; cin >> sel;
                            if (sel > 0 && sel <= (int)activas.size()) {
                                int idSeleccionado = activas[static_cast<size_t>(sel - 1)];
                                ejecutarHabilidad(p, m, idSeleccionado); // función central
                            } else {
                                cout << "[-] Seleccion invalida." << endl;
                                i--; // no gastar turno
                            }
                        }
                    } // Cierre del else de agotamiento
                } 
                else if (opc == 3) { // MENÚ DE INVENTARIO
                    // -------------------------------------------------------
                    // PARCHE 4: Trackeo inactivo
                    // -------------------------------------------------------
                    p.ultimoTurnoAtaco = false; 

                    int turnoValidacion = i; 
                    
                    bool enInventario = true;
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
                            i--; 
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
                            system("pause");
                        }
                    }

                    if (i < turnoValidacion) {
                        continue; 
                    }
                }
                else if (opc == 4) { // HUIR
                    // -------------------------------------------------------
                    // PARCHE 4: Trackeo inactivo
                    // -------------------------------------------------------
                    p.ultimoTurnoAtaco = false; 

                    int chanceHuida = 40 + ((p.velocidadBase - m.velocidad) * 3);
                    chanceHuida = max(10, min(chanceHuida, 90));

                    cout << "\n[HUIDA] Intentas escapar... (Probabilidad: " << chanceHuida << "%)" << endl;

                    if (rand() % 100 < chanceHuida) {
                        cout << "[HUIDA] ¡Lograste escapar!" << endl;
                        huidaExitosa = true;
                        system("pause");
                    } else {
                        cout << "[HUIDA] ¡Fallaste al escapar! " << m.nombre << " te bloquea el paso." << endl;
                        system("pause");
                    }
                }

                if (m.hp <= 0) break;

            } else {

            // =========================================================
            // TURNO DEL ENEMIGO INTELIGENTE
            // =========================================================
                bool usoHabilidad = false;

                if (!m.esJefe && (m.esRaro || m.esElite) && !m.habilidadesIds.empty()) {
                    if (m.nombre == "ARZOBISPO VALDRAME") {
                        int idHab = faseValdrame2 ? 402 : 401;
                        ejecutarHabilidadMonstruo(m, p, idHab);
                        usoHabilidad = true;
                    }
                    else if (rand() % 100 < 40) {
                        int idHab = m.habilidadesIds[static_cast<size_t>(rand()) % m.habilidadesIds.size()];
                        bool huyo = ejecutarHabilidadMonstruo(m, p, idHab);
                        if (huyo) monstruoHuyo = true; 
                        usoHabilidad = true;
                    }
                }

                if (!usoHabilidad && !monstruoHuyo) {
                    
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
                        
                        if (p.tieneReliquia("Calavera de Valdrame") && m.hp > 0) {    
                            int absorcion = max(1, static_cast<int>(static_cast<float>(m.hp) * 0.05f));    
                            m.hp  -= absorcion;    
                            p.hp   = min(p.hpMax, p.hp + absorcion);    
                            cout << "[RELIQUIA] Calavera de Valdrame: ¡Absorbes " << absorcion << " HP de " << m.nombre << "!" << endl;
                        }

                        if (p.tieneReliquia("Amuleto de Sangre") && m.hp > 0) {
                            if (rand() % 100 < 20) {
                                m.hp -= 10;
                                cout << "[RELIQUIA] Amuleto de Sangre contraataca por 10 de dano!" << endl;
                            }
                        }

                        p.usarPocionAuto();
                    } // Fin del else (sin escudo)
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
    
    if (huidaExitosa) return;

    if (monstruoHuyo) {
        cout << "\n[INFO] " << m.nombre << " ha huido cobardemente. No ganas recompensa." << endl;
        system("pause");
        return;
    }

    if (m.hp <= 0 && y < 241) {
        int oroGanado = (m.nivelEnemigo * 35) + (rand() % 25); 
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