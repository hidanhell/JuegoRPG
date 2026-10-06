#ifndef HABILIDADES_MONSTRUO_H
#define HABILIDADES_MONSTRUO_H

#include <string>
#include <vector>
#include <optional>
#include <utility>

// Declaraciones adelantadas para evitar dependencias circulares
struct Monstruo; 
struct Personaje;

// =========================================================
// ESTRUCTURA DE HABILIDAD DE MONSTRUO
// =========================================================
struct HabilidadMonstruo {
    int id;
    std::string nombre;
    std::string descripcion;
    std::string tipo;       // "Fisico", "Magico", "Estado", "Huida", "Buff", "Vampirismo", "Sagrado"
    int efectoId;           // ID del efecto a aplicar (0 = ninguno)
    float multiDano;        // Multiplicador sobre el ataque base (0.0f = sin daño directo)

    HabilidadMonstruo(int i = 0, std::string n = "-", std::string d = "", 
                      std::string t = "Fisico", int ef = 0, float multi = 0.0f)
        : id(i), nombre(std::move(n)), descripcion(std::move(d)), 
          tipo(std::move(t)), efectoId(ef), multiDano(multi) {}
};

// Catálogo global de habilidades de enemigos
extern std::vector<HabilidadMonstruo> listaHabilidadesMonstruo;

// =========================================================
// CONSULTAS Y LÓGICA DE EJECUCIÓN
// =========================================================
std::optional<HabilidadMonstruo> obtenerHabilidadMonstruo(int id);
void asignarHabilidadesMonstruo(Monstruo &m);

// Ejecuta la habilidad en combate. Retorna true si el enemigo huye con éxito.
bool ejecutarHabilidadMonstruo(Monstruo &m, Personaje &p, int idHabilidad);

#endif // HABILIDADES_MONSTRUO_H