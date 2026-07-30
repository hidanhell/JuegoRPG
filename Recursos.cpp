#include "Recursos.h"
#include "Personajes.h"
#include <iostream>
#include <algorithm>

// Asigna el tipo de recurso y los valores iniciales según la clase del personaje
void inicializarRecurso(Personaje &pj) {
    // Suponiendo que pj.clase guarda el tipo ("Guerrero", "Mago", "Cazador")
    if (pj.clase == "Guerrero") {
        pj.tipoRecurso = "Ira";
        pj.recursoMax = 100;
        pj.recursoActual = 0; // El guerrero empieza sin ira y la acumula en combate
    } 
    else if (pj.clase == "Mago") {
        pj.tipoRecurso = "Mana";
        pj.recursoMax = 100;
        pj.recursoActual = 100; // El mago empieza con todo el maná lleno
    } 
    else if (pj.clase == "Cazador") {
        pj.tipoRecurso = "Enfoque";
        pj.recursoMax = 50;
        pj.recursoActual = 50; // El cazador empieza con el enfoque al máximo
    } 
    else {
        pj.tipoRecurso = "Energia";
        pj.recursoMax = 100;
        pj.recursoActual = 100;
    }
}

// Retorna el nombre del recurso para mostrarlo en el HUD
std::string obtenerNombreRecurso(const Personaje &pj) {
    return pj.tipoRecurso;
}

// Valida si el personaje tiene suficiente recurso y lo descuenta
bool gastarRecurso(Personaje &pj, int costo) {
    if (pj.recursoActual >= costo) {
        pj.recursoActual -= costo;
        return true;
    }
    std::cout << "[!] No tienes suficiente " << pj.tipoRecurso << " (" << pj.recursoActual << "/" << costo << ").\n";
    return false;
}

// Reglas de generación pasiva o activa al finalizar/iniciar un turno
void generarRecursoPorTurno(Personaje &pj, bool ataco, bool recibioDano) {
    if (pj.tipoRecurso == "Ira") {
        // El guerrero gana ira si ataca o si recibe daño
        int ganancia = 0;
        if (ataco) ganancia += 15;
        if (recibioDano) ganancia += 20;
        
        if (ganancia > 0) {
            pj.recursoActual = std::min(pj.recursoMax, pj.recursoActual + ganancia);
            std::cout << "[RECURSO] ¡Ganas " << ganancia << " de Ira! (Actual: " << pj.recursoActual << "/" << pj.recursoMax << ")\n";
        }
    } 
    else if (pj.tipoRecurso == "Mana") {
        // El mago regenera maná pasivamente cada turno
        int regeneracion = 12;
        pj.recursoActual = std::min(pj.recursoMax, pj.recursoActual + regeneracion);
        std::cout << "[RECURSO] Regeneras " << regeneracion << " de Maná. (Actual: " << pj.recursoActual << "/" << pj.recursoMax << ")\n";
    } 
    else if (pj.tipoRecurso == "Enfoque") {
        // El cazador recupera enfoque si usa su ataque básico o descansa
        if (ataco) {
            int recuperacion = 10;
            pj.recursoActual = std::min(pj.recursoMax, pj.recursoActual + recuperacion);
            std::cout << "[RECURSO] Recuperas " << recuperacion << " de Enfoque con tu disparo. (Actual: " << pj.recursoActual << "/" << pj.recursoMax << ")\n";
        }
    }
}