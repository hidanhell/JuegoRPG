#include "loot.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Reliquias.h"
#include "Utilidades.h"
#include "catalogoObjetos.h"  
#include <iostream>
#include "Rng.h"
#include <vector>
#include <cmath>
#include "Consumibles.h"

using namespace std;

// =========================================================
// BLOQUE M2: LOOT Y MUNDO (v1.31)
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
    float m = raro ? 1.2f : 1.0f;

    if (sArma <= 40) {
        if (raro) cout << "\nUN BRILLO MISTICO EMANA DE LOS RESTOS" << endl;

        std::vector<Arma> posibles;
        for (const auto& arma : listaArmas) {
            if (arma.clase == p.clase && arma.zona == z) {
                if ((raro && arma.rareza == "Raro") || (!raro && arma.rareza == "Comun")) {
                    posibles.push_back(arma);
                }
            }
        }

        if (!posibles.empty()) {
            size_t index = static_cast<size_t>(Rng::get().entre(0, (int)posibles.size() - 1));
            nW = posibles[index];
            nW.poder = static_cast<int>(std::round(static_cast<float>(nW.poder) * m));

            if (raro) {
                nW.nombre += "+";
                nW.poder = static_cast<int>(static_cast<float>(nW.poder) * 1.15f + 0.5f);
            }

            if (nW.poder > p.armaEquipada.poder) {
                cout << "[!] HALLAZGO: " << nW.nombre 
                     << " (Poder: " << nW.poder << ") Equipar? (S/N): ";
                char c; cin >> c;
                if (tolower(c) == 's') p.armaEquipada = nW;
            }
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
        float mA = raro ? 1.2f : 1.0f;

        if (sArtefacto <= 30) {
            if (raro) cout << "\nUN DESTELLO SURGE ENTRE LOS RESTOS" << endl;

            std::vector<Artefacto> posibles;
            for (const auto& art : listaArtefactos) {
                if (art.clase == p.clase && art.zona == z) {
                    if ((raro && art.rareza == "Raro") || (!raro && art.rareza == "Comun")) {
                        posibles.push_back(art);
                    }
                }
            }

            if (!posibles.empty()) {
                int index = Rng::get().entre(0, (int)posibles.size() - 1);
                nA = posibles[static_cast<size_t>(index)];
                nA.defensa = static_cast<int>(static_cast<float>(nA.defensa) * mA);

                if (raro) {
                    nA.nombre += "+";
                    nA.defensa = static_cast<int>(static_cast<float>(nA.defensa) * 1.15f + 0.5f);
                }

                if (nA.defensa > p.artefactoEquipado.defensa) {
                    cout << "[!] HALLAZGO: " << nA.nombre 
                         << " (Def: +" << nA.defensa << ") Equipar? (S/N): ";
                    char c; cin >> c;
                    if (tolower(c) == 's') {
                        p.artefactoEquipado = nA;
                        cout << "Has equipado el nuevo artefacto.\n";
                    }
                } else {
                    cout << "[!] Artefacto encontrado (" << nA.nombre 
                         << ") descartado automaticamente.\n";
                }
            }
        }
    }

    // --- SISTEMA DE RELIQUIAS ---
    if (Rng::get().probabilidad(15) && p.reliquias.size() < 2) {

        // Filtramos reliquias que el jugador aún no tiene
        std::vector<Reliquia> posibles;
        for (const auto& rel : listaReliquias) {
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

            // Aplicar efectos inmediatos
            if (r.nombre == "Anillo del Asesino") {
                p.bonusCritico += 10;
                cout << "[BONUS] Critico +10%." << endl;
            } else if (r.nombre == "Medallon del Guardian") {
                p.defensaBase += 20;
                cout << "[BONUS] Defensa +20." << endl;
            } else if (r.nombre == "Garra del Berserker" && p.clase == "Guerrero") {
                p.fuerza += 15;
                p.ataqueBase += 15;
                cout << "[BONUS] Fuerza +15." << endl;
            } else if (r.nombre == "Ojo del Arcano" && p.clase == "Mago") {
                p.inteligencia += 15;
                p.ataqueBase += 15;
                cout << "[BONUS] Inteligencia +15." << endl;
            } else if (r.nombre == "Pluma del Viento" && p.clase == "Cazador") {
                p.destreza += 15;
                p.ataqueBase += 15;
                cout << "[BONUS] Destreza +15." << endl;
            } else if (r.nombre == "Botas del Relampago") {
                p.velocidadBase += 15;
                cout << "[BONUS] Velocidad +15." << endl;
            } else if (r.nombre == "Piedra del Alma") {
                cout << "[BONUS] Oro ganado por combate +25%." << endl;
                // El bonus se aplica en combate.cpp al calcular oroGanado
            } else if (r.nombre == "Amuleto de Sangre") {
                cout << "[BONUS] Activo: 20% de contraatacar al recibir dano." << endl;
                // La lógica de contraataque se aplica en combate.cpp
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