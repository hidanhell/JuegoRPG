#ifndef HABILIDADES_MONSTRUO_H
#define HABILIDADES_MONSTRUO_H

#include <string>
#include <vector>
#include <optional>

// Declaraciones adelantadas (Forward Declarations)
// Esto elimina los warnings de "tipo desconocido" sin necesidad de saturar con #includes
struct Monstruo; 
struct Personaje;

// =========================================================
// HABILIDADES DE MONSTRUOS
// Separadas de las habilidades del jugador
// =========================================================

struct HabilidadMonstruo {
    int id;
    std::string nombre;
    std::string descripcion;
    std::string tipo;       // "Fisico", "Magico", "Estado", "Huida", "Buff"
    int efectoId;           // ID del efecto a aplicar (0 = ninguno)
    float multiDano;        // Multiplicador de dano (0 = sin dano)
};

// IDs de habilidades de monstruo
// M01-M06: Habilidades genericas para Raros
// M10+:    Habilidades unicas para Elites (futuro)

// Acceso al catalogo. Devuelve std::nullopt si no existe.
std::optional<HabilidadMonstruo> obtenerHabilidadMonstruo(int id);

// Ejecucion de habilidad en combate
// Retorna true si el monstruo huyo exitosamente
// He quitado la palabra 'struct' de los parametros porque en C++ moderno ya no es necesaria si el tipo existe
bool ejecutarHabilidadMonstruo(Monstruo &m, Personaje &p, int idHabilidad);

// Asignar habilidad segun tipo de monstruo y rareza
void asignarHabilidadesMonstruo(Monstruo &m);

// El catalogo global de habilidades
extern std::vector<HabilidadMonstruo> listaHabilidadesMonstruo;

#endif