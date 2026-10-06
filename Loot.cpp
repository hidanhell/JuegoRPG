#include "Loot.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Reliquias.h"
#include "Utilidades.h"
#include "CatalogoObjetos.h"
#include "Habilidades.h"
#include "Consumibles.h"
#include "Rng.h"
#include "IO.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// =========================================================
// SISTEMA GENERAL DE LOOT Y BOTÍN
// =========================================================

void gestionarLoot(Personaje &p, int y, bool raro)
{
    int z = (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : 4;

    // --- CONSUMIBLES ---
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
                int index = Rng::get().entre(0, static_cast<int>(posibles.size()) - 1);
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

    // --- ARMAS ---
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
            size_t index = static_cast<size_t>(Rng::get().entre(0, static_cast<int>(posibles.size()) - 1));
            nW = posibles[index];

            if (raro) {
                if (nW.efectoId == 1)      nW.nombre += " Llameante";
                else if (nW.efectoId == 2) nW.nombre += " Tóxico/a";
                else if (nW.efectoId == 3) nW.nombre += " Sangriento/a";
                else if (nW.efectoId == 4) nW.nombre += " Paralizante";
                else if (nW.efectoId == 5) nW.nombre += " Esquivo/a";
                else                       nW.nombre += "+";

                nW.poder = static_cast<int>(static_cast<float>(nW.poder) * 1.15f + 0.5f);
            }

              const int diferenciaArma = nW.poder - p.armaEquipada.poder;
              cout << "[!] HALLAZGO: " << nW.nombre
                 << " (Poder: " << nW.poder << ")"
                 << " [Equipada: " << p.armaEquipada.nombre
                  << " - " << p.armaEquipada.poder << "] ";
              if (diferenciaArma > 0) cout << "[MEJORA +" << diferenciaArma << " ATQ]" << endl;
              else if (diferenciaArma < 0) cout << "[INFERIOR " << diferenciaArma << " ATQ]" << endl;
              else cout << "[MISMO PODER]" << endl;
            cout << "Equipar? (S/N): ";
            char c = IO::entrada().leerTecla();
            if (tolower(c) == 's') p.armaEquipada = nW;
        }
    }

    // --- ARTEFACTOS ---
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
                int index = Rng::get().entre(0, static_cast<int>(posibles.size()) - 1);
                nA = posibles[static_cast<size_t>(index)];

                if (raro) {
                    nA.nombre += "+";
                    nA.defensa = static_cast<int>(static_cast<float>(nA.defensa) * 1.15f + 0.5f);
                }

                 const int diferenciaArtefacto = nA.defensa - p.artefactoEquipado.defensa;
                 cout << "[!] HALLAZGO: " << nA.nombre
                     << " (Def: +" << nA.defensa << ")"
                     << " [Equipado: " << p.artefactoEquipado.nombre
                     << " - " << p.artefactoEquipado.defensa << "] ";
                 if (diferenciaArtefacto > 0) cout << "[MEJORA +" << diferenciaArtefacto << " DEF]" << endl;
                 else if (diferenciaArtefacto < 0) cout << "[INFERIOR " << diferenciaArtefacto << " DEF]" << endl;
                 else cout << "[MISMA DEFENSA]" << endl;
                cout << "Equipar? (S/N): ";
                char c = IO::entrada().leerTecla();
                if (tolower(c) == 's') {
                    p.artefactoEquipado = nA;
                    cout << "Has equipado el nuevo artefacto.\n";
                }
            }
        }
    }

    // --- RELIQUIAS ---
    if (Rng::get().probabilidad(15) && p.reliquias.size() < 2) {
        std::vector<Reliquia> posibles;
        for (const auto& rel : listaReliquias) {
            if (rel.nombre == "Calavera de Valdrame") continue;
            if (!reliquiaCompatibleConClase(rel, p.clase)) continue;

            bool yaTiene = false;
            for (const auto& nombre : p.reliquias) {
                if (nombre == rel.nombre) { yaTiene = true; break; }
            }
            if (!yaTiene) posibles.push_back(rel);
        }

        if (!posibles.empty()) {
            Reliquia r = posibles[static_cast<size_t>(Rng::get().entre(0, static_cast<int>(posibles.size()) - 1))];
            p.reliquias.push_back(r.nombre);
            cout << "[RELIQUIA] Has encontrado " << r.nombre << "! (" << r.efecto << ")" << endl;
            if (aplicarBonosReliquia(p, r)) cout << "[BONUS] Bono permanente aplicado." << endl;
        }
    }
}

// =========================================================
// LOOT EXCLUSIVO DEL ARZOBISPO VALDRAME
// =========================================================
void gestionarLootValdrame(Personaje& p) {
    cout << "\n[LOOT ESPECIAL] Entre las cenizas del Arzobispo encuentras algo..." << endl;

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
        char c = IO::entrada().leerTecla();
        if (tolower(c) == 's') {
            p.armaEquipada = nW;
            cout << "[OK] " << nW.nombre << " equipada." << endl;
            
            if (nW.habilidadId != 0) {
                Habilidad hab = obtenerHabilidadPorId(nW.habilidadId).value_or(Habilidad{0,"?","",0,0,"",0});
                cout << "[HABILIDAD] " << hab.nombre << " ahora esta activa en tu arma." << endl;
            }
        }
    }

    // --- CALAVERA DE VALDRAME ---
    std::string calavera = "Calavera de Valdrame";
    bool yaTiene = false;
    for (const auto& r : p.reliquias) {
        if (r == calavera) { yaTiene = true; break; }
    }

    if (!yaTiene) {
        if (p.reliquias.size() < 2u) {
            p.reliquias.push_back(calavera);
            std::cout << "\n[RELIQUIA EPICA] Has obtenido la CALAVERA DE VALDRAME!\n";
            std::cout << " -> Señor de los Muertos: Absorbes 5% HP del enemigo cada turno.\n";
        } else {
            std::cout << "\n[RELIQUIA EPICA] Has encontrado la CALAVERA DE VALDRAME!\n";
            std::cout << " -> Señor de los Muertos: Absorbes 5% HP del enemigo cada turno.\n";
            std::cout << "\nYa tienes el máximo de reliquias equipadas.\n";
            std::cout << "¿Quieres reemplazar una de tus reliquias actuales por la Calavera de Valdrame? (s/n): ";

            char opcion = IO::entrada().leerTecla();
            if (tolower(opcion) == 's') {
                std::cout << "Elige cuál reliquia quieres reemplazar:\n";
                for (size_t i = 0; i < p.reliquias.size(); ++i) {
                    std::cout << i + 1 << ") " << p.reliquias[i] << "\n";
                }
                int idx = IO::entrada().leerOpcion();
                if (idx >= 1 && static_cast<size_t>(idx) <= p.reliquias.size()) {
                    p.reliquias[static_cast<size_t>(idx - 1)] = calavera;
                    std::cout << "\nHas equipado la CALAVERA DE VALDRAME en lugar de tu reliquia anterior.\n";
                } else {
                    std::cout << "Selección inválida. No se reemplazó ninguna reliquia.\n";
                }
            } else {
                std::cout << "Has decidido no equipar la Calavera de Valdrame.\n";
            }
        }
    }
}

// =========================================================
// MUNDO Y AMBIENTACIÓN
// =========================================================

string obtenerNombreZona(int y) {
    if (y <= 60)  return "Aldea en las Sombras";
    if (y <= 120) return "Bosque Obscuro";
    if (y <= 180) return "Pantano Siniestro";
    if (y <= 240) return "Tierra Desolada";
    return "Castillo Abandonado";
}

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