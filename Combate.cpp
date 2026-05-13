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

// --- AGREGAR ESTA LÍNEA ---
// Le avisa a este archivo que 'generarHorda' existe en otro lado (monstruosdatos.cpp)
extern std::vector<Monstruo> generarHorda(int zonaDeseada); 

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

    // --- CAMBIO IMPORTANTE ---
    // Siempre regeneramos la horda de la zona actual con la función que tienes en monstruosdatos.cpp
    listaMonstruos = generarHorda(zona);
    // ------------------------------

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
        return posibles[static_cast<size_t>(rand() % posibles.size())];
    }

    // Retorno de seguridad
    return Monstruo{"Error", 1, 1, 1, 1, 0, 1, 1, false};
}

void iniciarCombate(Personaje &p, int y) {
    int turnosAbsorcion = 0; // Nueva variable para controlar la absorción de Lancelot
    Monstruo m = generarEnemigo(y);
    int hpInicial = m.hp; // Mantengo tu variable para el HUD y las fases
    bool fase2 = false;
    bool fase3 = false;
    bool huidaExitosa = false; // Variable necesaria para la opción de huir
    bool monstruoHuyo = false; // NUEVO 1.31: Para cuando el enemigo decide escapar

    // Respeto total a tu formato de título y etiqueta
    // NUEVO 1.31: Añadida etiqueta [JEFE] para Lancelot además de [RARO]
    string tituloCombate = string("COMBATE CONTRA: ") + (m.esJefe ? "[JEFE] " : m.esRaro ? "[RARO] " : "") + m.nombre;
    mostrarCabecera(tituloCombate);

    while (p.hp > 0 && m.hp > 0) {
        system("cls"); 

        // HUD Actualizado: Ahora incluye el Nivel del monstruo
        // NUEVO 1.31: Etiqueta [JEFE] para Lancelot
        cout << "====================================================" << endl;
        cout << " " << p.nombre << " [" << p.hp << "/" << p.hpMax << " HP]  VS  " 
        << (m.esJefe ? "[JEFE] " : m.esRaro ? "[RARO] " : "") << m.nombre
        << " (Nvl " << m.nivelEnemigo << ") [" << m.hp << "/" << hpInicial << " HP]" << endl;
        // NUEVO 1.31: Mostrar naturaleza del monstruo si tiene una asignada
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


       //Combate vs Lancelot el Rey Dragon
        // LÓGICA DE FASES (Sincronizada con y >= 241)

        // --- FASE 2: EL ESCUDO DEL REY (Activa al 50% de HP) ---
        if (y >= 241 && !fase2 && m.hp <= (hpInicial * 0.5)) {
            fase2 = true;
            // Tu buff pasivo original (intacto)
            m.defensa = (int)(m.defensa * 1.30); 
            
            // Nuestra nueva mecánica activa: Le damos 2 turnos de absorción
            turnosAbsorcion = 2; 
            
            // Expandimos el lore en la consola para avisar al jugador del peligro
            cout << "\n LANCELOT CAMBIA SUS ESCAMAS  Defensa aumentada." << endl;
            cout << "[MECANICA] Las escamas del Rey brillan con la magia corrupta de Angra." << endl;
            cout << "[PELIGRO] --Lancelot absorbera el dano de tus proximos--" << turnosAbsorcion << " ataques y lo convertira en vida!" << endl;
            system("pause");
        }

        // --- FASE 3: LA FURIA DE ANGRA (Activa al 25% de HP) ---
        if (y >= 241 && !fase3 && m.hp <= (hpInicial * 0.25)) {
            fase3 = true;
            // Tus buffs pasivos originales (intactos)
            m.ataque = (int)(m.ataque * 1.30);
            m.velocidad = (int)(m.velocidad * 1.30);
            
            cout << "\n LANCELOT RUGE CON FURIA  Ataque y Velocidad aumentados." << endl;
            
            // Nuestra nueva mecánica activa: Ataque inmediato fuera de turno
            cout << "[MECANICA] La sombra de Angra toma el control total del Rey para un golpe a traicion..." << endl;
            
            // Calculamos un golpe especial devastador (1.5x de su ataque ya bufado)
            int danoEspecial = (int)(m.ataque * 1.5);
            
            // Le restamos la vida directamente a Bastian (asumiendo que tu variable es 'p.hp')
            p.hp -= danoEspecial;
            
            cout << "[ENE] --Lancelot ejecuta 'ALIENTO DEL VACIO' y te causa-- " << danoEspecial << " de dano directo!" << endl;
            
            // Validamos si Bastian sobrevivió al golpe traicionero para darle más drama
            if (p.hp > 0) {
                cout << "[SISTEMA] Has resistido el embate, pero estas gravemente herido." << endl;
            } else {
                cout << "[SISTEMA] El poder del Caos te ha abrumado..." << endl;
            }
            
            system("pause");
        }


// --- Turnos ---
        bool turnoJugador = (p.velocidadBase >= m.velocidad);
        
        // NUEVO 1.31: Añadido monstruoHuyo al check para salir del loop si el enemigo huyó
        for (int i = 0; i < 2; i++) {
            if (m.hp <= 0 || p.hp <= 0 || huidaExitosa || monstruoHuyo) break;

            if ((i == 0 && turnoJugador) || (i == 1 && !turnoJugador)) {
                // --- Turno del Jugador (Menú Táctico) ---
                cout << "\n--- TU TURNO ---" << endl;
                cout << "1. " << p.nombreAtaque << " | 2. Habilidades (0) | 3. Inventario | 4. Huir" << endl;
                cout << "Seleccion: ";
                int opc; cin >> opc;

                if (opc == 1) { // ATAQUE BÁSICO (Tu lógica original)
                    int dTotal = p.ataqueBase + p.armaEquipada.poder;
                    bool esCritico = (rand() % 100 < (5 + p.bonusCritico));
                    int danoReal = max(2, dTotal - m.defensa);

                    if (esCritico) {
                        danoReal = (int)(danoReal * 1.5);
                        cout << "[CRITICO] Golpe devastador" << endl;
                    }

                    if (p.tieneReliquia("Colmillo de Vampiro")) {
                        int roboVida = (int)(danoReal * 0.15);
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
                } 


                else if (opc == 2) { 
               
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
                        int idSeleccionado = activas[sel-1];
                        ejecutarHabilidad(p, m, idSeleccionado); // función central
                    } else {
                        cout << "[-] Seleccion invalida." << endl;
                    i--; // no gastar turno
        }
    }
} 
                else if (opc == 3) { // MENÚ DE INVENTARIO (10 RANURAS)
    // --- EL CANDADO ---
    // Guardamos el turno actual para saber si el jugador canceló la acción
    int turnoValidacion = i; 
    
    bool enInventario = true;
    while (enInventario) {
        system("cls");
        cout << "=== INVENTARIO DE CONSUMIBLES (" << p.inventario.size() << "/10) ===" << endl;

        if (p.inventario.empty()) {
            cout << "\n[-] No tienes objetos en el inventario." << endl;
            cout << "1. Volver" << endl;
        } else {
            // Listar dinámicamente lo que hay en p.inventario
            for (size_t j = 0; j < p.inventario.size(); ++j) {
                cout << j + 1 << ". " << p.inventario[j].nombre 
                     << " - " << p.inventario[j].descripcion << endl;
            }
            cout << p.inventario.size() + 1 << ". Volver" << endl;
        }

        cout << "\nSeleccion: ";
        int selInv;
        if (!(cin >> selInv)) { cin.clear(); cin.ignore(1000, '\n'); continue; }

        // Si elige volver o el inventario está vacío
        if (selInv == (int)p.inventario.size() + 1 || p.inventario.empty()) {
            i--; // Restamos al turno para que no avance
            enInventario = false;
            // Salimos del while para procesar la validación
        }
        else if (selInv > 0 && selInv <= (int)p.inventario.size()) {
            Consumible& obj = p.inventario[selInv - 1];

            // 1. Lógica de Curación
            if (obj.curacion > 0) {
                p.hp = std::min(p.hpMax, p.hp + obj.curacion);
                cout << "\n[-] Usaste " << obj.nombre << ". Recuperas " << obj.curacion << " HP." << endl;
            }

            // 2. Lógica de Elixires (Atributos Base)
            if (obj.buffAtaque > 0) {
                p.fuerza += obj.buffAtaque; // Sube STR
                cout << "[!] Fuerza aumentada a: " << p.fuerza << endl;
            }
            if (obj.buffDefensa > 0) {
                p.vitalidad += (obj.buffDefensa / 2); // Sube VIT
                cout << "[!] Vitalidad aumentada a: " << p.vitalidad << endl;
            }
            if (obj.buffVelocidad > 0) {
                p.destreza += obj.buffVelocidad; // Sube DEX
                cout << "[!] Destreza aumentada a: " << p.destreza << endl;
            }

            // Eliminar del inventario de 10 ranuras
            p.inventario.erase(p.inventario.begin() + (selInv - 1));
            
            enInventario = false; // Aquí NO restamos i, el turno termina
            system("pause");
        }
    }

    // --- APLICACIÓN DEL CANDADO ---
    // Si i es menor que turnoValidacion, significa que se hizo i-- (el jugador canceló)
    if (i < turnoValidacion) {
        continue; // Salta el turno del enemigo y vuelve a mostrar Atacar/Habilidades/Inventario
    }
}

    else if (opc == 4) { // HUIR
    int chanceHuida = 40 + ((p.velocidadBase - m.velocidad) * 3);
    chanceHuida = max(10, min(chanceHuida, 90));

    cout << "\n[HUIDA] Intentas escapar... (Probabilidad: " << chanceHuida << "%)" << endl;

    if (rand() % 100 < chanceHuida) {
        cout << "[HUIDA] ¡Lograste escapar!" << endl;
        huidaExitosa = true;
        system("pause");
    } else {
        // Fallo: no hacemos nada, el turno pasa y el enemigo atacará normalmente
        cout << "[HUIDA] ¡Fallaste al escapar! " << m.nombre << " te bloquea el paso." << endl;
        system("pause");
    }
}

                if (m.hp <= 0) break;
            } else {

// =========================================================
// NUEVO 1.31: TURNO DEL ENEMIGO INTELIGENTE
// Antes solo atacaba. Ahora los Raros y Jefes pueden usar
// habilidades del modulo HabilidadesMonstruo con 40% de prob.
// Los comunes siguen atacando normal siempre.
// =========================================================
                bool usoHabilidad = false;

                // Solo Raros y Jefes tienen habilidades asignadas
                if ((m.esRaro || m.esJefe) && !m.habilidadesIds.empty()) {
                    // 40% de probabilidad de usar habilidad en vez de ataque normal
                    if (rand() % 100 < 40) {
                        int idHab = m.habilidadesIds[rand() % m.habilidadesIds.size()];
                        bool huyo = ejecutarHabilidadMonstruo(m, p, idHab);
                        if (huyo) monstruoHuyo = true; // Si la habilidad fue Huida exitosa
                        usoHabilidad = true;
                    }
                }

                // Si no usó habilidad (o es un común), ataque normal original
                if (!usoHabilidad && !monstruoHuyo) {
                    // AJUSTE: Se elimina armaEquipada.poder de la defensa para evitar daño nulo
                    double factorBase = (double)m.ataque / (p.defensaBase + p.artefactoEquipado.defensa); 
                    int dEnemigo = (int)((p.hpMax * 0.05) * factorBase);
                    dEnemigo = max(1, dEnemigo);

                    p.hp -= dEnemigo;
                    cout << "[ENE] " << m.nombre << " (Nvl " << m.nivelEnemigo << ") te golpea por " << dEnemigo << " de dano." << endl;
                    
                    // NUEVO 1.31: Amuleto de Sangre — 20% de contraatacar al recibir daño
                    if (p.tieneReliquia("Amuleto de Sangre") && m.hp > 0) {
                        if (rand() % 100 < 20) {
                            m.hp -= 10;
                            cout << "[RELIQUIA] Amuleto de Sangre contraataca por 10 de dano!" << endl;
                        }
                    }

                    // Mantenemos tu poción automática aquí para que el jugador no muera súbitamente
                    p.usarPocionAuto();
                }

                if (p.hp <= 0) break;
            }
        } // Fin del for de los 2 turnos

        // Salida inmediata si alguien huyó exitosamente
        if (huidaExitosa || monstruoHuyo) break;

        // TU PAUSA DE RUNDA: Respetada al 100%
        if (m.hp > 0 && p.hp > 0) {
            cout << "\nPresiona ENTER para el siguiente turno...";
            limpiarBuffer(); cin.get();
        }
    } // Fin del while de combate

