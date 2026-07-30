#include "Habilidades.h"
#include "Personajes.h"
#include "Efectos.h"
#include "Rng.h"
#include <iostream>
#include <optional>
#include <algorithm> // FIX: Necesario para std::max y std::min

extern std::vector<Habilidad> listaHabilidadesGlobal;
// CORRECCIÓN: listaEfectos no estaba declarada como extern en este archivo
extern std::vector<Efecto> listaEfectos;

// ==========================
// FIX: Funciones de ayuda para no repetir logica y evitar HP negativo
// No borran nada de lo tuyo, solo centralizan
// ==========================
inline int calcularDanoReal(int danoBase, int defensaEnemigo, bool ignoraParcial = false) {
    int defensaEfectiva = ignoraParcial ? defensaEnemigo / 2 : defensaEnemigo;
    return std::max(1, danoBase - defensaEfectiva);
}
inline void aplicarDanoSeguro(Monstruo &m, int danoReal) {
    m.hp -= danoReal;
    if (m.hp < 0) m.hp = 0; // FIX: Evita HP en -500 que rompe el 5% de Grito del Caido
}

// ==========================
// FUNCIÓN GLOBAL DE ACCESO
// ==========================
std::optional<Habilidad> obtenerHabilidadPorId(int id) {
    // NOTA TUYA: lo dejamos con for para que siga simple. 
    // A futuro, si tienes +300 habilidades, cambialo a unordered_map para que sea mas rapido.
    for (auto &h : listaHabilidadesGlobal)
        if (h.id == id) return h;
    return std::nullopt;
}

