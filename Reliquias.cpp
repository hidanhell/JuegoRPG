#include "reliquias.h"
#include "Enums.h"
#include <iostream>

Reliquia::Reliquia(int i, std::string n, std::string e, std::string r)
    : id(i), nombre(n), efecto(e), rareza(rarezaFromString(r).value_or(Rareza::Comun)) {}

void Reliquia::mostrarInfo() const {
    std::cout << "ID: " << id
              << " | Reliquia: " << nombre
              << " | Efecto: " << efecto
              << " | Rareza: " << rarezaToString(rareza)
              << std::endl;
}
