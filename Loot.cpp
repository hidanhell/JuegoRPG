#include "loot.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Reliquias.h"
#include "Utilidades.h"
#include "catalogoObjetos.h"
#include "Habilidades.h"
#include <iostream>
#include "Rng.h"
#include <vector>
#include <cmath>
#include "Consumibles.h"

using namespace std;

// =========================================================
// BLOQUE M2: LOOT Y MUNDO (v1.33 - Filtros de Clase)
// =========================================================

void gestionarLoot(Personaje &p, int y, bool raro)
{
    int z = (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : 4;

    // --- SISTEMA DE CONSUMIBLES ---
    if (Rng::get().probabilidad(25)) {
        if (p.inventario.size() < 10u) {
            std::vector<Consumible> posibles;
            for (const auto& item : listaConsumibles) {
                if (item.id < 10 && item.zona == z) {
                    posibles.push_back(item);
                }
                else if (item.id >= 10 && raro && item.zona <= z) {
                    posibles.push_back(item);
                }
            }

            if (!posibles.empty()) {
                int index = Rng::get().entre(0, (int)posibles.size() - 1);
                Consumible lootObtenido = posibles[static_cast<size_t>(index)];
                p.inventario.push_back(lootObtenido);

                if (lootObtenido.id >= 10) {
                    cout << "[LOOT] ¡ELIXIR ESPECIAL! " << lootObtenido.nombre << " obtenido." << endl;
                    cout << " -> " << lootObtenido.descripcion << endl;
                } else {
                    cout << "[LOOT] " << lootObtenido.nombre << " obtenida." << endl;
                }
            }
        }
    }

    // --- SISTEMA DE ARMAS ---
    int sArma = Rng::get().entre(1, 100);
    Arma nW(0, "-", 0, "Ninguno", 0, "Ninguno", 0, 0); 

    if (sArma <= 40) {
        if (raro) cout << "\nUN BRILLO MISTICO EMANA DE LOS RESTOS" << endl;

        std::vector<Arma> posibles;
        for (const auto& arma : listaArmas) {
            if (arma.clase == p.clase && arma.zona == z) {
                if ((raro && arma.rareza == Rareza::Raro) || (!raro && arma.rareza == Rareza::Comun)) {
                    posibles.push_back(arma);
                }
            }
        }

        if (!posibles.empty()) {
            size_t index = static_cast<size_t>(Rng::get().entre(0, (int)posibles.size() - 1));
            nW = posibles[index];

            if (raro) {
                // Afijos dinámicos según el efecto que le tocó al arma
                if (nW.efectoId == 1) {
                    nW.nombre += " Llameante";
                } else if (nW.efectoId == 2) {
                    nW.nombre += " Tóxico/a";
                } else if (nW.efectoId == 3) {
                    nW.nombre += " Sangriento/a";
                } else if (nW.efectoId == 4) {
                    nW.nombre += " Paralizante";
                } else if (nW.efectoId == 5) {
                    nW.nombre += " Esquivo/a";
                } else {
                    nW.nombre += "+"; // Por si cae otro efecto
                }

                nW.poder = static_cast<int>(static_cast<float>(nW.poder) * 1.15f + 0.5f);
            }

            cout << "[!] HALLAZGO: " << nW.nombre
                 << " (Poder: " << nW.poder << ")"
                 << " [Equipada: " << p.armaEquipada.nombre
                 << " - " << p.armaEquipada.poder << "]" << endl;
            cout << "Equipar? (S/N): ";
            char c; cin >> c;
            if (tolower(c) == 's') p.armaEquipada = nW;
        }
    }

    // --- SISTEMA DE ARTEFACTOS ---
    if (p.esBerserker) {
        int sArtefacto = Rng::get().entre(1, 100);
        if (sArtefacto <= 30) {
            cout << "\n[!] Viste un artefacto entre los restos, pero como Berserker prefieres tus manos libres." << endl;
        }
    } 
    else { 
        int sArtefacto = Rng::get().entre(1, 100);
        Artefacto nA(0, "-", 0, "Ninguno", 0, "Ninguno", 0, 0);

        if (sArtefacto <= 30) {
            if (raro) cout << "\nUN DESTELLO SURGE ENTRE LOS RESTOS" << endl;

            std::vector<Artefacto> posibles;
            for (const auto& art : listaArtefactos) {
                if (art.clase == p.clase && art.zona == z) {
                    if ((raro && art.rareza == Rareza::Raro) || (!raro && art.rareza == Rareza::Comun)) {
                        posibles.push_back(art);
                    }
                }
            }

            if (!posibles.empty()) {
                int index = Rng::get().entre(0, (int)posibles.size() - 1);
                nA = posibles[static_cast<size_t>(index)];

                if (raro) {
                    nA.nombre += "+";
                    nA.defensa = static_cast<int>(static_cast<float>(nA.defensa) * 1.15f + 0.5f);
                }

                cout << "[!] HALLAZGO: " << nA.nombre
                     << " (Def: +" << nA.defensa << ")"
                     << " [Equipado: " << p.artefactoEquipado.nombre
                     << " - " << p.artefactoEquipado.defensa << "]" << endl;
                cout << "Equipar? (S/N): ";
                char c; cin >> c;
                if (tolower(c) == 's') {
                    p.artefactoEquipado = nA;
                    cout << "Has equipado el nuevo artefacto.\n";
                }
            }
        }
    }

    // --- SISTEMA DE RELIQUIAS ---
    if (Rng::get().probabilidad(15) && p.reliquias.size() < 2) {

        std::vector<Reliquia> posibles;
        for (const auto& rel : listaReliquias) {
            
            // Excluir la calavera del boss
            if (rel.nombre == "Calavera de Valdrame") continue;

            // --- FILTRO DE CLASE ---
            if ((rel.nombre == "Garra del Berserker" || rel.nombre == "Hombreras del Coloso") && p.clase != Clase::Guerrero) continue;
            if ((rel.nombre == "Ojo del Arcano" || rel.nombre == "Gema del Eco") && p.clase != Clase::Mago) continue;
            if ((rel.nombre == "Pluma del Viento" || rel.nombre == "Carcaj Inagotable") && p.clase != Clase::Cazador) continue;

            bool yaTiene = false;
            for (const auto& nombre : p.reliquias) {
                if (nombre == rel.nombre) { yaTiene = true; break; }
            }
            if (!yaTiene) posibles.push_back(rel);
        }

        if (!posibles.empty()) {
            Reliquia r = posibles[static_cast<size_t>(Rng::get().entre(0, (int)posibles.size() - 1))];
            p.reliquias.push_back(r.nombre);
            cout << "[RELIQUIA] Has encontrado " << r.nombre << "! (" << r.efecto << ")" << endl;

            // --- APLICAR ESTADISTICAS (Valores ajustados al Catalogo) ---
            if (r.nombre == "Anillo del Asesino") {
                p.bonusCritico += 5;
                cout << "[BONUS] Critico +5%." << endl;
            } else if (r.nombre == "Medallon del Guardian") {
                p.defensaBase += 10;
                cout << "[BONUS] Defensa +10." << endl;
            } else if (r.nombre == "Garra del Berserker" && p.clase == Clase::Guerrero) {
                p.fuerza += 8;
                p.ataqueBase += 8;
                cout << "[BONUS] Fuerza +8." << endl;
            } else if (r.nombre == "Ojo del Arcano" && p.clase == Clase::Mago) {
                p.inteligencia += 8;
                p.ataqueBase += 8;
                cout << "[BONUS] Inteligencia +8." << endl;
            } else if (r.nombre == "Pluma del Viento" && p.clase == Clase::Cazador) {
                p.destreza += 8;
                p.ataqueBase += 8;
                cout << "[BONUS] Destreza +8." << endl;
            } else if (r.nombre == "Botas del Relampago") {
                p.velocidadBase += 10;
                cout << "[BONUS] Velocidad +10." << endl;
            } else if (r.nombre == "Piedra del Alma") {
                cout << "[BONUS] Oro ganado por combate +25%." << endl;
            } else if (r.nombre == "Anillo de la Vitalidad") {
                p.hpMax += 50;
                p.hp += 50; 
                cout << "[BONUS] Salud Maxima +50." << endl;
            }
            // Las reliquias con efectos pasivos puros (Escudo del Destino, Corazon de Fenix, Amuleto) no suman stats aqui.
        }
    }
}

// =========================================================
// LOOT EXCLUSIVO DEL ARZOBISPO VALDRAME (v1.33)
// =========================================================
void gestionarLootValdrame(Personaje& p) {
    cout << "\n[LOOT ESPECIAL] Entre las cenizas del Arzobispo encuentras algo..." << endl;

    // --- ARMA ELITE (siempre cae, de tu clase) ---
    std::vector<Arma> posibles;
    for (const auto& arma : listaArmas) {
        if (arma.rareza == Rareza::Elite && arma.clase == p.clase) {
            posibles.push_back(arma);
        }
    }

    if (!posibles.empty()) {
        int index = Rng::get().entre(0, static_cast<int>(posibles.size()) - 1);
        Arma nW = posibles[static_cast<size_t>(index)];

        cout << "\n========================================" << endl;
        cout << "  *** ARMA ELITE: " << nW.nombre << " ***" << endl;
        cout << "  Poder  : " << nW.poder << endl;
        cout << "  Rareza : " << rarezaToString(nW.rareza) << std::endl;

        if (nW.efectoId != 0) {
            cout << "  Efecto : ID " << nW.efectoId << " (Paralisis al golpear)" << endl;
        }

        if (nW.habilidadId != 0) {
            Habilidad hab = obtenerHabilidadPorId(nW.habilidadId).value_or(Habilidad{0,"?","",0,0,"",0});
            cout << "  Habilidad especial [" << hab.nombre << "]:" << endl;
            cout << "    -> " << hab.descripcion << endl;
            cout << "    (Se activa automaticamente al realizar un golpe critico)" << endl;
        }

        cout << "  [Equipada: " << p.armaEquipada.nombre
             << " - Poder " << p.armaEquipada.poder << "]" << endl;
        cout << "========================================" << endl;

        cout << "Equipar? (S/N): ";
        char c; cin >> c;
        if (tolower(c) == 's') {
            p.armaEquipada = nW;
            cout << "[OK] " << nW.nombre << " equipada." << endl;
            
            if (nW.habilidadId != 0) {
                Habilidad hab = obtenerHabilidadPorId(nW.habilidadId).value_or(Habilidad{0,"?","",0,0,"",0});
                cout << "[HABILIDAD] " << hab.nombre << " ahora esta activa en tu arma." << endl;
            }
        }
    }

    // --- RELIQUIA: CALAVERA DE VALDRAME ---
    std::string calavera = "Calavera de Valdrame";
    bool yaTiene = false;
    for (const auto& r : p.reliquias) {
        if (r == calavera) { yaTiene = true; break; }
    }

    if (!yaTiene) {
        if (p.reliquias.size() < 2u) {
            p.reliquias.push_back(calavera);
            std::cout << "\n[RELIQUIA EPICA] Has obtenido la CALAVERA DE VALDRAME!" << std::endl;
            std::cout << " -> Señor de los Muertos: Absorbes 5% HP del enemigo cada turno." << std::endl;
        } else {
            std::cout << "\n[RELIQUIA EPICA] Has encontrado la CALAVERA DE VALDRAME!" << std::endl;
            std::cout << " -> Señor de los Muertos: Absorbes 5% HP del enemigo cada turno." << std::endl;
            std::cout << "\nYa tienes el máximo de reliquias equipadas." << std::endl;
            std::cout << "¿Quieres reemplazar una de tus reliquias actuales por la Calavera de Valdrame? (s/n): ";

            char opcion;
            std::cin >> opcion;
            if (opcion == 's' || opcion == 'S') {
                std::cout << "Elige cuál reliquia quieres reemplazar (Asegurate de restar las estadisticas base manualmente si es necesario):" << std::endl;
                for (size_t i = 0; i < p.reliquias.size(); ++i) {
                    std::cout << i+1 << ") " << p.reliquias[i] << std::endl;
                }
                size_t idx;
                std::cin >> idx;
                if (idx >= 1 && idx <= p.reliquias.size()) {
                    p.reliquias[idx-1] = calavera;
                    std::cout << "\nHas equipado la CALAVERA DE VALDRAME en lugar de tu reliquia anterior." << std::endl;
                } else {
                    std::cout << "Selección inválida. No se reemplazó ninguna reliquia." << std::endl;
                }
            } else {
                std::cout << "Has decidido no equipar la Calavera de Valdrame." << std::endl;
            }
        }
    }
}

// --- NOMBRES DE ZONA ---
string obtenerNombreZona(int y)
{
    if (y <= 60) return "Aldea en las Sombras";
    if (y <= 120) return "Bosque Obscuro";
    if (y <= 180) return "Pantano Siniestro";
    if (y <= 240) return "Tierra Desolada";
    return "Castillo Abandonado";
}

// --- DIÁLOGOS AMBIENTALES ---
void lanzarDialogoAmbiental(int y) {
    if (Rng::get().probabilidad(15)) {
        cout << "\n[PENSAMIENTO] ";
        if (y <= 60) {
            string f[] = {"Las ventanas de la aldea estan selladas...", "Sientes ojos observandote tras las cortinas.", "El aire huele a madera vieja y miedo."};
            cout << f[Rng::get().entre(0, 2)] << endl;
        } else if (y <= 120) {
            string f[] = {"El crujir de las ramas suena como huesos.", "Una niebla espesa cubre tus botas.", "Algo se movio rapido entre los pinos..."};
            cout << f[Rng::get().entre(0, 2)] << endl;
        } else if (y <= 180) {
            string f[] = {"El lodo burbujea con un sonido extraño.", "Susurros lejanos parecen decir tu nombre...", "Cada paso pesa mas en este fango."};
            cout << f[Rng::get().entre(0, 2)] << endl;
        } else if (y <= 240) {
            string f[] = {"Ceniza volcanica cae del cielo gris.", "Un calor antinatural emana del suelo.", "El Castillo se alza imponente frente a ti."};
            cout << f[Rng::get().entre(0, 2)] << endl;
        } else {
            string f[] = {
                "El eco de tus pasos retumba en los salones vacios.",
                "Las estatuas de dragones parecen seguirte con la mirada.",
                "Sientes una presion inmensa en el pecho... el Rey esta cerca.",
                "El frio del castillo cala hasta tus huesos, a pesar del fuego exterior."
            };
            cout << f[Rng::get().entre(0, 3)] << endl;
        }
        esperarTecla();
    }
}