#include "personajes.h"
#include <iostream>
#include <algorithm>
#include <limits>
#include "Utilidades.h"
#include "Consumibles.h"
#include "Habilidades.h"

using namespace std;

// =========================================================
// BLOQUE 4: Personajes     Version 1.32
// =========================================================

// Inicio del personaje con Atributos Base
Personaje::Personaje(string n, int tipo) {
    nombre = n; nivel = 1; exp = 0; posX = 0; posY = 0; oro = 10;
    
    // Inicialización de equipo base
    artefactoEquipado = Artefacto(0, "Ninguno", 0, "Comun", 0, "Ninguno", 0, 0); 
    armaEquipada = Arma(0, "Manos", 2, "Comun", 0, "Ninguno", 0, 0, 0); // 9 args: agrega habilidadId = 0
    
    usadaPluma = false;
    bonusCritico = 0;

    // --- NUEVO 1.32: Flags de combate para las Ultimates de nivel 20 ---
    // ultimoTurnoAtaco: Flecha del Juicio Final requiere haber atacado el turno anterior
    // turnosAgotado: Colapso Solar bloquea habilidades N turnos tras usarla
    // turnosEscudoCompanero: Pacto de Sangre — el companero absorbe el proximo golpe
    ultimoTurnoAtaco      = false;
    turnosAgotado         = 0;
    turnosEscudoCompanero = 0;
    
    // Asignación de Atributos por Clase
    if (tipo == 1) { // Guerrero: Enfocado en Fuerza y Vitalidad
        clase = "Guerrero"; nombreAtaque = "Hachazo Brutal";
        fuerza = 15; destreza = 8; inteligencia = 5; vitalidad = 12;
        defensaBase = 28; velocidadBase = 10;
    } else if (tipo == 2) { // Mago: Enfocado en Inteligencia
        clase = "Mago"; nombreAtaque = "Explosion Ignea";
        fuerza = 5; destreza = 10; inteligencia = 18; vitalidad = 8;
        defensaBase = 4; velocidadBase = 18;
    } else { // Cazador: Enfocado en Destreza y Velocidad
        clase = "Cazador"; nombreAtaque = "Flecha Perforante";
        fuerza = 10; destreza = 15; inteligencia = 8; vitalidad = 10;
        defensaBase = 6; velocidadBase = 28;
    }

    // Cálculo inicial de estadísticas derivadas
    // --- BALANCE MATEMÁTICO 1.30: Línea base ajustada ---
    hpMax = 50 + (vitalidad * 5); 
    hp = hpMax;
    
    // El ataque ahora escala con el atributo principal de la clase
    if (clase == "Guerrero") ataqueBase = fuerza * 2;
    else if (clase == "Mago") ataqueBase = inteligencia * 2;
    else ataqueBase = destreza * 2;
}

// Determina si un personaje tiene una reliquia (Uso de const para evitar warnings)
bool Personaje::tieneReliquia(const string &r) {
    for (const auto &rel : reliquias) {
        if (rel == r) return true;
    }
    return false;
}

// Sistema de autopociones v2.1 (Limpieza de warnings de tipos)
void Personaje::usarPocionAuto() {
    if (inventario.empty()) return;
    int idx = -1;

    // Buscamos la mejor poción según el estado de salud
    for(size_t i = 0; i < inventario.size(); i++) {
        if (inventario[i].id < 10) {
            // Se usa static_cast para comparar tipos de forma segura
            if (hp < static_cast<int>(hpMax * 0.35) && inventario[i].id == 3) { idx = static_cast<int>(i); break; }
            if (hp < static_cast<int>(hpMax * 0.60) && inventario[i].id == 2) { idx = static_cast<int>(i); break; }
            if (hp < static_cast<int>(hpMax * 0.85) && inventario[i].id == 1) { idx = static_cast<int>(i); break; }
            if (hp < static_cast<int>(hpMax * 0.20) && inventario[i].id == 4) { idx = static_cast<int>(i); break; }
        }
    }

    if (idx != -1) {
        Consumible& p = inventario[static_cast<size_t>(idx)];
        int heal = p.curacion;
        if (p.id == 4) heal = hpMax;

        hp = min(hpMax, hp + heal);
        cout << "\n[SISTEMA] Uso automatico de " << p.nombre << "! Recuperaste " << heal << " HP." << endl;
        cout << "[SISTEMA] HP actual: " << hp << "/" << hpMax << endl;
        inventario.erase(inventario.begin() + idx);
    }
}

