#include "Armas.h"
#include "Enums.h"
#include "Rng.h"
#include <iostream>

// --- Constructor ---
// hId = 0 por defecto (sin habilidad). Solo armas Elite llevan hId != 0.
// eId == -1 activa efecto aleatorio para armas Raras.
Arma::Arma(int i, const std::string& n, int p, const std::string& r, int eId, const std::string& c, int z, int pr, int hId)
    : id(i), nombre(n), poder(p), rareza(rarezaFromString(r).value_or(Rareza::Comun)), efectoId(eId), clase(claseFromString(c).value_or(Clase::Ninguno)), zona(z), precio(pr), habilidadId(hId) {

    // Logica de automatizacion para Armas Raras
    // NOTA: Solo se activa si se pasa eId == -1 explicitamente al crear el arma
    if (rareza == Rareza::Raro && eId == -1) {
        // Centralizamos el efecto aleatorio de las armas raras en el RNG único del proyecto.
        efectoId = Rng::get().entre(1, 5);
    }
}

void Arma::mostrarInfo() const {
    std::cout << "ID: " << id
         << " | Arma: " << nombre
         << " | Poder: " << poder
         << " | Rareza: " << rarezaToString(rareza)
         << " | Clase: " << claseToString(clase)
         << " | Zona: " << zona
         << " | Precio: " << precio;

    if (efectoId == 0) {
        std::cout << " | Efecto: Ninguno";
    } else {
        std::cout << " | EfectoID: " << efectoId;
    }

    if (habilidadId != 0) {
        std::cout << " | HabilidadID: " << habilidadId << " [ARMA ELITE]";
    }

    std::cout << std::endl;
}