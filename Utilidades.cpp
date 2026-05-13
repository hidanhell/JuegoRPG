#include "Utilidades.h"
#include <iostream>
#include <limits>
#include <cctype>
using namespace std;

void mostrarBarraHP(int hp, int hpMax) {
    const int LARGO = 20; // Largo de la barra en bloques
    int llenos = (hpMax > 0) ? (hp * LARGO / hpMax) : 0;
    if (llenos < 0) llenos = 0;
    if (llenos > LARGO) llenos = LARGO;

    cout << "[";
    for (int i = 0; i < LARGO; i++) {
        cout << (i < llenos ? "#" : ".");
    }
    cout << "] " << hp << "/" << hpMax << " HP" << endl;
}

void mostrarCabecera(const std::string &titulo) {
    cout << "==== " << titulo << " ====" << endl;
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

char aMinuscula(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}