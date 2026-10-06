#ifndef RELIQUIAS_H
#define RELIQUIAS_H

#include <string>
#include <vector>
#include "Enums.h"

struct Reliquia {
    int id;
    std::string nombre;
    std::string efecto;
    Rareza rareza;

    Reliquia(int i = 0,
             std::string n = "-",
             std::string e = "Ninguno",
             std::string r = "Comun");

    void mostrarInfo() const;
};

struct Personaje;

// Catálogo global de reliquias (definido en reliquiasDatos.cpp)
extern std::vector<Reliquia> listaReliquias;

bool reliquiaCompatibleConClase(const Reliquia& reliquia, Clase clase);
bool aplicarBonosReliquia(Personaje& personaje, const Reliquia& reliquia);
void retirarBonosReliquia(Personaje& personaje, const Reliquia& reliquia);

#endif