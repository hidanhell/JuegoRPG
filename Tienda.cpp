#include "tienda.h"
#include "utilidades.h"
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// =========================================================
// BLOQUE M3: SISTEMA DE TIENDA (v1.18)
// =========================================================

void entrarTienda(Personaje &p) {
    bool enMenu = true;

    while (enMenu) {
        system("cls");
        mostrarCabecera("MERCADER DE LA CIUDADELA");

        cout << "Oro disponible: " << p.oro 
             << " | Bolsa: " << p.inventario.size() << "/10 consumibles"
             << " | Reliquias: " << p.reliquias.size() << "/2" << endl;
        cout << "----------------------------------------" << endl;
        cout << "¿Qué deseas hacer?" << endl;
        cout << "1. Comprar" << endl;
        cout << "2. Vender" << endl;
        cout << "3. Salir" << endl;
        cout << "----------------------------------------" << endl;

        int opcion; cin >> opcion;

        // --- COMPRAR ---
        if (opcion == 1) {
            cout << "\n--- OBJETOS DISPONIBLES ---" << endl;
            cout << "1. Pocion Baja (200 oro) [Cura 30%]" << endl;
            cout << "2. Pocion Media (400 oro) [Cura 60%]" << endl;
            cout << "3. Pocion Alta (1200 oro) [Cura 90%]" << endl;
            cout << "4. Colmillo de Vampiro (2500 oro) [Roba vida 5%]" << endl;
            cout << "5. Pluma del Fénix (3000 oro) [Revive una vez]" << endl;
            cout << "6. Medallón del Guardián (2200 oro) [+10 defensa]" << endl;
            cout << "7. Llave del Pueblo (1500 oro) [Teleporta a Aldea, un solo uso]" << endl;
            cout << "8. Anillo del Asesino (2800 oro) [+5% crítico]" << endl;
            cout << "9. Cancelar" << endl;

            int compra; cin >> compra;
            if (compra == 1 && p.oro >= 200 && p.inventario.size() < 10) { p.oro -= 200; p.inventario.push_back("Pocion Baja"); cout << "¡Compraste Pocion Baja!" << endl; }
            else if (compra == 2 && p.oro >= 400 && p.inventario.size() < 10) { p.oro -= 400; p.inventario.push_back("Pocion Media"); cout << "¡Compraste Pocion Media!" << endl; }
            else if (compra == 3 && p.oro >= 1200 && p.inventario.size() < 10) { p.oro -= 1200; p.inventario.push_back("Pocion Alta"); cout << "¡Compraste Pocion Alta!" << endl; }
            else if (compra == 4 && p.oro >= 2500 && p.reliquias.size() < 2) { p.oro -= 2500; p.reliquias.push_back("Colmillo de Vampiro"); cout << "¡Compraste Colmillo de Vampiro!" << endl; }
            else if (compra == 5 && p.oro >= 3000 && p.reliquias.size() < 2) { p.oro -= 3000; p.reliquias.push_back("Pluma del Fénix"); cout << "¡Compraste Pluma del Fénix!" << endl; }
            else if (compra == 6 && p.oro >= 2200 && p.reliquias.size() < 2) { p.oro -= 2200; p.reliquias.push_back("Medallón del Guardián"); p.defensaBase += 10; cout << "¡Compraste Medallón del Guardián! Defensa +10." << endl; }
            else if (compra == 7 && p.oro >= 1500 && p.reliquias.size() < 2) { p.oro -= 1500; p.reliquias.push_back("Llave del Pueblo"); cout << "¡Compraste Llave del Pueblo!" << endl; }
            else if (compra == 8 && p.oro >= 2800 && p.reliquias.size() < 2) { p.oro -= 2800; p.reliquias.push_back("Anillo del Asesino"); p.bonusCritico += 5; cout << "¡Compraste Anillo del Asesino! Crítico +5%." << endl; }
            else if (compra == 9) { cout << "Cancelaste la compra." << endl; }
            else { cout << "Oro insuficiente, inventario lleno o selección inválida." << endl; }
        }

        // --- VENDER DINÁMICO ---
        else if (opcion == 2) {
            cout << "\n--- OBJETOS EN TU PODER ---" << endl;
            vector<pair<string,int>> opcionesVenta;
            int idx = 1;

            // Inventario
            for (string item : p.inventario) {
                int precio = (item=="Pocion Baja")?50:(item=="Pocion Media")?100:(item=="Pocion Alta")?300:0;
                if (precio > 0) {
                    cout << idx << ". " << item << " (+" << precio << " oro)" << endl;
                    opcionesVenta.push_back({item,precio});
                    idx++;
                }
            }

            // Reliquias
            for (string r : p.reliquias) {
                int precio = (r=="Colmillo de Vampiro")?1250:(r=="Pluma del Fénix")?1500:(r=="Medallón del Guardián")?1100:(r=="Llave del Pueblo")?750:(r=="Anillo del Asesino")?1400:0;
                if (precio > 0) {
                    cout << idx << ". " << r << " (+" << precio << " oro)" << endl;
                    opcionesVenta.push_back({r,precio});
                    idx++;
                }
            }

            cout << idx << ". Cancelar" << endl;
            int venta; cin >> venta;
            if (venta > 0 && venta < idx) {
                string elegido = opcionesVenta[venta-1].first;
                int precio = opcionesVenta[venta-1].second;

                auto itInv = find(p.inventario.begin(), p.inventario.end(), elegido);
                if (itInv != p.inventario.end()) p.inventario.erase(itInv);
                else {
                    auto itRel = find(p.reliquias.begin(), p.reliquias.end(), elegido);
                    if (itRel != p.reliquias.end()) p.reliquias.erase(itRel);
                }
                p.oro += precio;
                cout << "¡Venta realizada!" << endl;
            } else {
                cout << "Cancelaste la venta." << endl;
            }
        }

        else if (opcion == 3) {
            enMenu = false;
        }

        if (opcion != 3) system("pause");
    }
}
