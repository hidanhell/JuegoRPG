#pragma once
#include <string>
#include <vector>

struct Consumible {
    int id;
    std::string nombre;
    std::string descripcion;
    
    // Atributos de efecto
    int curacion;       // Salud restaurada
    int buffAtaque;     // Aumento de fuerza
    int buffDefensa;    // Aumento de resistencia
    int buffVelocidad;  // Aumento de agilidad
    
    int duracion;       // Turnos que dura el efecto (0 si es instantáneo como curar)
    int zona;           // Para saber en qué nivel del mundo aparece
    int precio;

    Consumible(int i=0, std::string n="-", std::string d="", int cur=0, int atk=0, int def=0, int vel=0, int dur=0, int z=0, int pre=0)
        : id(i), nombre(n), descripcion(d), curacion(cur), buffAtaque(atk), buffDefensa(def), buffVelocidad(vel), duracion(dur), zona(z), precio(pre) {}
};

extern std::vector<Consumible> listaConsumibles;