// Función para subir de nivel con crecimiento diferenciado
void Personaje::subirNivel() {
    int expNecesaria = nivel * 50;
    while (exp >= expNecesaria) {
        exp -= expNecesaria;
        nivel++;

        // Aumento de atributos base según el Lore de la clase
        if (clase == "Guerrero") {
            fuerza += 4; vitalidad += 3; destreza += 1;
        } else if (clase == "Mago") {
            inteligencia += 5; vitalidad += 1; destreza += 2;
        } else { // Cazador
            destreza += 4; vitalidad += 2; fuerza += 2;
        }

        // --- BALANCE MATEMÁTICO 1.30: Crecimiento Lineal ---
        // Recalcular HP Max: Un valor fijo + bono de vitalidad
        hpMax += 15 + (vitalidad / 3); 
        hp = hpMax;
        
        // Recalcular Ataque: Un valor fijo + bono de stat principal
        if (clase == "Guerrero") ataqueBase += 3 + (fuerza / 4);
        else if (clase == "Mago") ataqueBase += 3 + (inteligencia / 4);
        else ataqueBase += 3 + (destreza / 4);

        defensaBase += 3;
        velocidadBase += 2; // Mantener la progresión de velocidad para turnos
        
        expNecesaria = nivel * 50;
        cout << "\n----------------------------------------" << endl;
        cout << ">>> FELICIDADES  HAS SUBIDO AL NIVEL " << nivel << " <<<" << endl;
        cout << "Tus fuerzas de " << clase << " aumentan considerablemente." << endl;
        cout << "----------------------------------------" << endl;

// --- DESBLOQUEO DE ÁRBOL DE HABILIDADES ---

        //=======================Guerrero======================

        //-----Habilidades Pasivas Nivel 5
        // CORRECCIÓN: Usamos tu lógica del Nivel 10 para evitar que se pierda la habilidad 
        // si el jugador sube del nivel 4 al 6 de golpe por ganar mucha experiencia.
        bool tieneHabilidad5 = false;
        for (int id : habilidadesIds) {
            if (id >= 1 && id <= 3) {
                tieneHabilidad5 = true;
                break;
            }
        }

        if (nivel >= 5 && clase == "Guerrero" && !tieneHabilidad5) {
            cout << "\nHas alcanzado el nivel 5. Tu espiritu guerrero despierta..." << endl;
            cout << "Debes elegir tu senda:\n";
            cout << "1. Guardia de Hierro (+5 defensa con escudos)\n";
            cout << "2. Furia del Acero (+5 ataque con armas)\n";
            cout << "3. Voluntad Inquebrantable (+20 vida maxima)\n";
            cout << "Elige (1-3): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) {
                habilidadesIds.push_back(1);
                defensaBase += 5;
            } else if (eleccion == 2) {
                habilidadesIds.push_back(2);
                ataqueBase += 5;
            } else if (eleccion == 3) {
                habilidadesIds.push_back(3);
                vitalidad += 5;
                hpMax= 50 + (vitalidad * 5); // Recalcular HP Max con el nuevo bono de vitalidad
                hp = hpMax; // Curar al subir de nivel para sentir el poder de la clase
            }

            cout << "---Has desbloqueado la habilidad:--- " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
        }

        
        //-----Habilidades Activas Nivel 10
        // Usamos >= para asegurar que no se pierda la elección si subes más de un nivel de golpe.
        // Además verificamos que aún no tengas una habilidad activa de nivel 10.
        tieneHabilidad10 = false;
        for (int id : habilidadesIds) {
            if (id >= 101 && id <= 103) {
                tieneHabilidad10 = true;
        break;
    }
}

if (nivel >= 10 && clase == "Guerrero" && !tieneHabilidad10) {
    cout << "\nHas alcanzado el nivel 10. Tu furia guerrera se convierte en poder desatado..." << endl;
    cout << "Debes elegir tu nueva habilidad activa:\n";
    cout << "1. Embate con Escudo (dano + paralisis)\n";
    cout << "2. Tajo Sangriento (dano + sangrado)\n";
    cout << "3. Frenesi (multi-hit + sangrado, pero paralisis al usuario)\n";
    cout << "Elige (1-3): ";

    int eleccion; cin >> eleccion;
    if (eleccion == 1) {
        habilidadesIds.push_back(101);
    } else if (eleccion == 2) {
        habilidadesIds.push_back(102);
    } else if (eleccion == 3) {
        habilidadesIds.push_back(103);
    }

    cout << "---Has desbloqueado la habilidad activa:--- " 
         << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
}

        //-----Habilidades Pasivas Nivel 15 (Subclases)
if (nivel >= 15 && clase == "Guerrero" && !tieneSubclase15) {
    cout << "\nHas alcanzado el nivel 15. Tu camino se define..." << endl;
    cout << "Debes elegir tu subclase:\n";
    cout << "1. Paladin (+25% defensa, +25% vitalidad)\n";
    cout << "2. Berserker (+40% fuerza, +40% velocidad, Dano x2 pero SIN ESCUDO)\n";
    cout << "Elige (1-2): ";

    int eleccion; cin >> eleccion;
    if (eleccion == 1) {
        habilidadesIds.push_back(201); // Pasiva Paladín
        aplicarSubclase(*this, 201);
    } else if (eleccion == 2) {
        habilidadesIds.push_back(202); // Pasiva Berserker
        
        // --- LÓGICA DE TRANSFORMACIÓN BERSERKER ---
        this->esBerserker = true; 

        // EL DESCARTE: Limpiar el slot de artefacto permanentemente
        this->artefactoEquipado = Artefacto(0, "Ninguno (Estilo 2 Manos)", 0, "Comun", 0, "Ninguno", 0, 0);

        aplicarSubclase(*this, 202); // Aquí se aplica el +40% de ataque y velocidad
        
        cout << "\n---LA FURIA TE DOMINA--- Has soltado tu escudo para luchar con toda tu fuerza." << endl;
    }

    cout << "---Has desbloqueado la subclase:--- " 
        << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
    tieneSubclase15 = true;
}

        //-----Habilidades Ultimate Nivel 20 (Guerrero)
        // NUEVO 1.32: El jugador no elige, la ultimate se asigna segun la subclase de nivel 15.
        // Paladin -> Ira del Cielo (ID 301) | Berserker -> Ultima Bestia (ID 302)
        // tieneUltimate20 evita que se repita si subes mas de un nivel de golpe.
        if (nivel >= 20 && clase == "Guerrero" && !tieneUltimate20) {
            cout << "\n========================================" << endl;
            cout << "  NIVEL 20 — EL PODER DEFINITIVO" << endl;
            cout << "  Tu voluntad como guerrero alcanza su cima." << endl;
            cout << "========================================" << endl;

            // Buscamos el ID de subclase elegida en nivel 15
            bool esPaladin = false;
            for (int id : habilidadesIds) {
                if (id == 201) { esPaladin = true; break; }
            }

            if (esPaladin) {
                // IRA DEL CIELO: Dano = defensaBase*2.5 + ataqueBase
                // Paralisis garantizada 2 turnos + autocura 15% hpMax
                cout << "Como PALADIN, desbloqueas IRA DEL CIELO:\n";
                cout << "Golpe sagrado devastador. Paralisis garantizada 2 turnos. Te curas 15% HP.\n";
                habilidadesIds.push_back(301);
            } else {
                // ULTIMA BESTIA: ataqueBase*1.8 por golpe, 6-9 golpes aleatorios
                // Si mata -> recuperas el 10% HP de autodano
                // Si NO mata -> pierdes 10% HP propio + enemigo pierde 40% defensa por 2 turnos
                cout << "Como BERSERKER, desbloqueas ULTIMA BESTIA:\n";
                cout << "Frenesi de 6 a 9 golpes devastadores.\n";
                cout << "Si mata: recuperas 10% HP. Si no mata: pierdes 10% HP y rompes la armadura enemiga -40% por 2 turnos.\n";
                habilidadesIds.push_back(302);
            }

            cout << "---ULTIMATE DESBLOQUEADA: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
            tieneUltimate20 = true;
        }

//------------------Mago------------------

        //-----Habilidades Pasivas Nivel 5
        // CORRECCIÓN: Seguridad para no perder la elección si saltas niveles
        bool tieneHabilidad5Mago = false;
        for (int id : habilidadesIds) {
            if (id >= 11 && id <= 13) {
                tieneHabilidad5Mago = true;
                break;
            }
        }

        if (nivel >= 5 && clase == "Mago" && !tieneHabilidad5Mago) {
            cout << "\nHas alcanzado el nivel 5. El poder arcano despierta en tu interior..." << endl;
            cout << "Debes elegir tu senda:\n";
            cout << "1. Muro de Hielo (+5 defensa)\n";
            cout << "2. Llama Arcana (+5 inteligencia)\n";
            cout << "3. Robo Arcano (+20 vida maxima)\n";
            cout << "Elige (1-3): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) {
                habilidadesIds.push_back(11);
                defensaBase += 5;
            } else if (eleccion == 2) {
                habilidadesIds.push_back(12);
                inteligencia += 5;
                ataqueBase += 5; 
            } else if (eleccion == 3) {
                habilidadesIds.push_back(13);
                hpMax += 20;
                hp = hpMax;
            }

            cout << "---Has desbloqueado la habilidad:--- " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
        }

        //-----Habilidades Activas Nivel 10
        bool tieneHabilidad10Mago = false;
        for (int id : habilidadesIds) {
            if (id >= 111 && id <= 113) {
                tieneHabilidad10Mago = true;
                break;
            }
        }

        if (nivel >= 10 && clase == "Mago" && !tieneHabilidad10Mago) {
            cout << "\nHas alcanzado el nivel 10. Tu poder arcano se desata..." << endl;
            cout << "Debes elegir tu nueva habilidad activa:\n";
            cout << "1. Bola de Fuego (dano + quemadura)\n";
            cout << "2. Bola de Escarcha (dano + congelacion)\n";
            cout << "3. Rayo Arcano (dano + desorientacion)\n";
            cout << "Elige (1-3): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) { habilidadesIds.push_back(111); } 
            else if (eleccion == 2) { habilidadesIds.push_back(112); } 
            else if (eleccion == 3) { habilidadesIds.push_back(113); }

            cout << "¡Has desbloqueado la habilidad activa: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
        }


        //-----Habilidades Pasivas Nivel 15 (Subclases)
        if (nivel >= 15 && clase == "Mago" && !tieneSubclase15) {
            cout << "\nHas alcanzado el nivel 15. Tu camino se define..." << endl;
            cout << "Debes elegir tu subclase:\n";
            cout << "1. Fuego (+25% intelecto, +25% vitalidad)\n";
            cout << "2. Hielo (+25% intelecto, +25% defensa)\n";
            cout << "Elige (1-2): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) {
                habilidadesIds.push_back(211); // Pasiva Fuego
                aplicarSubclase(*this, 211);
            } else if (eleccion == 2) {
                habilidadesIds.push_back(212); // Pasiva Hielo
                aplicarSubclase(*this, 212);
            }

            cout << "---Has desbloqueado la subclase:--- " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
            tieneSubclase15 = true;
        }

        //-----Habilidades Ultimate Nivel 20 (Mago)
        // NUEVO 1.32: Mago Fuego -> Colapso Solar (ID 311) | Mago Hielo -> Absoluto Cero (ID 312)
        if (nivel >= 20 && clase == "Mago" && !tieneUltimate20) {
            cout << "\n========================================" << endl;
            cout << "  NIVEL 20 — EL PODER DEFINITIVO" << endl;
            cout << "  La magia en ti alcanza su forma mas pura." << endl;
            cout << "========================================" << endl;

            bool esFuego = false;
            for (int id : habilidadesIds) {
                if (id == 211) { esFuego = true; break; }
            }

            if (esFuego) {
                // COLAPSO SOLAR: Dano = inteligencia*3.5
                // Quemadura garantizada 3 turnos
                // Debuff propio: turnosAgotado = 1 (no puede usar habilidades ese turno)
                cout << "Como MAGO DE FUEGO, desbloqueas COLAPSO SOLAR:\n";
                cout << "Una estrella en miniatura. Quemadura garantizada 3 turnos.\n";
                cout << "ADVERTENCIA: Te deja agotado 1 turno. No podras usar habilidades ese turno.\n";
                habilidadesIds.push_back(311);
            } else {
                // ABSOLUTO CERO: Dano = inteligencia*2.0 (x2 si enemigo ya congelado)
                // Congelacion garantizada 3 turnos + reduce ATQ enemigo -30% mientras dure
                cout << "Como MAGO DE HIELO, desbloqueas ABSOLUTO CERO:\n";
                cout << "El tiempo se detiene. Congelacion garantizada 3 turnos y -30% ATQ enemigo.\n";
                cout << "COMBO: Si el enemigo ya esta congelado, el dano se DUPLICA.\n";
                habilidadesIds.push_back(312);
            }

            cout << "---ULTIMATE DESBLOQUEADA: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
            tieneUltimate20 = true;
        }