// ============================================
// EJECUCIÓN DE HABILIDADES PERSONAJES
// ============================================
void ejecutarHabilidad(Personaje &pj, Monstruo &m, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;
    int dano = 0;
    bool turnoPerdido = false;

    switch (hab.id) {

        // -----------------------------------------------
        // GUERRERO — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 101: // Embate con Escudo
            dano = pj.ataqueBase + static_cast<int>(pj.defensaBase * 1.4);
            // FIX: Ahora si resta defensa, antes pegaba siempre full
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(4)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 102: // Tajo Sangriento
            dano = pj.ataqueBase + static_cast<int>(pj.fuerza * 1.3);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 103: // Frenesí
        {
            int golpes = Rng::get().entre(2, 4);
            int danoTotal = 0;
            for (int i = 0; i < golpes; i++) {
                int golpe = static_cast<int>(pj.ataqueBase * 1.5);
                // FIX: Cada golpe ahora resta defensa y se clampe a 0
                int danoReal = calcularDanoReal(golpe, m.defensa);
                aplicarDanoSeguro(m, danoReal);
                danoTotal += danoReal;
                if (m.hp <= 0) break; // FIX: Si ya murio, no sigue pegando al aire
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " realiza " << golpes
                      << " golpes e inflige un total de " << danoTotal << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
        }
            break;

        // -----------------------------------------------
        // MAGO — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 111: // Bola de Fuego
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(1)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 112: // Bola de Escarcha
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(6)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 113: // Rayo Arcano
            dano = pj.inteligencia + static_cast<int>(pj.inteligencia * 1.4);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(7)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        // -----------------------------------------------
        // CAZADOR — ACTIVAS NIVEL 10
        // -----------------------------------------------

        case 121: // Flecha Venenosa
            dano = pj.destreza + static_cast<int>(pj.destreza * 1.4);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige " << dano << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(2)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
            break;

        case 122: // Disparo Preciso
            dano = pj.ataqueBase + static_cast<int>(pj.destreza * 1.5);
            aplicarDanoSeguro(m, calcularDanoReal(dano, m.defensa));
            std::cout << "[HABILIDAD] " << hab.nombre << " inflige un golpe critico de " << dano << " de dano.\n";
            break;

        case 123: // Lluvia de Flechas
        {
            int impactos = Rng::get().entre(3, 5);
            int danoTotal = 0;
            for (int i = 0; i < impactos; i++) {
                if (m.hp <= 0) break;
                int golpe = static_cast<int>(pj.destreza * 1.4);
                int danoReal = calcularDanoReal(golpe, m.defensa);
                aplicarDanoSeguro(m, danoReal);
                danoTotal += danoReal;
            }
            std::cout << "[HABILIDAD] " << hab.nombre << " lanza " << impactos
                      << " flechas e inflige un total de " << danoTotal << " de dano.\n";
            if (auto ef = obtenerEfectoPorId(3)) ef->aplicar(m.hp, m.hpMax, turnoPerdido);
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

    // Evita que se stackee si por error se llama 2 veces al elegir subclase.
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

    pj.subclaseAplicada = true; // Marca que ya se usó la subclase.
    std::cout << "[SUBCLASE] " << hab.nombre << " activada: " << hab.descripcion << "\n";
}

// ============================================
// ULTIMATES NIVEL 20 (una sola vez por partida)
// ============================================
// Estas son ACTIVAS de combate igual que ejecutarHabilidad, pero
// se separan en su propia funcion porque tienen reglas distintas:
// solo se pueden usar 1 vez por partida y (a futuro) consumen
// todo el recurso de mana/ira/enfoque del personaje. Por eso NO
// van dentro de aplicarSubclase (que solo corre una vez, al elegir
// subclase en nivel 15) ni mezcladas en el switch de habilidades
// normales (que se pueden repetir libremente).

// Busca en habilidadesIds cual ultimate (rango 300-399) aprendio
// el personaje segun la subclase que eligio. Devuelve -1 si no
// tiene ninguna todavia.
int obtenerUltimateIdDePersonaje(const Personaje &pj) {
    for (int id : pj.habilidadesIds) {
        if (id >= 300 && id < 400) return id;
    }
    return -1;
}

// Valida si el personaje puede usar su ultimate ahora mismo.
// Hoy solo revisa "desbloqueada y no gastada". Cuando implementes
// mana/ira/enfoque, agrega aqui el chequeo de recurso, por ejemplo:
//   if (pj.manaActual < pj.manaMax) return false;
bool puedeUsarUltimate(const Personaje &pj) {
    if (!pj.tieneUltimate20)      return false;
    if (pj.ultimateUsada)         return false;
    if (obtenerUltimateIdDePersonaje(pj) == -1) return false;
    return true;
}

void ejecutarUltimate(Personaje &pj, Monstruo &m, int idHabilidad) {
    auto habOpt = obtenerHabilidadPorId(idHabilidad);
    if (!habOpt) return;
    Habilidad hab = *habOpt;

    switch (hab.id) {

        // -----------------------------------------------
        // GUERRERO — ULTIMATES NIVEL 20
        // -----------------------------------------------

        case 311: // Ira del Cielo (Ultimate de Paladín)
        {
            std::cout << "\n[ULTIMATE] ¡Invocas la ira de los cielos sobre " << m.nombre << "!\n";

            // Daño basado en Fuerza y Vitalidad del Guerrero
            int danoBase = static_cast<int>((pj.fuerza * 2.0) + (pj.vitalidad * 1.5)) + pj.ataqueBase;
            int danoReal = calcularDanoReal(danoBase, m.defensa); // FIX: usa la misma funcion que las demas
            aplicarDanoSeguro(m, danoReal);

            std::cout << " [SAGRADO] ¡Un rayo de luz divina fulmina al enemigo causando " << danoReal << " de daño!\n";

            // Autocura masiva como recompensa defensiva
            int curacion = static_cast<int>(pj.hpMax * 0.25);
            pj.hp = std::min(pj.hpMax, pj.hp + curacion);
            std::cout << " [CURACIÓN] La bendición divina restaura " << curacion << " HP de tu salud.\n";

            // Aplicar Parálisis garantizada (Efecto ID 4) por el impacto sagrado
            auto efParalisis = obtenerEfectoPorId(4);
            if (efParalisis) {
                m.efectosActivos.push_back(*efParalisis);
                std::cout << " [ESTADO] ¡El impacto sagrado deja paralizado al enemigo!\n";
            }
        }
            break;

        case 312: // Última Bestia (Ultimate de Berserker)
        {
            std::cout << "\n[ULTIMATE] ¡Entras en un frenesí bestial incontrolable y descargas una furia implacable!\n";

            // Simulación de 4 golpes rápidos consecutivos
            int totalDano = 0;
            for (int i = 1; i <= 4; ++i) {
                if (m.hp <= 0) break;
                int danoGolpe = static_cast<int>(pj.fuerza * 0.8) + (pj.ataqueBase / 2);
                int danoRealGolpe = calcularDanoReal(danoGolpe, m.defensa, true); // FIX: Penetra parcialmente la defensa, usando helper
                aplicarDanoSeguro(m, danoRealGolpe);
                totalDano += danoRealGolpe;
                std::cout << " -> [GOLPE " << i << "] Desgarras al enemigo por " << danoRealGolpe << " de daño.\n";
            }

            std::cout << " [FRENESÍ] Daño total infligido en la ráfaga: " << totalDano << ".\n";

            // Mecánica de castigo/ruptura si el enemigo sobrevive
            if (m.hp > 0) {
                // Rompe un 40% de la defensa actual del enemigo
                int reduccionDef = static_cast<int>(m.defensa * 0.40);
                m.defensa = std::max(0, m.defensa - reduccionDef);
                m.turnosDebuffDefensa = 2; // El enemigo queda con armadura rota por 2 turnos

                std::cout << " [RUPTURA] ¡La brutalidad del ataque redujo la armadura de " << m.nombre << " en " << reduccionDef << " puntos!\n";
            } else {
                std::cout << " [EJECUCIÓN] ¡Tu furia destrozó por completo al enemigo en el acto!\n";
            }
        }
            break;

        // -----------------------------------------------
        // MAGO — ULTIMATES NIVEL 20
        // -----------------------------------------------

        case 321: // Colapso Solar (Ultimate de Mago Fuego)
        {
            std::cout << "\n[ULTIMATE] ¡Canalizas el poder del sol y desatas un colapso cataclísmico sobre " << m.nombre << "!\n";
            
            // Daño masivo basado fuertemente en Inteligencia (ej. Inteligencia x 3.5)
            int danoBase = static_cast<int>(pj.inteligencia * 3.5);
            int danoReal = calcularDanoReal(danoBase, m.defensa);
            aplicarDanoSeguro(m, danoReal);

            std::cout << " [FUEGO] ¡Una implosión solar calcina al enemigo causando " << danoReal << " de daño masivo!\n";

            // Aplica Quemadura garantizada (Efecto ID 1) por 3 turnos
            auto efQuemadura = obtenerEfectoPorId(1);
            if (efQuemadura) {
                m.efectosActivos.push_back(*efQuemadura);
                std::cout << " [ESTADO] ¡El objetivo queda envuelto en llamas inextinguibles!\n";
            }

            // Mecánica de castigo: El mago queda "agotado" el próximo turno
            pj.turnosAgotado = 1;
            std::cout << "[AGOTAMIENTO] El esfuerzo arcano te deja exhausto. No podrás usar habilidades el próximo turno.\n";
        }
            break;

        case 322: // Absoluto Cero (Ultimate de Mago Hielo)
        {
            std::cout << "\n[ULTIMATE] ¡La temperatura desciende al cero absoluto, congelando el tiempo y el espacio en torno a " << m.nombre << "!\n";
            
            // Daño base de hielo
            int danoBase = static_cast<int>(pj.inteligencia * 2.8);
            
            // Si el enemigo ya estaba congelado o con escarcha (buscando ID 6), el daño se duplica
            bool yaCongelado = false;
            for (const auto &ef : m.efectosActivos) {
                if (ef.id == 6) {
                    yaCongelado = true;
                    break;
                }
            }

            if (yaCongelado) {
                danoBase *= 2;
                std::cout << " [CRÍTICO HELADO] ¡El objetivo ya sufría los estragos del frío! El daño se duplica.\n";
            }

            int danoReal = calcularDanoReal(danoBase, m.defensa);
            aplicarDanoSeguro(m, danoReal);

            std::cout << " [HIELO] ¡Una ráfaga glacial fractura al enemigo infligiendo " << danoReal << " de daño!\n";

            // Aplica Congelación garantizada (Efecto ID 6)
            auto efCongelacion = obtenerEfectoPorId(6);
            if (efCongelacion) {
                m.efectosActivos.push_back(*efCongelacion);
                std::cout << " [ESTADO] ¡El enemigo queda totalmente congelado en un bloque de hielo!\n";
            }

            // Debuff de ataque al enemigo por el frío extremo
            m.ataque = std::max(1, static_cast<int>(m.ataque * 0.80));
            std::cout << " [DEBUFF] ¡El frío entumece los músculos del enemigo, reduciendo su ataque un 20%!\n";
        }
            break;

        // -----------------------------------------------
        // CAZADOR — ULTIMATES NIVEL 20
        // -----------------------------------------------

        case 331: // Flecha del Juicio Final (Ultimate de Puntería)
        {
            std::cout << "\n[ULTIMATE] ¡Tensas tu arco al límite absoluto concentrando toda tu puntería en " << m.nombre << "!\n";
            
            // Daño masivo basado en destreza que IGNORA por completo la defensa del enemigo (defensa = 0 en el cálculo)
            int danoBase = static_cast<int>(pj.destreza * 3.2) + pj.ataqueBase;
            int danoReal = calcularDanoReal(danoBase, 0); // Pasa 0 en defensa para ignorarla por completo
            aplicarDanoSeguro(m, danoReal);

            std::cout << " [CRÍTICO LETAL] ¡La flecha atraviesa cualquier armadura infligiendo " << danoReal << " de daño puro!\n";
        }
            break;

        case 332: // Pacto de Sangre (Ultimate de Bestias)
        {
            std::cout << "\n[ULTIMATE] ¡Un silbido rasga el aire y convoca a tus tres bestias compañeras a la batalla!\n";
            
            // 1. El Oso: Daño físico bruto basado en destreza y ataque
            int danoOso = static_cast<int>(pj.destreza * 1.5) + pj.ataqueBase;
            int danoRealOso = calcularDanoReal(danoOso, m.defensa);
            aplicarDanoSeguro(m, danoRealOso);
            std::cout << " -> [EL OSO] Carga con fuerza bruta e inflige " << danoRealOso << " de daño.\n";

            // 2. El Lobo: Daño ágil y aplicación de Sangrado (Efecto ID 3)
            if (m.hp > 0) {
                int danoLobo = static_cast<int>(pj.destreza * 1.2);
                int danoRealLobo = calcularDanoReal(danoLobo, m.defensa);
                aplicarDanoSeguro(m, danoRealLobo);
                std::cout << " -> [EL LOBO] Salta y desgarra, infligiendo " << danoRealLobo << " de daño.\n";
                
                auto efSangrado = obtenerEfectoPorId(3); // Sangrado
                if (efSangrado) {
                    m.efectosActivos.push_back(*efSangrado);
                    std::cout << " [ESTADO] ¡El objetivo comienza a sangrar severamente!\n";
                }
            }

            // 3. El León: Remate feroz y aplicación de Veneno (Efecto ID 2)
            if (m.hp > 0) {
                int danoLeon = static_cast<int>(pj.destreza * 1.3);
                int danoRealLeon = calcularDanoReal(danoLeon, m.defensa);
                aplicarDanoSeguro(m, danoRealLeon);
                std::cout << " -> [EL LEÓN] Muerde letalmente causando " << danoRealLeon << " de daño.\n";
                
                auto efVeneno = obtenerEfectoPorId(2); // Veneno
                if (efVeneno) {
                    m.efectosActivos.push_back(*efVeneno);
                    std::cout << " [ESTADO] ¡El veneno de las bestias inunda las venas del enemigo!\n";
                }
            }

            // Activación del escudo de la bestia para proteger al jugador el próximo turno
            pj.turnosEscudoCompanero = 1;
            std::cout << "[DEFENSA] ¡Tus bestias se ponen en posición defensiva para interceptar el próximo ataque enemigo!\n";
        }
            break;

        default:
            std::cout << "[ULTIMATE] " << hab.nombre << " no tiene logica definida aun.\n";
            return; // no se marca como usada si no existia logica
    }

    // Se gasta el unico uso por partida, sin importar cual ultimate fue.
    pj.ultimateUsada = true;
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
    aplicarDanoSeguro(m, dano); // FIX: Usa helper para no dejar hp negativo
    std::cout << "[ESPECTRO] El grito arranca " << dano << " HP de " << m.nombre << "!" << std::endl;

    // El efecto de paralisis del espectro usa el RNG central del proyecto.
    if (Rng::get().entre(1, 100) <= 40) {
        if (auto ef = obtenerEfectoPorId(4)) {
            m.efectosActivos.push_back(*ef);
            std::cout << "[ESPECTRO] " << m.nombre << " queda paralizado por el terror!" << std::endl;
        }
    }
}