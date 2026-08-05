#include "Artefactos.h"
#include "Enums.h"
#include "Rng.h"
#include <iostream>

// --- Constructor ---
// e == -1 es la senal para asignar efecto aleatorio a artefactos Raros
Artefacto::Artefacto(int i, const std::string& n, int d, const std::string& r, int e, const std::string& c, int z, int pr)
    : id(i), nombre(n), defensa(d), rareza(rarezaFromString(r).value_or(Rareza::Comun)), efectoId(e), clase(claseFromString(c).value_or(Clase::Ninguno)), zona(z), precio(pr) {
    
    // Logica hibrida: Azar solo para Raros con senal -1
    // NOTA: Solo se activa si se pasa e == -1 explicitamente al crear el artefacto
    if (rareza == Rareza::Raro && e == -1) {
        // Centralizamos el efecto aleatorio de los artefactos raros en el RNG único del proyecto.
        efectoId = Rng::get().entre(1, 5);
    }
    // No hace falta el else porque efectoId ya se inicio con 'e' arriba
}

void Artefacto::mostrarInfo() const {
    std::cout << "ID: " << id
         << " | Artefacto: " << nombre
         << " | Defensa: " << defensa
         << " | Rareza: " << rarezaToString(rareza)
         << " | Clase: " << claseToString(clase)
         << " | Zona: " << zona
         << " | Precio: " << precio;

    if (efectoId == 0) {
        std::cout << " | Efecto: Ninguno";
    } else {
        std::cout << " | EfectoId: " << efectoId;
    }
    
    std::cout << std::endl;
}