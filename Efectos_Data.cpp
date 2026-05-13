#include "Efectos.h"
#include <vector>

// Catálogo de efectos comunes
std::vector<Efecto> listaEfectos = {
    {1, "Quemadura", "Ofensivo", 3, 10},   // 3 turnos, 10% del HP máximo por turno
    {2, "Veneno", "Ofensivo", 4, 8},      // 4 turnos, 8% del HP máximo por turno
    {3, "Sangrado", "Ofensivo", 2, 15},   // 2 turnos, 15 de daño fijo por turno
    {4, "Paralisis", "Estado", 2, 30},    // 2 turnos, 30% de probabilidad de perder turno
    {5, "Evasion", "Defensivo", 1, 25},    // 1 turno, 25% de probabilidad de esquivar ataque
    {6, "Congelacion", "Estado", 2, 40},  // 2 turnos, 40% de probabilidad de perder turno (similar a paralisis pero temática hielo)
    {7, "Desorientacion Arcana", "Estado", 2, 20} // 2 turnos, -20% precisión o intelecto
};

// Catálogo de efectos legendarios
std::vector<Efecto> listaEfectosLegendarios = {
    {201, "Corte del Rey Dragon", "Ofensivo", 1, 110},   // Excalibur: 110% ataque guerrero
    {202, "Supernova", "Ofensivo", 1, 90},               // Bastón: 90% ataque mago
    {203, "Flecha Divina", "Ofensivo", 1, 90},           // Arco: 90% ataque cazador
    {204, "Dios del Caos", "Estado", 1, 25}              // Lancelot: 25% paralisis + 25% curación
};