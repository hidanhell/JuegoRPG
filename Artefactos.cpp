#include "Artefactos.h"
#include "Rng.h"
#include <iostream>

using namespace std;

// --- Constructor ---
// e == -1 es la senal para asignar efecto aleatorio a artefactos Raros
Artefacto::Artefacto(int i, const string& n, int d, const string& r, int e, const string& c, int z, int pr)
    : id(i), nombre(n), defensa(d), rareza(r), efectoId(e), clase(c), zona(z), precio(pr) {
    
    // Logica hibrida: Azar solo para Raros con senal -1
    // NOTA: Solo se activa si se pasa e == -1 explicitamente al crear el artefacto
    if (rareza == "Raro" && e == -1) {
        // Centralizamos el efecto aleatorio de los artefactos raros en el RNG único del proyecto.
        efectoId = Rng::get().entre(1, 5);
    }
    // No hace falta el else porque efectoId ya se inicio con 'e' arriba
}

void Artefacto::mostrarInfo() const {
    cout << "ID: " << id
         << " | Artefacto: " << nombre
         << " | Defensa: " << defensa
         << " | Rareza: " << rareza
         << " | Clase: " << clase
         << " | Zona: " << zona
         << " | Precio: " << precio;

    if (efectoId == 0) {
        cout << " | Efecto: Ninguno";
    } else {
        cout << " | EfectoId: " << efectoId;
    }
    
    cout << endl;
}