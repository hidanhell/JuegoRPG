#include "tienda.h"
#include "utilidades.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Reliquias.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "catalogoObjetos.h"
#include "Consumibles.h"
#include "Rng.h"

using namespace std;

// =========================================================
// BLOQUE M3: SISTEMA DE TIENDA (v2.1)
// =========================================================

void entrarTienda(Personaje &p) {
    bool enMenu = true;

    while (enMenu) {
        limpiarPantalla();
        mostrarCabecera("MERCADER DE LA CIUDADELA");

        cout << "Oro disponible: " << p.oro 
             << " | Bolsa: " << p.inventario.size() << "/10 consumibles"
             << " | Reliquias: " << p.reliquias.size() << "/2" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Que deseas hacer?" << endl;
        cout << "1. Comprar" << endl;
        cout << "2. Vender" << endl;
        cout << "3. Salir" << endl;
        cout << "----------------------------------------" << endl;

        int opcion; cin >> opcion;

        if (opcion == 1) { // --- COMPRAR ---
            cout << "\n--- CATEGORIAS DE COMPRA ---" << endl;
            cout << "1. Consumibles (Pociones y Elixires)" << endl;
            cout << "2. Armas y Artefactos" << endl;
            cout << "3. Reliquias" << endl;
            cout << "0. Cancelar" << endl;

            int categoria; cin >> categoria;

            if (categoria == 1) { // --- CONSUMIBLES ---
                cout << "\n--- BOTICA DE LA CIUDADELA ---" << endl;
                vector<Consumible> stockTienda;
                int idxItem = 1;

                for (const auto& item : listaConsumibles) {
                    cout << idxItem << ". " << item.nombre 
                         << " (" << item.precio << " oro) - " << item.descripcion << endl;
                    stockTienda.push_back(item);
                    idxItem++;
                }
                cout << "0. Cancelar" << endl;

                int seleccion; cin >> seleccion;
                if (seleccion > 0 && seleccion <= (int)stockTienda.size()) {
                    Consumible elegido = stockTienda[static_cast<size_t>(seleccion - 1)];

                    if (p.oro >= elegido.precio && p.inventario.size() < 10) {
                        p.oro -= elegido.precio;
                        p.inventario.push_back(elegido);
                        cout << "-Compraste- " << elegido.nombre << "-" << endl;
                    } else {
                        cout << "Oro insuficiente o inventario lleno." << endl;
                    }
                }
            }
            else if (categoria == 2) { // --- ARMAS Y ARTEFACTOS ---
                cout << "\n--- ARMAS Y ARTEFACTOS ---" << endl;

                vector<Arma> poolArmas;
                vector<Artefacto> poolArt;
                int idxCompra = 1;

                for (auto &a : listaArmas) if (a.clase == p.clase) poolArmas.push_back(a);
                for (auto &a : listaArtefactos) if (a.clase == p.clase) poolArt.push_back(a);

                // Centralizamos la aleatoriedad de la tienda en el RNG único del proyecto.
                std::vector<Arma> poolArmasSeleccionadas;
                std::vector<Artefacto> poolArtSeleccionadas;

                for (int i = 0; i < 2 && !poolArmas.empty(); ++i) {
                    int idx = Rng::get().entre(0, static_cast<int>(poolArmas.size()) - 1);
                    poolArmasSeleccionadas.push_back(poolArmas[static_cast<size_t>(idx)]);
                    poolArmas.erase(poolArmas.begin() + idx);
                }
                for (int i = 0; i < 2 && !poolArt.empty(); ++i) {
                    int idx = Rng::get().entre(0, static_cast<int>(poolArt.size()) - 1);
                    poolArtSeleccionadas.push_back(poolArt[static_cast<size_t>(idx)]);
                    poolArt.erase(poolArt.begin() + idx);
                }

                for (int i = 0; i < (int)poolArmasSeleccionadas.size(); i++) {
                    cout << idxCompra << ". " << poolArmasSeleccionadas[static_cast<size_t>(i)].nombre
                         << " [Atk +" << poolArmasSeleccionadas[static_cast<size_t>(i)].poder << "] (" 
                         << poolArmasSeleccionadas[static_cast<size_t>(i)].precio << " oro)" << endl;
                    idxCompra++;
                }
                for (int i = 0; i < (int)poolArtSeleccionadas.size(); i++) {
                    cout << idxCompra << ". " << poolArtSeleccionadas[static_cast<size_t>(i)].nombre 
                         << " [Def +" << poolArtSeleccionadas[static_cast<size_t>(i)].defensa << "] (" 
                         << poolArtSeleccionadas[static_cast<size_t>(i)].precio << " oro)" << endl;
                    idxCompra++;
                }
                cout << "0. Cancelar" << endl;

                int seleccion; cin >> seleccion;

                if (seleccion > 0 && seleccion <= (int)poolArmasSeleccionadas.size()) {
                    Arma elegida = poolArmasSeleccionadas[static_cast<size_t>(seleccion - 1)];
                    if (elegida.poder > p.armaEquipada.poder) {
                        if (p.oro >= elegida.precio) {
                            p.oro -= elegida.precio;
                            p.armaEquipada = elegida;
                            cout << "-Arma equipada con exito-" << endl;
                        } else cout << "No tienes suficiente oro." << endl;
                    } else cout << "Tu arma actual es mejor o igual." << endl;
                } 
                else if (seleccion > static_cast<int>(poolArmasSeleccionadas.size()) && seleccion <= static_cast<int>(poolArmasSeleccionadas.size()) + static_cast<int>(poolArtSeleccionadas.size())) {
                    const auto offset = static_cast<std::size_t>(seleccion - 1 - static_cast<int>(poolArmasSeleccionadas.size()));
                    Artefacto elegido = poolArtSeleccionadas[offset];
                    if (elegido.defensa > p.artefactoEquipado.defensa) {
                        if (p.oro >= elegido.precio) {
                            p.oro -= elegido.precio;
                            p.artefactoEquipado = elegido;
                            cout << "-Artefacto equipado con exito-" << endl;
                        } else cout << "Tu artefacto actual es mejor o igual." << endl;
                    } else cout << "No tienes suficiente oro." << endl;
                }
            }
            else if (categoria == 3) { // --- RELIQUIAS ---
                limpiarPantalla();
                cout << "\n--- RELIQUIAS DEL MERCADER ---" << endl;
                cout << "Ranuras disponibles: " << p.reliquias.size() << "/2" << endl;
                cout << "----------------------------------------" << endl;

                if (p.reliquias.size() >= 2) {
                    cout << "[!] Ya cargas 2 reliquias. No puedes llevar mas." << endl;
                } else {
                    // Mostrar solo reliquias que el jugador no tiene aún
                    vector<pair<int, Reliquia>> disponibles; // par: precio, reliquia
                    int idx = 1;

                    for (const auto& rel : listaReliquias) {
                        bool yaTiene = false;
                        for (const auto& nombre : p.reliquias) {
                            if (nombre == rel.nombre) { yaTiene = true; break; }
                        }
                        if (!yaTiene) {
                            // Precio según rareza
                            int precio = 0;
                            if (rel.rareza == "Comun") precio = 300;
                            else if (rel.rareza == "Raro") precio = 700;
                            else if (rel.rareza == "Epico") precio = 1500;

                            cout << idx << ". " << rel.nombre 
                                 << " (" << precio << " oro)" << endl;
                            cout << "   -> " << rel.efecto 
                                 << " [" << rel.rareza << "]" << endl;
                            disponibles.push_back({precio, rel});
                            idx++;
                        }
                    }

                    if (disponibles.empty()) {
                        cout << "[!] Ya tienes todas las reliquias disponibles." << endl;
                    } else {
                        cout << "0. Cancelar" << endl;
                        int seleccion; cin >> seleccion;

                        if (seleccion > 0 && seleccion <= (int)disponibles.size()) {
                            int precio = disponibles[static_cast<size_t>(seleccion - 1)].first;
                            Reliquia elegida = disponibles[static_cast<size_t>(seleccion - 1)].second;

                            if (p.oro >= precio) {
                                p.oro -= precio;
                                p.reliquias.push_back(elegida.nombre);
                                cout << "\n[RELIQUIA] Obtuviste: " << elegida.nombre << "!" << endl;
                                cout << "[EFECTO] " << elegida.efecto << endl;

                                // Aplicar efectos inmediatos
                                if (elegida.nombre == "Anillo del Asesino") {
                                    p.bonusCritico += 5;
                                    cout << "[BONUS] Critico +5%." << endl;
                                } else if (elegida.nombre == "Medallon del Guardian") {
                                    p.defensaBase += 10;
                                    cout << "[BONUS] Defensa +10." << endl;
                                } else if (elegida.nombre == "Garra del Berserker" && p.clase == "Guerrero") {
                                    p.fuerza += 8;
                                    p.ataqueBase += 8;
                                    cout << "[BONUS] Fuerza +8." << endl;
                                } else if (elegida.nombre == "Ojo del Arcano" && p.clase == "Mago") {
                                    p.inteligencia += 8;
                                    p.ataqueBase += 8;
                                    cout << "[BONUS] Inteligencia +8." << endl;
                                } else if (elegida.nombre == "Pluma del Viento" && p.clase == "Cazador") {
                                    p.destreza += 8;
                                    p.ataqueBase += 8;
                                    cout << "[BONUS] Destreza +8." << endl;
                                } else if (elegida.nombre == "Botas del Relampago") {
                                    p.velocidadBase += 10;
                                    cout << "[BONUS] Velocidad +10." << endl;
                                } else if (elegida.nombre == "Piedra del Alma") {
                                    cout << "[BONUS] Oro ganado por combate +25%." << endl;
                                } else if (elegida.nombre == "Amuleto de Sangre") {
                                    cout << "[BONUS] Activo: 20% de contraatacar al recibir dano." << endl;
                                }
                            } else {
                                cout << "[!] No tienes suficiente oro." << endl;
                            }
                        }
                    }
                }
            }
        }
        else if (opcion == 2) { // --- VENDER ---
            cout << "\n--- OBJETOS EN TU PODER ---" << endl;
            int idx = 1;

            if (p.inventario.empty()) {
                cout << "No tienes consumibles para vender." << endl;
            } else {
                for (size_t i = 0; i < p.inventario.size(); i++) {
                    int pVenta = static_cast<int>(static_cast<float>(p.inventario[i].precio) * 0.4f);
                    cout << idx << ". " << p.inventario[i].nombre << " (+" << pVenta << " oro)" << endl;
                    idx++;
                }
                cout << idx << ". Cancelar" << endl;

                int venta; cin >> venta;
                if (venta > 0 && venta < idx) {
                    int iReal = venta - 1;
                    int pVenta = static_cast<int>(
                        static_cast<float>(p.inventario[static_cast<size_t>(iReal)].precio) * 0.4f
                    );
                    p.oro += pVenta;
                    cout << "Vendiste " << p.inventario[static_cast<size_t>(iReal)].nombre 
                         << " por " << pVenta << " oro." << endl;
                    p.inventario.erase(p.inventario.begin() + iReal);
                }
            }
        }
        else if (opcion == 3) {
            enMenu = false;
        }

        if (opcion != 3) esperarTecla();
    }
}