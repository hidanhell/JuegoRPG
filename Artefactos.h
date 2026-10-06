#pragma once
#include <string>
#include "Enums.h"

// --- Artefacto ---
struct Artefacto {
    int id;                 // Identificador único
    std::string nombre;     // Nombre del artefacto
    int defensa;            // Valor de defensa
    Rareza rareza;          // Rareza (Común, Raro, etc.)
    int efectoId;           // ID del efecto especial (1=Quemadura, 2=Veneno, etc.)
    Clase clase;            // Clase que lo puede usar (Guerrero, Mago, Cazador)
    int zona;               // Zona de aparición (1-4)
    int precio;             // Costo en la tienda

    // Constructor por defecto
    Artefacto()
        : id(0), nombre("-"), defensa(0),
          rareza(Rareza::Comun), efectoId(0),
          clase(Clase::Ninguno), zona(0), precio(0) {}

    // Constructor con parámetros
    Artefacto(int i, const std::string& n, int d,
              const std::string& r, int e,
              const std::string& c, int z, int pr);

    // Método para mostrar información
    void mostrarInfo() const;
};