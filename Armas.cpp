#include "Armas.h"
#include <iostream>
#include <random>

using namespace std;

// --- Constructor ---
// hId = 0 por defecto (sin habilidad). Solo armas Elite llevan hId != 0.
// eId == -1 activa efecto aleatorio para armas Raras.
Arma::Arma(int i, const string& n, int p, const string& r, int eId, const string& c, int z, int pr, int hId)
    : id(i), nombre(n), poder(p), rareza(r), efectoId(eId), clase(c), zona(z), precio(pr), habilidadId(hId) {

    // Logica de automatizacion para Armas Raras
    // NOTA: Solo se activa si se pasa eId == -1 explicitamente al crear el arma
    if (rareza == "Raro" && eId == -1) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);
        efectoId = dis(gen);
    }
}

void Arma::mostrarInfo() const {
    cout << "ID: " << id
         << " | Arma: " << nombre
         << " | Poder: " << poder
         << " | Rareza: " << rareza
         << " | Clase: " << clase
         << " | Zona: " << zona
         << " | Precio: " << precio;

    if (efectoId == 0) {
        cout << " | Efecto: Ninguno";
    } else {
        cout << " | EfectoID: " << efectoId;
    }

    if (habilidadId != 0) {
        cout << " | HabilidadID: " << habilidadId << " [ARMA ELITE]";
    }

    cout << endl;
}