//------------------Cazador------------------

        //-----Habilidades Pasivas Nivel 5
        // CORRECCIÓN: Seguridad para saltos de nivel
        bool tieneHabilidad5Cazador = false;
        for (int id : habilidadesIds) {
            if (id >= 21 && id <= 23) {
                tieneHabilidad5Cazador = true;
                break;
            }
        }

        if (nivel >= 5 && clase == "Cazador" && !tieneHabilidad5Cazador) {
            cout << "\nHas alcanzado el nivel 5. Tus sentidos se agudizan y tu instinto cazador despierta..." << endl;
            cout << "Debes elegir tu senda:\n";
            cout << "1. Ojo de Halcon (+5 destreza)\n";
            cout << "2. Paso Ligero (+5 velocidad)\n";
            cout << "3. Instinto de Supervivencia (+20 vida maxima)\n";
            cout << "Elige (1-3): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) {
                habilidadesIds.push_back(21);
                destreza += 5;
                ataqueBase += 5; 
            } else if (eleccion == 2) {
                habilidadesIds.push_back(22);
                velocidadBase += 5;
            } else if (eleccion == 3) {
                habilidadesIds.push_back(23);
                hpMax += 20;
                hp = hpMax;
            }

            cout << "¡Has desbloqueado la habilidad: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
        }

        //-----Habilidades Activas Nivel 10
        bool tieneHabilidad10Cazador = false;
        for (int id : habilidadesIds) {
            if (id >= 121 && id <= 123) {
                tieneHabilidad10Cazador = true;
                break;
            }
        }

        if (nivel >= 10 && clase == "Cazador" && !tieneHabilidad10Cazador) {
            cout << "\nHas alcanzado el nivel 10. Tus instintos cazadores se perfeccionan..." << endl;
            cout << "Debes elegir tu nueva habilidad activa:\n";
            cout << "1. Flecha Venenosa (dano + veneno)\n";
            cout << "2. Disparo Preciso (golpe critico, puro dano)\n";
            cout << "3. Lluvia de Flechas (multi-hit + sangrado)\n";
            cout << "Elige (1-3): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) { habilidadesIds.push_back(121); } 
            else if (eleccion == 2) { habilidadesIds.push_back(122); } 
            else if (eleccion == 3) { habilidadesIds.push_back(123); }

            cout << "¡Has desbloqueado la habilidad activa: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
        }

        //-----Habilidades Pasivas Nivel 15 (Subclases)
        if (nivel >= 15 && clase == "Cazador" && !tieneSubclase15) {
            cout << "\nHas alcanzado el nivel 15. Tu camino se define..." << endl;
            cout << "Debes elegir tu subclase:\n";
            cout << "1. Punteria (+25% destreza, +25% velocidad)\n";
            cout << "2. Bestias (+25% destreza, +25% defensa)\n";
            cout << "Elige (1-2): ";

            int eleccion; cin >> eleccion;
            if (eleccion == 1) {
                habilidadesIds.push_back(221); // Pasiva Puntería
                aplicarSubclase(*this, 221);
            } else if (eleccion == 2) {
                habilidadesIds.push_back(222); // Pasiva Bestias
                aplicarSubclase(*this, 222);
            }

            cout << "---Has desbloqueado la subclase:--- " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
            tieneSubclase15 = true;
        }

        //-----Habilidades Ultimate Nivel 20 (Cazador)
        // NUEVO 1.32: Punteria -> Flecha del Juicio Final (ID 321) | Bestias -> Pacto de Sangre (ID 322)
        if (nivel >= 20 && clase == "Cazador" && !tieneUltimate20) {
            cout << "\n========================================" << endl;
            cout << "  NIVEL 20 — EL PODER DEFINITIVO" << endl;
            cout << "  Tu precision y tus instintos llegan al limite." << endl;
            cout << "========================================" << endl;

            bool esPunteria = false;
            for (int id : habilidadesIds) {
                if (id == 221) { esPunteria = true; break; }
            }

            if (esPunteria) {
                // FLECHA DEL JUICIO FINAL: Dano = destreza*2.0 + ataqueBase*1.5
                // Ignora completamente la defensa del enemigo
                // 50% de chance de veneno 3 turnos
                // REQUISITO: ultimoTurnoAtaco == true (debes haber atacado el turno anterior)
                cout << "Como maestro de PUNTERIA, desbloqueas FLECHA DEL JUICIO FINAL:\n";
                cout << "El disparo definitivo. Ignora toda la defensa del enemigo.\n";
                cout << "REQUISITO: Debes haber atacado el turno anterior para cargar el disparo.\n";
                cout << "50% de chance de aplicar veneno 3 turnos.\n";
                habilidadesIds.push_back(321);
            } else {
                // PACTO DE SANGRE: Dano = destreza*1.5 (tu) + destreza*1.0 (companero)
                // Sangrado garantizado (tu ataque) + Veneno garantizado (companero)
                // turnosEscudoCompanero = 1: el companero absorbe el proximo golpe que recibirias
                cout << "Como maestro de BESTIAS, desbloqueas PACTO DE SANGRE:\n";
                cout << "Tu y tu companero atacan como uno. Dano doble.\n";
                cout << "Sangrado + Veneno garantizados. Tu companero absorbe el proximo golpe que recibirias.\n";
                habilidadesIds.push_back(322);
            }

            cout << "---ULTIMATE DESBLOQUEADA: " 
                 << obtenerHabilidadPorId(habilidadesIds.back()).nombre << "!" << endl;
            tieneUltimate20 = true;
        }

        // Finalización del ciclo de subida de nivel
        expNecesaria = nivel * 50; 
    }
}