// --- Resultado del combate ---
    
    // Si el jugador huyó con éxito, salimos de la función sin procesar victoria ni derrota
    if (huidaExitosa) return;

    // NUEVO 1.31: Si el monstruo huyó, no hay recompensa
    if (monstruoHuyo) {
        cout << "\n[INFO] " << m.nombre << " ha huido cobardemente. No ganas recompensa." << endl;
        system("pause");
        return;
    }

    if (m.hp <= 0 && y < 241) {
        // Victoria estándar: Oro dependiente del nivel y experiencia
        // NUEVO 1.31: Piedra del Alma añade 25% de oro extra si el jugador la tiene
        int oroGanado = (m.nivelEnemigo * 35) + (rand() % 25); 
        if (p.tieneReliquia("Piedra del Alma")) {
            oroGanado = (int)(oroGanado * 1.25f);
            cout << "[RELIQUIA] Piedra del Alma: +25% oro!" << endl;
        }
        cout << "\n--ENEMIGO DERROTADO--" << endl;
        cout << "Ganaste " << oroGanado << " de Oro y " << m.expAlMorir << " puntos de Experiencia." << endl;
        
        p.oro += oroGanado;
        p.exp += m.expAlMorir;
        
        // Lógica original de botín y nivel
        gestionarLoot(p, y, m.esRaro);
        p.subirNivel();
        system("pause");

    // Este es si ganas el combate final (LORE INTACTO)
    } else if (m.hp <= 0 && y >= 241) {
        cout << "\n--LANCELOT HA CAIDO-- El cielo se aclara por primera vez en decadas." << endl;
        cout << "Entre las cenizas recoges la ESCAMA DEL REY DRAGON, simbolo eterno de tu victoria." << endl;
        system("pause");
        
    } else if (p.hp <= 0) {
        // Lógica de muerte original
        p.reaparecer();
    }  
}