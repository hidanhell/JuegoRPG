#include "reliquias.h"
#include <iostream>

Reliquia::Reliquia(int i, std::string n, std::string e, std::string r)
    : id(i), nombre(n), efecto(e), rareza(r) {}

void Reliquia::mostrarInfo() const {
    std::cout << "ID: " << id
              << " | Reliquia: " << nombre
              << " | Efecto: " << efecto
              << " | Rareza: " << rareza
              << std::endl;
}
