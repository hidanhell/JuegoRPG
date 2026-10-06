#include "Tienda.h"
#include "Utilidades.h"
#include "Armas.h"
#include "Artefactos.h"
#include "Reliquias.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "CatalogoObjetos.h"
#include "Consumibles.h"
#include "Rng.h"

using namespace std;

static int zonaDePosicion(int y) {
    return (y <= 60) ? 1 : (y <= 120) ? 2 : (y <= 180) ? 3 : (y <= 240) ? 4 : 5;
}

static int precioTienda(int precio, int zonaArticulo, int zonaActual) {
    return zonaArticulo > zonaActual ? (precio * 3 + 1) / 2 : precio;
}

// =========================================================
// BLOQUE M3: SISTEMA DE TIENDA (v2.2 - Venta de Reliquias)
// =========================================================

void entrarTienda(Personaje &p) {
    const int zonaActual = zonaDePosicion(p.posY);
    const int zonaMaximaTienda = min(4, zonaActual + 1);
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
                    if (item.zona > zonaMaximaTienda) continue;
                    const int precio = precioTienda(item.precio, item.zona, zonaActual);
                    cout << idxItem << ". " << item.nombre 
                        << " (" << precio << " oro)"
                        << (item.zona > zonaActual ? " [zona adelantada +50%]" : "")
                        << " - " << item.descripcion << endl;
                    stockTienda.push_back(item);
                    idxItem++;
                }
                cout << "0. Cancelar" << endl;

                int seleccion; cin >> seleccion;
                if (seleccion > 0 && seleccion <= (int)stockTienda.size()) {
                    Consumible elegido = stockTienda[static_cast<size_t>(seleccion - 1)];
                    const int precio = precioTienda(elegido.precio, elegido.zona, zonaActual);

                    if (p.oro >= precio && p.inventario.size() < 10) {
                        p.oro -= precio;
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

                for (auto &a : listaArmas) {
                    if (a.clase == p.clase && a.zona <= zonaMaximaTienda && a.rareza != Rareza::Elite) {
                        poolArmas.push_back(a);
                    }
                }
                for (auto &a : listaArtefactos) {
                    if (a.clase == p.clase && a.zona <= zonaMaximaTienda) poolArt.push_back(a);
                }

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
                    const Arma& arma = poolArmasSeleccionadas[static_cast<size_t>(i)];
                    const int precio = precioTienda(arma.precio, arma.zona, zonaActual);
                    cout << idxCompra << ". " << arma.nombre
                         << " [Atk +" << poolArmasSeleccionadas[static_cast<size_t>(i)].poder << "] (" 
                        << precio << " oro)"
                        << (arma.zona > zonaActual ? " [zona adelantada +50%]" : "") << endl;
                    idxCompra++;
                }
                for (int i = 0; i < (int)poolArtSeleccionadas.size(); i++) {
                    const Artefacto& artefacto = poolArtSeleccionadas[static_cast<size_t>(i)];
                    const int precio = precioTienda(artefacto.precio, artefacto.zona, zonaActual);
                    cout << idxCompra << ". " << artefacto.nombre 
                         << " [Def +" << poolArtSeleccionadas[static_cast<size_t>(i)].defensa << "] (" 
                        << precio << " oro)"
                        << (artefacto.zona > zonaActual ? " [zona adelantada +50%]" : "") << endl;
                    idxCompra++;
                }
                cout << "0. Cancelar" << endl;

                int seleccion; cin >> seleccion;

                if (seleccion > 0 && seleccion <= (int)poolArmasSeleccionadas.size()) {
                    Arma elegida = poolArmasSeleccionadas[static_cast<size_t>(seleccion - 1)];
                    const int precio = precioTienda(elegida.precio, elegida.zona, zonaActual);
                    if (elegida.poder > p.armaEquipada.poder) {
                        if (p.oro >= precio) {
                            p.oro -= precio;
                            p.armaEquipada = elegida;
                            cout << "-Arma equipada con exito-" << endl;
                        } else cout << "No tienes suficiente oro." << endl;
                    } else cout << "Tu arma actual es mejor o igual." << endl;
                } 
                else if (seleccion > static_cast<int>(poolArmasSeleccionadas.size()) && seleccion <= static_cast<int>(poolArmasSeleccionadas.size()) + static_cast<int>(poolArtSeleccionadas.size())) {
                    const auto offset = static_cast<std::size_t>(seleccion - 1 - static_cast<int>(poolArmasSeleccionadas.size()));
                    Artefacto elegido = poolArtSeleccionadas[offset];
                    const int precio = precioTienda(elegido.precio, elegido.zona, zonaActual);
                    if (elegido.defensa > p.artefactoEquipado.defensa) {
                        if (p.oro >= precio) {
                            p.oro -= precio;
                            p.artefactoEquipado = elegido;
                            cout << "-Artefacto equipado con exito-" << endl;
                        } else cout << "No tienes suficiente oro." << endl;
                    } else cout << "Tu artefacto actual es mejor o igual." << endl;
                }
            }
            else if (categoria == 3) { // --- RELIQUIAS ---
                limpiarPantalla();
                cout << "\n--- RELIQUIAS DEL MERCADER ---" << endl;
                cout << "Ranuras disponibles: " << p.reliquias.size() << "/2" << endl;
                cout << "----------------------------------------" << endl;

                if (p.reliquias.size() >= 2) {
                    cout << "[!] Ya cargas 2 reliquias. Vende una si quieres comprar otra." << endl;
                } else {
                    vector<pair<int, Reliquia>> disponibles; 
                    int idx = 1;

                    for (const auto& rel : listaReliquias) {
                        if (!reliquiaCompatibleConClase(rel, p.clase)) continue;

                        bool yaTiene = false;
                        for (const auto& nombre : p.reliquias) {
                            if (nombre == rel.nombre) { yaTiene = true; break; }
                        }
                        if (!yaTiene) {
                            int precio = 0;
                            if (rel.rareza == Rareza::Comun) precio = 300;
                            else if (rel.rareza == Rareza::Raro) precio = 700;
                            else if (rel.rareza == Rareza::Epico) precio = 1500;

                            cout << idx << ". " << rel.nombre 
                                 << " (" << precio << " oro)" << endl;
                            cout << "   -> " << rel.efecto 
                                 << " [" << rarezaToString(rel.rareza) << "]" << endl;
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
                                if (aplicarBonosReliquia(p, elegida)) {
                                    cout << "[BONUS] Bono permanente aplicado." << endl;
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
            cout << "\n--- QUE DESEAS VENDER? ---" << endl;
            cout << "1. Consumibles" << endl;
            cout << "2. Reliquias Equipadas" << endl;
            cout << "0. Cancelar" << endl;

            int tipoVenta; cin >> tipoVenta;

            if (tipoVenta == 1) { // --- VENDER CONSUMIBLES ---
                cout << "\n--- CONSUMIBLES EN TU PODER ---" << endl;
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
            else if (tipoVenta == 2) { // --- VENDER RELIQUIAS ---
                cout << "\n--- TUS RELIQUIAS EQUIPADAS ---" << endl;
                
                if (p.reliquias.empty()) {
                    cout << "No tienes reliquias para vender." << endl;
                } else {
                    for (size_t i = 0; i < p.reliquias.size(); i++) {
                        // Buscar la reliquia en el catálogo para mostrar su precio de venta
                        int precioVenta = 0;
                        for (const auto& rel : listaReliquias) {
                            if (rel.nombre == p.reliquias[i]) {
                                if (rel.rareza == Rareza::Comun) precioVenta = 120; // 40% de 300
                                else if (rel.rareza == Rareza::Raro) precioVenta = 280; // 40% de 700
                                else if (rel.rareza == Rareza::Epico) precioVenta = 600; // 40% de 1500
                                break;
                            }
                        }
                        cout << i + 1 << ". " << p.reliquias[i] << " (+" << precioVenta << " oro)" << endl;
                    }
                    cout << "0. Cancelar" << endl;

                    int relVenta; cin >> relVenta;
                    if (relVenta > 0 && relVenta <= (int)p.reliquias.size()) {
                        int rReal = relVenta - 1;
                        string nombreReliquia = p.reliquias[static_cast<size_t>(rReal)];
                        
                        int pVentaReliquia = 0;
                        for (const auto& rel : listaReliquias) {
                            if (rel.nombre == nombreReliquia) {
                                if (rel.rareza == Rareza::Comun) pVentaReliquia = 120;
                                else if (rel.rareza == Rareza::Raro) pVentaReliquia = 280;
                                else if (rel.rareza == Rareza::Epico) pVentaReliquia = 600;
                                break;
                            }
                        }

                        const auto reliquia = std::find_if(listaReliquias.begin(), listaReliquias.end(),
                            [&nombreReliquia](const Reliquia& candidata) {
                                return candidata.nombre == nombreReliquia;
                            });
                        if (reliquia != listaReliquias.end()) retirarBonosReliquia(p, *reliquia);

                        p.oro += pVentaReliquia;
                        p.reliquias.erase(p.reliquias.begin() + rReal);
                        
                        cout << "\nHas vendido la reliquia " << nombreReliquia << " por " << pVentaReliquia << " oro." << endl;
                        cout << "[!] Se han revertido sus efectos y has recuperado el espacio." << endl;
                    }
                }
            }
        }
        else if (opcion == 3) {
            enMenu = false;
        }

        if (opcion != 3) esperarTecla();
    }
}