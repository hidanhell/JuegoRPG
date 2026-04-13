#include "Utilidades.h"
#include <iostream>
#include <limits>
using namespace std;

void mostrarBarraHP(int hp, int hpMax) {
    cout << "[HP: " << hp << "/" << hpMax << "]" << endl;
}

void mostrarCabecera(string titulo) {
    cout << "==== " << titulo << " ====" << endl;
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
