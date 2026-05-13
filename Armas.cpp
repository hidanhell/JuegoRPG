#include "Armas.h"
#include <iostream>
#include <random>

using namespace std;

// --- Constructor ---
// 'precio(pr)' incluido en la lista de inicializacion
// eId == -1 es la senal para asignar efecto aleatorio a armas Raras
Arma::Arma(int i, const string& n, int p, const string& r, int eId, const string& c, int z, int pr) 
    : id(i), nombre(n), poder(p), rareza(r), efectoId(eId), clase(c), zona(z), precio(pr) {
    
    // Logica de automatizacion para Armas Raras
    // NOTA: Solo se activa si se pasa eId == -1 explicitamente al crear el arma
    if (rareza == "Raro" && eId == -1) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 5);
        
        efectoId = dis(gen);
    }
    // No hace falta el else porque efectoId ya se inicializo con eId en la lista de arriba
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
    
    cout << endl;
}