// =========================================================
// BLOQUE M: MOTOR PRINCIPAL DEL JUEGO (v1.19)
// =========================================================

#include "utilidades.h"
#include "personajes.h"
#include "combate.h"
#include "loot.h"
#include "tienda.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
#include <cctype>
using namespace std;

inline char aMinuscula(char c) {
    return static_cast<char>(tolower(static_cast<unsigned char>(c)));
}

int main() {
    srand((unsigned)time(0));
    string nombreUser;
    int tipoClase;

    // --- Inicio del juego con Lore ---
    mostrarCabecera("EL REINO DE LAS SOMBRAS v1.19");

    cout << "Tu cuerpo yace en medio de la aldea, derrotado tras una gran batalla." << endl;
    cout << "No recuerdas nada... ni cómo llegaste aquí." << endl;
    cout << "Cierras los ojos y ves a un caballero dragón luchando con furia." << endl;
    cout << "Abres los ojos: una figura negra se acerca lentamente entre las ruinas." << endl;
    cout << "Cierras los ojos de nuevo: un dragón lanza un ataque de fuego brutal." << endl;
    cout << "El tiempo se desvanece... no sabes cuánto permaneces inconsciente." << endl;
    cout << "\nFinalmente despiertas en una pequeña casa." << endl;
    cout << "Desde la cama ves una mesa con dos sillas y una chimenea apagada," << endl;
    cout << "aún con brasas rojas consumiéndose." << endl;
    cout << "Junto a la cama descansa un cofre polvoriento, y frente a ti la puerta." << endl;

    cout << "\n¿Quieres abrir el cofre antes de salir? (S/N): ";

char decision; cin >> decision;

Arma inicial = {"", 0};  

if (aMinuscula(decision) == 's') {
    inicial = {"Espada Oxidada", 12}; // ejemplo para guerrero
    cout << "\nHas tomado el cofre: " << inicial.nombre << " y una Pocion Baja." << endl;
    system("pause");
} else {
    cout << "\nDecides salir sin abrir el cofre. El mundo será más hostil sin ayuda inicial." << endl;
    system("pause");
}

    // --- Ahora el jugador recuerda quién es ---
    system("cls");
    cout << "Al salir de la casa, un viento frío golpea tu rostro." << endl;
    cout << "Tu mente comienza a aclararse... recuerdas tu nombre y tu senda." << endl;
    cout << "Dime tu nombre: "; 
    limpiarBuffer(); getline(cin, nombreUser);
    cout << "Elige tu senda (1. Guerrero, 2. Mago, 3. Cazador): "; cin >> tipoClase;

    Personaje p(nombreUser, tipoClase);
    if (tolower(decision) == 's') {
        p.armaEquipada = inicial;
        p.inventario.push_back("Pocion Baja");
    }

    while (true) {
        system("cls");
        cout << "ESTAS EN: " << obtenerNombreZona(p.posY) << endl;
        cout << "POSICION: [X: " << p.posX << " | Y: " << p.posY << "]" << endl;
        cout << "ESTADO: " << p.hp << "/" << p.hpMax << " HP | ORO: " << p.oro << " | NIV: " << p.nivel << endl;
        cout << "----------------------------------------------------" << endl;
        
        // --- CONTROL DE RESURRECCIÓN GLOBAL ---
        if (p.hp <= 0) {
            system("cls");
            cout << "Has sido derrotado... Regresas a la Aldea para recuperarte." << endl;
            p.posX = 0; 
            p.posY = 0;
            p.hp = p.hpMax; 
            system("pause");
            continue; 
        }

        // Detección de Tienda
        if (p.posY == 30 && p.posX == 3) {
            cout << "[!] TIENDA CERCANA: Presiona 'T' para comerciar." << endl;
        }
        
        if (p.posY >= 230) cout << "[ALERTA] El aire hierve... Lancelot esta muy cerca." << endl;

        cout << "CONTROLES: (W-A-S-D) Mover | (P) Status | (Q) Salir" << endl;
        
        char input; cin >> input; input = aMinuscula(input);

        if (input == 'w') {
            if (p.posY < 240) {
                p.posY++;
                if (p.posY < 240 && rand() % 100 < 30) iniciarCombate(p, p.posY);
            }
        }

        //Controles
        else if (input == 's') { if (p.posY > 0) p.posY--; }
        else if (input == 'a') { p.posX--; }
        else if (input == 'd') { p.posX++; }
        else if (input == 't' && p.posY == 30 && p.posX == 3) { entrarTienda(p); }
        else if (input == 'p') {
            system("cls");
            mostrarCabecera("STATUS DE " + p.nombre);
            cout << "CLASE: " << p.clase << " | NIVEL: " << p.nivel << endl;
            mostrarBarraHP(p.hp, p.hpMax);
            cout << "EXP: " << p.exp << " / " << (p.nivel * 50) << endl;
            cout << "----------------------------------------" << endl;
            cout << "ATAQUE BASE: " << p.ataqueBase << " (+ " << p.armaEquipada.atk << " del arma)" << endl;
            cout << "DEFENSA BASE: " << p.defensaBase << " | VELOCIDAD: " << p.velocidadBase << endl;
            cout << "ORO: " << p.oro << " monedas." << endl;
            cout << "ARMA: " << p.armaEquipada.nombre << endl;
            cout << "INVENTARIO (max 10): ";
            if (p.inventario.empty()) cout << "Vacio";
            else {
                map<string,string> abrevs = {
                    {"Pocion Baja","PB"},
                    {"Pocion Media","PM"},
                    {"Pocion Alta","PA"}
                };
                map<string,int> conteo;
                for (string it : p.inventario) {
                    if (abrevs.count(it)) conteo[abrevs[it]]++;
                    else conteo[it]++;
                }
                for (auto &kv : conteo) cout << kv.first << " x" << kv.second << " | ";
            }

            cout << "\nRELIQUIAS (max 2): ";
            if (p.reliquias.empty()) cout << "Ninguna";
            else {
                for (string r : p.reliquias) cout << "[" << r << "] ";
            }

            cout << "\n\nPresiona ENTER para volver...";
            limpiarBuffer(); cin.get();
        }
        else if (input == 'q') {
            cout << "¿Estas seguro de que quieres abandonar la mision? (S/N): ";
            char conf; cin >> conf; if (tolower(conf) == 's') break;
        }

        // Límites laterales
        if (p.posX > 10) {
            p.posX = 10;
            cout << "[SISTEMA] No puedes avanzar más allá, el camino está bloqueado." << endl;
            system("pause");
        }
        if (p.posX < -10) {
            p.posX = -10;
            cout << "[SISTEMA] No puedes retroceder más allá, un muro invisible te detiene." << endl;
            system("pause");
        }

        // Encuentro final
        if (p.posY == 240) {
            system("cls");
            mostrarCabecera("EL TRONO DEL REY DRAGON");
            cout << "Lancelot te espera sentado en un trono de huesos." << endl;
            cout << "Antes de avanzar, ves un Cofre Divino." << endl;
            cout << "¿Abrirlo? (S/N): ";
            char cL; cin >> cL;
            if (tolower(cL) == 's') {
                Arma legend;
                if (p.clase == "Guerrero") legend = {"EXCALIBUR Asesina de Dragones", 325};
                else if (p.clase == "Mago") legend = {"Baston del Rey Dragon", 300};
                else legend = {"Arco Alma de Dragon", 300};
                p.armaEquipada = legend;
                for(int i=0; i<3; i++) p.inventario.push_back("Pocion Alta");
                cout << "\n¡HAS OBTENIDO " << legend.nombre << " Y 3 POCIONES ALTAS!" << endl;
                system("pause");
            }

            cout << "\n¿Es el momento de la venganza? (S/N): ";
            char pelear; cin >> pelear;
            if (tolower(pelear) == 's') {
                iniciarCombate(p, 240);

                if (p.hp > 0) {
                    system("cls");
                    cout << "¡Lancelot ha caido! El cielo se aclara por primera vez en decadas." << endl;
                    cout << "Has completado tu destino, " << p.nombre << "." << endl;
                    system("pause");
                    break;
                } else {
                    // Mantenemos tu mensaje aquí, el control de arriba se encargará del reset real
                    system("cls");
                    cout << "Has sido derrotado por Lancelot... Regresas a la Aldea para recuperarte." << endl;
                    p.posX = 0; 
                    p.posY = 0;
                    p.hp = p.hpMax; 
                    system("pause");
                }
            } else {
                p.posY = 239;
            }
        }
    }
    return 0;
}