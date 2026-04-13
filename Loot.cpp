#include "loot.h"
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

// =========================================================
// BLOQUE M2: LOOT Y MUNDO (v1.18)
// =========================================================

void gestionarLoot(Personaje &p, int y, bool raro) {
    int z = (y <= 60) ? 1 : (y <= 90) ? 2 : (y <= 120) ? 3 : 4;

    // --- SISTEMA DE POCIONES (por zona + reliquias) ---
    if ((rand() % 100) < (raro ? 20 : 20)) { // 20% probabilidad para todos
        if (p.inventario.size() < 10u) {
            string pItem;
            int roll = rand() % 100;

            if (z == 1) pItem = "Pocion Baja";
            else if (z == 2) pItem = (roll < 60) ? "Pocion Baja" : "Pocion Media";
            else if (z == 3) pItem = (roll < 50) ? "Pocion Media" : "Pocion Alta";
            else {
                pItem = (roll < 70) ? "Pocion Alta" : "Pocion Media";
                if ((rand() % 100) < 15 && p.reliquias.size() < 2) {
                    vector<string> reliquiasPosibles = {
                        "Colmillo de Vampiro","Pluma del Fénix",
                        "Medallón del Guardián","Llave del Pueblo","Anillo del Asesino"
                    };
                    string reliquia = reliquiasPosibles[rand() % reliquiasPosibles.size()];
                    p.reliquias.push_back(reliquia);
                    cout << "[RELIQUIA] ¡Has encontrado " << reliquia << "!" << endl;
                    if (reliquia == "Anillo del Asesino") { p.bonusCritico += 5; cout << "[BONUS] Crítico +5%." << endl; }
                    else if (reliquia == "Medallón del Guardián") { p.defensaBase += 10; cout << "[BONUS] Defensa +10." << endl; }
                }
            }
            p.inventario.push_back(pItem);
            cout << "[LOOT] " << pItem << " obtenida." << endl;
        }
    }

    // --- SISTEMA DE ARMAS MEJORADO (Rarezas + Lore) ---
    int s = rand() % 100 + 1;
    Arma nW = {"-", 0};
    float m = raro ? 1.2f : 1.0f;

    if (s <= 40) { // 40% probabilidad para todos
        if (raro) cout << "\n¡UN BRILLO MISTICO EMANA DE LOS RESTOS!" << endl;

        if (z == 1) {
            if (raro) {
                if (p.clase == "Guerrero") nW = {"Espada de Hierro", (int)(30 * m)};
                else if (p.clase == "Mago") nW = {"Baculo de las Sombras", (int)(33 * m)};
                else nW = {"Arco Veloz", (int)(30 * m)};
            } else {
                if (p.clase == "Guerrero") nW = {"Espada Antigua", (int)(20 * m)};
                else if (p.clase == "Mago") nW = {"Baculo Maldito", (int)(22 * m)};
                else nW = {"Arco del Bandido", (int)(20 * m)};
            }
        }
        else if (z == 2) {
            if (raro) {
                if (p.clase == "Guerrero") nW = {"Sed de Sangre", (int)(42 * m)};
                else if (p.clase == "Mago") nW = {"Varita Negra", (int)(45 * m)};
                else nW = {"Colmillo de Fuego", (int)(43 * m)};
            } else {
                if (p.clase == "Guerrero") nW = {"Masamune", (int)(35 * m)};
                else if (p.clase == "Mago") nW = {"Baculo de Bruja Negra", (int)(37 * m)};
                else nW = {"Arco de Hueso", (int)(35 * m)};
            }
        }
        else if (z == 3) {
            if (raro) {
                if (p.clase == "Guerrero") nW = {"La Rompe Huesos", (int)(65 * m)};
                else if (p.clase == "Mago") nW = {"Varita del Oraculo Maldito", (int)(68 * m)};
                else nW = {"Colmillo de Sombra", (int)(64 * m)};
            } else {
                if (p.clase == "Guerrero") nW = {"Hacha del Verdugo", (int)(50 * m)};
                else if (p.clase == "Mago") nW = {"Cetro de la Cienaga", (int)(50 * m)};
                else nW = {"Arco del Acechador", (int)(48 * m)};
            }
        }
        else if (z == 4) {
            if (raro) {
                if (p.clase == "Guerrero") nW = {"Espada Acero de Elfo", (int)(110 * m)};
                else if (p.clase == "Mago") nW = {"Baculo del Nigromante", (int)(115 * m)};
                else nW = {"Colmillo de Dragon", (int)(113 * m)};
            } else {
                if (p.clase == "Guerrero") nW = {"Espada de Obsidiana", (int)(95 * m)};
                else if (p.clase == "Mago") nW = {"Baculo del Caos", (int)(105 * m)};
                else nW = {"Arco de Hueso de Dragon", (int)(98 * m)};
            }
        }

        if (raro) {
            nW.nombre += "+";
            nW.atk = (int)(nW.atk * 1.15f);
        }

        if (nW.atk > p.armaEquipada.atk) {
            cout << "[!] HALLAZGO: " << nW.nombre << " (Atk: " << nW.atk << ") ¿Equipar? (S/N): ";
            char c; cin >> c;
            if (tolower(c) == 's') p.armaEquipada = nW;
        }
    }
}

// --- Zonas del Juego ---
string obtenerNombreZona(int y) {
    if (y <= 60) return "Aldea en las Sombras";
    if (y <= 90) return "Bosque Obscuro";
    if (y <= 120) return "Pantano Siniestro";
    if (y < 150) return "Tierra Desolada";
    return "Castillo Abandonado";
}

// --- Lore del Mundo, diálogos mientras caminas ---
void lanzarDialogoAmbiental(int y) {
    if (rand() % 100 < 15) {
        cout << "\n[PENSAMIENTO] ";
        if (y <= 60) {
            string f[] = {"Las ventanas de la aldea estan selladas...", "Sientes ojos observandote tras las cortinas.", "El aire huele a madera vieja y miedo."};
            cout << f[rand()%3] << endl;
        } else if (y <= 90) {
            string f[] = {"El crujir de las ramas suena como huesos.", "Una niebla espesa cubre tus botas.", "Algo se movio rapido entre los pinos..."};
            cout << f[rand()%3] << endl;
        } else if (y <= 120) {
            string f[] = {"El lodo burbujea con un sonido extraño.", "Susurros lejanos parecen decir tu nombre...", "Cada paso pesa mas en este fango."};
            cout << f[rand()%3] << endl;
        } else if (y < 150) {
            string f[] = {"Ceniza volcanica cae del cielo gris.", "Un calor antinatural emana del suelo.", "El Castillo se alza imponente frente a ti."};
            cout << f[rand()%3] << endl;
        }
        system("pause");
    }
}
