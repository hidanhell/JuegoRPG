#pragma once
#include <string>

// --- Artefacto ---
struct Artefacto {
    int id;                 // Identificador unico
    std::string nombre;     // Nombre del artefacto
    int defensa;            // Valor de defensa
    std::string rareza;     // Rareza (Comun, Raro, etc.)
    int efectoId;           // ID del efecto especial (1=Quemadura, 2=Veneno, etc.)
    std::string clase;      // Clase que lo puede usar (Guerrero, Mago, Cazador)
    int zona;               // Zona de aparicion (1-4)
    int precio;             // Costo en la tienda

    // Constructor por defecto
    Artefacto()
        : id(0), nombre("-"), defensa(0),
          rareza("Ninguno"), efectoId(0),
          clase("Ninguno"), zona(0), precio(0) {}

    // Constructor con parametros
    // Usa const& en strings para evitar copias innecesarias
    Artefacto(int i, const std::string& n, int d,
              const std::string& r, int e,
              const std::string& c, int z, int pr);

    // Metodo para mostrar informacion
    void mostrarInfo() const;
};