// Función para reaparecer (Reset de posición y penalización de oro)
void Personaje::reaparecer() {
    cout << "\n****************************************" << endl;
    cout << "   HAS CAIDO EN COMBATE... " << endl;
    cout << "****************************************" << endl;
    cout << "Despiertas con el cuerpo magullado en la Aldea." << endl;
    
    // Restauración de estado vital
    hp = hpMax;
    posY = 0;
    posX = 0;
    
    // Penalización de oro: Mantenemos tu lógica de no dejarlo en negativo
    if (oro > 15) {
        cout << "Has perdido 15 de oro en el camino de regreso." << endl;
        oro -= 15;
    } else {
        cout << "Has perdido todo tu oro restante en la huida..." << endl;
        oro = 0;
    }
    
    // Limpiar estados alterados al morir (Opcional, pero recomendado)
    efectos.clear();
    
    system("pause");
}

// =========================================================
// NUEVA FUNCIÓN: Sincronizador de estadísticas 1.30
// =========================================================
void Personaje::actualizarEstadisticas() {
    /* 
       En la Versión 1.30, los incrementos son acumulativos en subirNivel()
       para respetar los bonos elegidos por el usuario.
       Esta función queda como puente técnico para futuras expansiones
       (como un sistema de 'Respec' o reinicio de talentos, o el NG+).
    */
}