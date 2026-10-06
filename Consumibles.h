#pragma once
#include <string>
#include <vector>
#include <utility>

struct Consumible {
    int id;
    std::string nombre;
    std::string descripcion;
    
    // Modificadores de estadísticas y estado
    int curacion;          // Salud restaurada al instante
    int buffAtaque;        // Incremento de ataque
    int buffDefensa;       // Incremento de defensa
    int buffVelocidad;     // Incremento de velocidad
    int duracion;          // Duración en turnos (0 = instantáneo)
    int zona;              // Zona de aparición en el mapa (1-4)
    int precio;            // Costo en la tienda

    // Propiedades mecánicas especiales
    bool curaEstados;      // Limpia todas las alteraciones de estado activas
    int recursoRestaurado; // Puntos restaurados de Ira, Maná o Enfoque
    int danoDirecto;       // Daño infligido directamente al enemigo
    int efectoAlUsar;      // ID de efecto de estado a aplicar al objetivo
    bool huidaGarantizada; // Éxito de huida automático en combate

    // Constructor con valores por defecto
    Consumible(int i = 0, std::string n = "-", std::string d = "", 
               int cur = 0, int atk = 0, int def = 0, int vel = 0, 
               int dur = 0, int z = 0, int pre = 0,
               bool curaEst = false, int recurso = 0, int danoDir = 0, 
               int efAlUsar = 0, bool huidaGar = false)
        : id(i), nombre(std::move(n)), descripcion(std::move(d)), 
          curacion(cur), buffAtaque(atk), buffDefensa(def), buffVelocidad(vel),
          duracion(dur), zona(z), precio(pre),
          curaEstados(curaEst), recursoRestaurado(recurso), 
          danoDirecto(danoDir), efectoAlUsar(efAlUsar), huidaGarantizada(huidaGar) {}
};

// Catálogo global de consumibles
extern std::vector<Consumible> listaConsumibles;