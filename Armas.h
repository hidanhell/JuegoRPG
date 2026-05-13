#ifndef ARMAS_H
#define ARMAS_H

#include <string>

// --- Arma ---
struct Arma {
    int id;                 // Identificador único
    std::string nombre;     // Nombre del arma
    int poder;              // Daño base
    std::string rareza;     // Común, Raro, Épico, Legendario, Elite
    int efectoId;           // ID del efecto de estado (0 = ninguno, -1 = aleatorio para Raros)
    std::string clase;      // Guerrero, Mago, Cazador
    int zona;               // Zona de aparición (1-4)
    int precio;             // Costo en la tienda
    int habilidadId;        // ID de habilidad especial del arma (0 = ninguna, solo armas Elite)

    // Constructor
    Arma(int i = 0,
         const std::string& n = "-",
         int p = 0,
         const std::string& r = "Comun",
         int eId = 0,
         const std::string& c = "Ninguno",
         int z = 0,
         int pr = 0,
         int hId = 0);

    void mostrarInfo() const;
};

#endif