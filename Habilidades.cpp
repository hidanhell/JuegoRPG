#include "Habilidades.h"
#include "Personajes.h"
#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <optional>
#include <algorithm>

extern std::vector<Habilidad> listaHabilidadesGlobal;
extern std::vector<Efecto> listaEfectos;

// --- Funciones auxiliares de cálculo de daño ---
inline int calcularDanoReal(int danoBase, int defensaEnemigo, bool ignoraParcial = false) {
    int defensaEfectiva = ignoraParcial ? defensaEnemigo / 2 : defensaEnemigo;
    return std::max(1, danoBase - defensaEfectiva);
}

inline void aplicarDanoSeguro(Monstruo &m, int danoReal) {
    m.hp -= danoReal;
    if (m.hp < 0) m.hp = 0;
}

// --- Función global de acceso ---
std::optional<Habilidad> obtenerHabilidadPorId(int id) {
    for (auto &h : listaHabilidadesGlobal) {
        if (h.id == id) return h;
    }
    return std::nullopt;
}

// ============================================
// EJECUCIÓN DE HABILIDADES DE PERSONAJES
// ============================================
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;
    int dano = 0;
    auto aplicarEstado = [&m](int idEfecto) {
        if (m.hp <= 0) return;
        if (auto efecto = obtenerEfectoPorId(idEfecto)) {
            agregarEfectoActivo(m.efectosActivos, *efecto);
        }
    };

    switch (hab.id) {

        // --- GUERRERO (Nivel 10) ---
        case 101: // Embate con Escudo
            dano = pj.ataqueBase + pj.armaEquipada.poder + static_cast<int>(pj.defensaBase * 1.4f);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(4);
            break;

        case 102: // Tajo Sangriento
            dano = pj.ataqueBase + pj.armaEquipada.poder + static_cast<int>(pj.fuerza * 1.3f);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(3);
            break;

        case 103: // Frenesí
        {
            int golpes = Rng::get().entre(2, 4);
            int danoTotal = 0;
            for (int i = 0; i < golpes; i++) {
                int golpe = static_cast<int>(pj.ataqueBase * 1.5f);
                if (i == 0) golpe += pj.armaEquipada.poder;
                int danoReal = calcularDanoReal(golpe, m.defensa);
                aplicarDanoSeguro(m, danoReal);
                danoTotal += danoReal;
                if (m.hp <= 0) break;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " realiza " << golpes
                      << " golpes e inflige un total de " << danoTotal << " de dano.\n";
            aplicarEstado(3);
        }
            break;

        // --- MAGO (Nivel 10) ---
        case 111: // Bola de Fuego
            dano = static_cast<int>(pj.inteligencia * 1.4f) + pj.armaEquipada.poder;
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(1);
            break;

        case 112: // Bola de Escarcha
            dano = static_cast<int>(pj.inteligencia * 1.4f) + pj.armaEquipada.poder;
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(6);
            break;

        case 113: // Rayo Arcano
            dano = static_cast<int>(pj.inteligencia * 1.4f) + pj.armaEquipada.poder;
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(7);
            break;

        // --- CAZADOR (Nivel 10) ---
        case 121: // Flecha Venenosa
            dano = static_cast<int>(pj.destreza * 1.4f) + pj.armaEquipada.poder;
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            aplicarEstado(2);
            break;

        case 122: // Disparo Preciso
            dano = static_cast<int>((pj.ataqueBase + pj.destreza + pj.armaEquipada.poder) * 1.5f);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige un golpe critico de " << dano << " de dano.\n";
            break;

        case 123: // Lluvia de Flechas
        {
            int impactos = Rng::get().entre(3, 5);
            int danoTotal = 0;
            for (int i = 0; i < impactos; i++) {
                if (m.hp <= 0) break;
                int golpe = static_cast<int>(pj.destreza * 1.4f);
                if (i == 0) golpe += pj.armaEquipada.poder;
                int danoReal = calcularDanoReal(golpe, m.defensa);
                aplicarDanoSeguro(m, danoReal);
                danoTotal += danoReal;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " lanza " << impactos
                      << " flechas e inflige un total de " << danoTotal << " de dano.\n";
            aplicarEstado(3);
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
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;

    if (pj.subclaseAplicada) {
        std::cout << "[SUBCLASE] Ya tienes una subclase activa.\n";
        return;
    }

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

    pj.subclaseAplicada = true;
    std::cout << "[SUBCLASE] " << hab.nombre << " activada: " << hab.descripcion << "\n";
}

// ============================================
// ULTIMATES NIVEL 20
// ============================================
int obtenerUltimateIdDePersonaje(const Personaje &pj) {
    for (int id : pj.habilidadesIds) {
        if (id >= 300 && id < 400) return id;
    }
    return -1;
}

bool puedeUsarUltimate(const Personaje &pj) {
    if (!pj.tieneUltimate20) return false;
    if (pj.ultimateUsada)    return false;
    const int idUltimate = obtenerUltimateIdDePersonaje(pj);
    if (idUltimate == -1) return false;
    if (pj.recursoActual < pj.recursoMax) return false;
    if (idUltimate == 331 && !pj.ultimoTurnoAtaco) return false;
    return true;
}

void ejecutarUltimate(Personaje &pj, Monstruo &m, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;

    switch (hab.id) {

        // --- GUERRERO (Ultimates Nivel 20) ---
        case 311: // Ira del Cielo (Paladín)
        {
            std::cout << "\n[ULTIMATE] ¡Invocas la ira de los cielos sobre " << m.nombre << "!\n";

            int danoBase = static_cast<int>((pj.fuerza * 2.0f) + (pj.vitalidad * 1.5f)) + pj.ataqueBase + pj.armaEquipada.poder;
            int danoReal = calcularDanoReal(danoBase, m.defensa);
            aplicarDanoSeguro(m, danoReal);

            std::cout << " [SAGRADO] ¡Un rayo de luz divina fulmina al enemigo causando " << danoReal << " de daño!\n";

            int curacion = static_cast<int>(pj.hpMax * 0.25);
            pj.hp = std::min(pj.hpMax, pj.hp + curacion);
            std::cout << " [CURACIÓN] La bendición divina restaura " << curacion << " HP de tu salud.\n";

            if (auto efParalisis = obtenerEfectoPorId(4)) {
                agregarEfectoActivo(m.efectosActivos, *efParalisis);
                std::cout << " [ESTADO] ¡El impacto sagrado deja paralizado al enemigo!\n";
            }
        }
            break;

        case 312: // Última Bestia (Berserker)
        {
            std::cout << "\n[ULTIMATE] ¡Entras en un frenesí bestial incontrolable y descargas una furia implacable!\n";

            int totalDano = 0;
            for (int i = 1; i <= 4; ++i) {
                if (m.hp <= 0) break;
                int danoGolpe = static_cast<int>(pj.fuerza * 0.8f) + (pj.ataqueBase / 2);
                if (i == 1) danoGolpe += pj.armaEquipada.poder;
                int danoRealGolpe = calcularDanoReal(danoGolpe, m.defensa, true);
                aplicarDanoSeguro(m, danoRealGolpe);
                totalDano += danoRealGolpe;
                std::cout << " -> [GOLPE " << i << "] Desgarras al enemigo por " << danoRealGolpe << " de daño.\n";
            }

            std::cout << " [FRENESÍ] Daño total infligido en la ráfaga: " << totalDano << ".\n";

            if (m.hp > 0) {
                int reduccionDef = static_cast<int>(m.defensa * 0.40);
                m.defensa = std::max(0, m.defensa - reduccionDef);
                m.turnosDebuffDefensa = 2;
                std::cout << " [RUPTURA] ¡La brutalidad del ataque redujo la armadura de " << m.nombre << " en " << reduccionDef << " puntos!\n";
            } else {
                std::cout << " [EJECUCIÓN] ¡Tu furia destrozó por completo al enemigo en el acto!\n";
            }
        }
            break;

        // --- MAGO (Ultimates Nivel 20) ---
        case 321: // Colapso Solar (Mago Fuego)
        {
            std::cout << "\n[ULTIMATE] ¡Canalizas el poder del sol y desatas un colapso cataclísmico sobre " << m.nombre << "!\n";

            int danoBase = static_cast<int>(pj.inteligencia * 3.5f) + pj.armaEquipada.poder;
            int danoReal = calcularDanoReal(danoBase, m.defensa);
            aplicarDanoSeguro(m, danoReal);
            std::cout << " [FUEGO] ¡Una implosión solar calcina al enemigo causando " << danoReal << " de daño masivo!\n";

            if (auto efQuemadura = obtenerEfectoPorId(1)) {
                agregarEfectoActivo(m.efectosActivos, *efQuemadura);
                std::cout << " [ESTADO] ¡El objetivo queda envuelto en llamas inextinguibles!\n";
            }

            pj.turnosAgotado = 1;
            std::cout << "[AGOTAMIENTO] El esfuerzo arcano te deja exhausto. No podrás usar habilidades el próximo turno.\n";
        }
            break;

        case 322: // Absoluto Cero (Mago Hielo)
        {
            std::cout << "\n[ULTIMATE] ¡La temperatura desciende al cero absoluto, congelando el tiempo y el espacio en torno a " << m.nombre << "!\n";

            int danoBase = static_cast<int>(pj.inteligencia * 2.8f) + pj.armaEquipada.poder;

            bool yaCongelado = false;
            for (const auto &ef : m.efectosActivos) {
                if (ef.id == 6) { yaCongelado = true; break; }
            }

            if (yaCongelado) {
                danoBase *= 2;
                std::cout << " [CRÍTICO HELADO] ¡El objetivo ya sufría los estragos del frío! El daño se duplica.\n";
            }

            int danoReal = calcularDanoReal(danoBase, m.defensa);
            aplicarDanoSeguro(m, danoReal);
            std::cout << " [HIELO] ¡Una ráfaga glacial fractura al enemigo infligiendo " << danoReal << " de daño!\n";

            if (auto efCongelacion = obtenerEfectoPorId(6)) {
                agregarEfectoActivo(m.efectosActivos, *efCongelacion);
                std::cout << " [ESTADO] ¡El enemigo queda totalmente congelado en un bloque de hielo!\n";
            }

            m.ataque = std::max(1, static_cast<int>(m.ataque * 0.80));
            std::cout << " [DEBUFF] ¡El frío entumece los músculos del enemigo, reduciendo su ataque un 20%!\n";
        }
            break;

        // --- CAZADOR (Ultimates Nivel 20) ---
        case 331: // Flecha del Juicio Final (Puntería)
        {
            if (!pj.ultimoTurnoAtaco) {
                std::cout << "\n[ULTIMATE] Necesitas haber atacado con tu ataque basico el turno "
                             "inmediatamente anterior para tensar tu arco al maximo. "
                             "No se cumple la condicion; la ultimate no se activa.\n";
                return;
            }

            std::cout << "\n[ULTIMATE] ¡Tensas tu arco al límite absoluto concentrando toda tu puntería en " << m.nombre << "!\n";

            int danoBase = static_cast<int>(pj.destreza * 3.2f) + pj.ataqueBase + pj.armaEquipada.poder;
            int danoReal = calcularDanoReal(danoBase, 0); // Ignora armadura por completo
            aplicarDanoSeguro(m, danoReal);
            std::cout << " [CRÍTICO LETAL] ¡La flecha atraviesa cualquier armadura infligiendo " << danoReal << " de daño puro!\n";
        }
            break;

        case 332: // Pacto de Sangre (Bestias)
        {
            std::cout << "\n[ULTIMATE] ¡Un silbido rasga el aire y convoca a tus tres bestias compañeras a la batalla!\n";

            int danoOso = static_cast<int>(pj.destreza * 1.5f) + pj.ataqueBase + pj.armaEquipada.poder;
            int danoRealOso = calcularDanoReal(danoOso, m.defensa);
            aplicarDanoSeguro(m, danoRealOso);
            std::cout << " -> [EL OSO] Carga con fuerza bruta e inflige " << danoRealOso << " de daño.\n";

            if (m.hp > 0) {
                int danoLobo = static_cast<int>(pj.destreza * 1.2);
                int danoRealLobo = calcularDanoReal(danoLobo, m.defensa);
                aplicarDanoSeguro(m, danoRealLobo);
                std::cout << " -> [EL LOBO] Salta y desgarra, infligiendo " << danoRealLobo << " de daño.\n";

                if (auto efSangrado = obtenerEfectoPorId(3)) {
                    agregarEfectoActivo(m.efectosActivos, *efSangrado);
                    std::cout << " [ESTADO] ¡El objetivo comienza a sangrar severamente!\n";
                }
            }

            if (m.hp > 0) {
                int danoLeon = static_cast<int>(pj.destreza * 1.3);
                int danoRealLeon = calcularDanoReal(danoLeon, m.defensa);
                aplicarDanoSeguro(m, danoRealLeon);
                std::cout << " -> [EL LEÓN] Muerde letalmente causando " << danoRealLeon << " de daño.\n";

                if (auto efVeneno = obtenerEfectoPorId(2)) {
                    agregarEfectoActivo(m.efectosActivos, *efVeneno);
                    std::cout << " [ESTADO] ¡El veneno de las bestias inunda las venas del enemigo!\n";
                }
            }

            pj.turnosEscudoCompanero = 1;
            std::cout << "[DEFENSA] ¡Tus bestias se ponen en posición defensiva para interceptar el próximo ataque enemigo!\n";
        }
            break;

        default:
            std::cout << "[ULTIMATE] " << hab.nombre << " no tiene logica definida aun.\n";
            return; 
    }

    pj.ultimateUsada = true;
}

// ============================================
// HABILIDADES DE ARMAS ELITE
// ============================================
void aplicarGritoDelCaido(Monstruo& m, Personaje& /*p*/) {
    std::cout << "[ESPECTRO] De tu arma surge un espectro que lanza un grito desgarrador!\n";

    int dano = std::max(1, static_cast<int>(static_cast<float>(m.hp) * 0.05f));
    aplicarDanoSeguro(m, dano);
    std::cout << "[ESPECTRO] El grito arranca " << dano << " HP de " << m.nombre << "!\n";

    if (Rng::get().entre(1, 100) <= 40) {
        if (auto ef = obtenerEfectoPorId(4)) {
            agregarEfectoActivo(m.efectosActivos, *ef);
            std::cout << "[ESPECTRO] " << m.nombre << " queda paralizado por el terror!\n";
        }
    }
}