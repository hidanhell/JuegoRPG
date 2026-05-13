#include "Habilidades.h"
#include "Personajes.h"
#include "Efectos.h"
#include <iostream>

extern std::vector<Efecto> listaEfectos;
extern std::vector<Habilidad> listaHabilidadesGlobal;

// ==========================
// FUNCIÓN GLOBAL DE ACCESO
// ==========================
Habilidad obtenerHabilidadPorId(int id) {
    for (auto &h : listaHabilidadesGlobal) {
        if (h.id == id) return h;
    }
    return {0, "Ninguna", "Sin efecto", 0, 0, "Nulo", 0};
}

// ============================================
// EJECUCIÓN DE HABILIDADES PERSONAJES
// ============================================
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad) {
    Habilidad hab = obtenerHabilidadPorId(idHabilidad);
    int dano = 0;
    bool turnoPerdido = false;

    switch (hab.id) {

        // -----------------------------------------------
        // GUERRERO — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 101: // Embate con Escudo
            dano = pj.ataqueBase + static_cast<int>(pj.defensaBase * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[4].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 102: // Tajo Sangriento
            dano = pj.ataqueBase + static_cast<int>(pj.fuerza * 1.3);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[3].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 103: // Frenesí
        {
            int golpes = rand() % 3 + 2;
            int danoTotal = 0;
            for (int i = 0; i < golpes; i++) {
                int golpe = static_cast<int>(pj.ataqueBase * 1.5);
                m.hp -= golpe;
                danoTotal += golpe;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " realiza " << golpes
                      << " golpes e inflige un total de " << danoTotal << " de dano.\n";
            listaEfectos[3].aplicar(m.hp, m.hpMax, turnoPerdido);
        }
            break;

        // -----------------------------------------------
        // MAGO — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 111: // Bola de Fuego
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[1].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 112: // Bola de Escarcha
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[6].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 113: // Rayo Arcano
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[7].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        // -----------------------------------------------
        // CAZADOR — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 121: // Flecha Venenosa
            dano = pj.destreza + static_cast<int>(pj.destreza * 1.4);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            listaEfectos[2].aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 122: // Disparo Preciso
            dano = pj.ataqueBase + static_cast<int>(pj.destreza * 1.5);
            m.hp -= dano;
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige un golpe critico de " << dano << " de dano.\n";
            break;

        case 123: // Lluvia de Flechas
        {
            int impactos = rand() % 3 + 3;
            int danoTotal = 0;
            for (int i = 0; i < impactos; i++) {
                int golpe = static_cast<int>(pj.destreza * 1.4);
                m.hp -= golpe;
                danoTotal += golpe;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " lanza " << impactos
                      << " flechas e inflige un total de " << danoTotal << " de dano.\n";
            listaEfectos[3].aplicar(m.hp, m.hpMax, turnoPerdido);
        }
            break;

        // -----------------------------------------------
        // ULTIMATES NIVEL 20 — POR SUBCLASE
        // -----------------------------------------------

        case 301: // IRA DEL CIELO — Paladin
        {
            std::cout << "[ULTIMATE] " << pj.nombre << " alza su escudo al cielo. Una luz cegadora desciende!\n";
            dano = static_cast<int>(pj.defensaBase * 2.5f) + pj.ataqueBase;
            m.hp -= dano;
            std::cout << "[ULTIMATE] Ira del Cielo inflige " << dano << " de dano sagrado.\n";

            // Paralisis garantizada 2 turnos
            for (auto& ef : listaEfectos) {
                if (ef.id == 4) {
                    Efecto paralisis = ef;
                    paralisis.duracion = 2;
                    m.efectosActivos.push_back(paralisis);
                    std::cout << "[ULTIMATE] " << m.nombre << " queda PARALIZADO por 2 turnos!\n";
                    break;
                }
            }

            // Autocura 15% hpMax
            int cura = static_cast<int>(static_cast<float>(pj.hpMax) * 0.15f);
            pj.hp = std::min(pj.hpMax, pj.hp + cura);
            std::cout << "[ULTIMATE] Tu fe te restaura " << cura << " HP.\n";
        }
            break;

        case 302: // ULTIMA BESTIA — Berserker
        {
            std::cout << "[ULTIMATE] " << pj.nombre << " pierde la razon. Sus ojos se tornan rojos...\n";

            int golpes = rand() % 4 + 6; // 6 a 9 golpes
            int danoTotal = 0;
            for (int i = 0; i < golpes; i++) {
                int golpe = static_cast<int>(pj.ataqueBase * 1.8f);
                m.hp -= golpe;
                danoTotal += golpe;
            }
            std::cout << "[ULTIMATE] Ultima Bestia realiza " << golpes
                      << " golpes devastadores por un total de " << danoTotal << " de dano!\n";

            // Autodano: 10% HP propio
            int autodano = static_cast<int>(static_cast<float>(pj.hpMax) * 0.10f);

            if (m.hp <= 0) {
                // Mato al enemigo: recupera el autodano
                std::cout << "[ULTIMATE] El enemigo cayo! Tu frenesi te recupera " << autodano << " HP.\n";
                pj.hp = std::min(pj.hpMax, pj.hp + autodano);
            } else {
                // No mato: pierde vida + debuff armadura enemiga 40% por 2 turnos
                pj.hp -= autodano;
                std::cout << "[ULTIMATE] El enemigo sobrevivio. El frenesi te cuesta " << autodano << " HP.\n";

                // Debuff defensa enemiga -40% por 2 turnos (lo guardamos en el monstruo)
                int defensaOriginal = m.defensa;
                m.defensa = static_cast<int>(static_cast<float>(m.defensa) * 0.60f);
                m.turnosDebuffDefensa = 2; // necesitas este campo en Monstruo (ver nota abajo)
                std::cout << "[ULTIMATE] La armadura de " << m.nombre
                          << " se rompe! Defensa reducida de " << defensaOriginal
                          << " a " << m.defensa << " por 2 turnos.\n";

                // Sangrado garantizado
                listaEfectos[3].aplicar(m.hp, m.hpMax, turnoPerdido);
            }
        }
            break;

        case 311: // COLAPSO SOLAR — Mago Fuego
        {
            std::cout << "[ULTIMATE] Una luz solar cegadora surge de tus manos...\n";
            dano = static_cast<int>(pj.inteligencia * 3.5f);
            m.hp -= dano;
            std::cout << "[ULTIMATE] Colapso Solar inflige " << dano << " de dano de fuego puro!\n";

            // Quemadura garantizada 3 turnos
            for (auto& ef : listaEfectos) {
                if (ef.id == 1) {
                    Efecto quemadura = ef;
                    quemadura.duracion = 3;
                    m.efectosActivos.push_back(quemadura);
                    std::cout << "[ULTIMATE] " << m.nombre << " arde en llamas por 3 turnos!\n";
                    break;
                }
            }

            // Debuff propio: agotado (no puede usar habilidades el proximo turno)
            pj.turnosAgotado = 1;
            std::cout << "[ULTIMATE] El esfuerzo te agota. No podras usar habilidades el proximo turno.\n";
        }
            break;

        case 312: // ABSOLUTO CERO — Mago Hielo
        {
            std::cout << "[ULTIMATE] El aire se cristaliza. El tiempo parece detenerse...\n";
            dano = static_cast<int>(pj.inteligencia * 2.0f);

            // Combo: si ya esta congelado el daño se duplica
            bool yaCongelado = false;
            for (const auto& ef : m.efectosActivos) {
                if (ef.id == 6) { yaCongelado = true; break; }
            }
            if (yaCongelado) {
                dano *= 2;
                std::cout << "[COMBO] El enemigo ya estaba congelado! Dano DUPLICADO!\n";
            }

            m.hp -= dano;
            std::cout << "[ULTIMATE] Absoluto Cero inflige " << dano << " de dano glacial!\n";

            // Congelacion garantizada 3 turnos
            for (auto& ef : listaEfectos) {
                if (ef.id == 6) {
                    Efecto hielo = ef;
                    hielo.duracion = 3;
                    m.efectosActivos.push_back(hielo);
                    std::cout << "[ULTIMATE] " << m.nombre << " queda CONGELADO por 3 turnos!\n";
                    break;
                }
            }

            // Debuff ataque enemigo -30% mientras dure la congelacion
            int ataqueOriginal = m.ataque;
            m.ataque = static_cast<int>(static_cast<float>(m.ataque) * 0.70f);
            std::cout << "[ULTIMATE] El frio reduce el ataque de " << m.nombre
                      << " de " << ataqueOriginal << " a " << m.ataque << ".\n";
        }
            break;

        case 321: // FLECHA DEL JUICIO FINAL — Punteria
        {
            // Requiere haber atacado el turno anterior
            if (!pj.ultimoTurnoAtaco) {
                std::cout << "[ULTIMATE] Necesitas haber atacado el turno anterior para cargar este disparo!\n";
                break;
            }

            std::cout << "[ULTIMATE] " << pj.nombre << " tensa el arco al limite...\n";
            // Ignora defensa: dano directo sin restar defensa
            dano = static_cast<int>(pj.destreza * 2.0f) + static_cast<int>(pj.ataqueBase * 1.5f);
            m.hp -= dano; // directo, sin pasar por defensa
            std::cout << "[ULTIMATE] Flecha del Juicio Final traspasa toda defensa por " << dano << " de dano!\n";

            // 50% de veneno 3 turnos
            if (rand() % 100 < 50) {
                for (auto& ef : listaEfectos) {
                    if (ef.id == 2) {
                        Efecto veneno = ef;
                        veneno.duracion = 3;
                        m.efectosActivos.push_back(veneno);
                        std::cout << "[ULTIMATE] La flecha estaba envenenada! Veneno por 3 turnos.\n";
                        break;
                    }
                }
            }
        }
            break;

        case 322: // PACTO DE SANGRE — Bestias
        {
            std::cout << "[ULTIMATE] Un silbido agudo rasga el aire. Tu companero emerge de las sombras!\n";

            // Tu ataque
            int danoTuyo = static_cast<int>(pj.destreza * 1.5f);
            m.hp -= danoTuyo;
            std::cout << "[TU] Atacas por " << danoTuyo << " de dano!\n";

            // Ataque del companero
            int danoCompanero = static_cast<int>(pj.destreza * 1.0f);
            m.hp -= danoCompanero;
            std::cout << "[COMPANERO] Tu bestia salta y causa " << danoCompanero << " de dano!\n";

            int danoTotal = danoTuyo + danoCompanero;
            std::cout << "[ULTIMATE] Pacto de Sangre inflige " << danoTotal << " de dano total!\n";

            // Sangrado garantizado (tu ataque)
            listaEfectos[3].aplicar(m.hp, m.hpMax, turnoPerdido);
            std::cout << "[ULTIMATE] Tu golpe causa SANGRADO!\n";

            // Veneno garantizado (companero)
            for (auto& ef : listaEfectos) {
                if (ef.id == 2) {
                    m.efectosActivos.push_back(ef);
                    std::cout << "[ULTIMATE] Tu companero envenena al enemigo!\n";
                    break;
                }
            }

            // Companero absorbe el proximo golpe
            pj.turnosEscudoCompanero = 1;
            std::cout << "[COMPANERO] Tu bestia se interpone. Absorbera el proximo golpe que recibirias.\n";
        }
            break;

        default:
            std::cout << "[HABILIDAD] " << hab.nombre << " no tiene logica definida aun.\n";
            break;
    }
}

// ============================================
// APLICAR SUBCLASE NIVEL 15 (Pasivas)
// ============================================
void aplicarSubclase(Personaje &pj, int idHabilidad) {
    Habilidad hab = obtenerHabilidadPorId(idHabilidad);

    switch (hab.id) {
        case 201: // Paladín
            pj.defensaBase = static_cast<int>(pj.defensaBase * 1.40);
            pj.vitalidad   = static_cast<int>(pj.vitalidad   * 1.40);
            pj.hpMax       = static_cast<int>(pj.hpMax       * 1.40);
            pj.hp = pj.hpMax;
            break;

        case 202: // Berserker
            pj.ataqueBase    = static_cast<int>(pj.ataqueBase    * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        case 211: // Mago Fuego
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.vitalidad    = static_cast<int>(pj.vitalidad    * 1.35);
            pj.hpMax        = static_cast<int>(pj.hpMax        * 1.35);
            pj.hp = pj.hpMax;
            break;

        case 212: // Mago Hielo
            pj.inteligencia = static_cast<int>(pj.inteligencia * 1.35);
            pj.defensaBase  = static_cast<int>(pj.defensaBase  * 1.35);
            break;

        case 221: // Puntería
            pj.destreza      = static_cast<int>(pj.destreza      * 1.40);
            pj.velocidadBase = static_cast<int>(pj.velocidadBase * 1.40);
            break;

        case 222: // Bestias
            pj.destreza  = static_cast<int>(pj.destreza  * 1.35);
            pj.vitalidad = static_cast<int>(pj.vitalidad * 1.35);
            pj.hpMax     = static_cast<int>(pj.hpMax     * 1.35);
            pj.hp = pj.hpMax;
            break;

        default:
            std::cout << "[SUBCLASE] No definida aun.\n";
            break;
    }

    std::cout << "[SUBCLASE] " << hab.nombre << " activada: " << hab.descripcion << "\n";
}

// ============================================
// BLOQUE: HABILIDADES DE OBJETOS / ARMAS ELITE
// ============================================

// GRITO DEL CAIDO — Armas Elite de Valdrame
// Se activa automaticamente al hacer un critico con rareza "Elite"
// Efecto: Espectro baja 5% HP actual del enemigo + 40% chance de paralisis
void aplicarGritoDelCaido(Monstruo& m, Personaje& /*p*/) {
    std::cout << "[ESPECTRO] De tu arma surge un espectro que lanza un grito desgarrador!" << std::endl;

    int dano = std::max(1, static_cast<int>(static_cast<float>(m.hp) * 0.05f));
    m.hp -= dano;
    std::cout << "[ESPECTRO] El grito arranca " << dano << " HP de " << m.nombre << "!" << std::endl;

    if (rand() % 100 < 40) {
        for (auto& ef : listaEfectos) {
            if (ef.id == 4) {
                m.efectosActivos.push_back(ef);
                std::cout << "[ESPECTRO] " << m.nombre << " queda paralizado por el terror!" << std::endl;
                break;
            }
        }
    }
}