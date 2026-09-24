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

    // ================================================================
    // NUEVO: campos para nuevos tipos de consumibles (Antidoto, Tonico
    // de recurso, bombas ofensivas, Amuleto de Huida). Se agregan al
    // FINAL y con valores por defecto para no romper ninguna de las
    // listas de inicializacion que ya existian en Consumibles.cpp --
    // esos items simplemente no los mencionan y quedan en 0/false.
    // ================================================================
    bool curaEstados;       // true = al usarlo limpia todos tus efectos de estado activos (Antidoto)
    int recursoRestaurado;  // Cantidad fija de recurso (Ira/Mana/Enfoque) que restaura al usarlo
    int danoDirecto;        // Dano directo al enemigo al usarlo en combate (objetos ofensivos)
    int efectoAlUsar;       // ID de efecto (ver listaEfectos en Efectos_Data.cpp) que se aplica al enemigo al usar este objeto
    bool huidaGarantizada;  // true = usar este objeto garantiza escapar del combate sin tirar probabilidad

    Consumible(int i=0, std::string n="-", std::string d="", int cur=0, int atk=0, int def=0, int vel=0, int dur=0, int z=0, int pre=0,
               bool curaEst=false, int recurso=0, int danoDir=0, int efAlUsar=0, bool huidaGar=false)
        : id(i), nombre(n), descripcion(d), curacion(cur), buffAtaque(atk), buffDefensa(def), buffVelocidad(vel),
          duracion(dur), zona(z), precio(pre),
          curaEstados(curaEst), recursoRestaurado(recurso), danoDirecto(danoDir), efectoAlUsar(efAlUsar), huidaGarantizada(huidaGar) {}
};

extern std::vector<Consumible> listaConsumibles;