#ifndef RECURSOS_H
#define RECURSOS_H

#include <string>

// Declaración anticipada para evitar dependencias circulares
struct Personaje;

// Funciones del sistema de recursos
void inicializarRecurso(Personaje &pj);
std::string obtenerNombreRecurso(const Personaje &pj);
bool gastarRecurso(Personaje &pj, int costo);
void generarRecursoPorTurno(Personaje &pj, bool ataco, bool recibioDano);

#endif // RECURSOS_H