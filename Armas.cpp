#include "Armas.h"
#include "Enums.h"
#include "Rng.h"
#include <iostream>
#include <utility>

// --- Constructor ---
// hId = 0 por defecto (sin habilidad). Solo armas Elite llevan hId != 0.
// eId == -1 activa efecto aleatorio para armas Raras.
Arma::Arma(int i, std::string n, int p, std::string r, int eId, std::string c, int z, int pr, int hId)
    : id(i), 
      nombre(std::move(n)), 
      poder(p), 
      rareza(rarezaFromString(r).value_or(Rareza::Comun)), 
      efectoId(eId), 
      clase(claseFromString(c).value_or(Clase::Ninguno)), 
      zona(z), 
      precio(pr), 
      habilidadId(hId) {

    // Lógica de automatización para Armas Raras
    if (rareza == Rareza::Raro && eId == -1) {
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

    std::cout << '\n'; // '\n' es ligeramente más rápido que std::endl al evitar flush innecesario
}