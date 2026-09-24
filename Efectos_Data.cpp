#include "Efectos.h"
#include <vector>

// =========================================================
// CATÁLOGO DE EFECTOS COMUNES
// Formato: {ID, Nombre, Tipo, Duracion (Turnos), Intensidad}
// =========================================================
std::vector<Efecto> listaEfectos = {
    {1, "Quemadura", "Ofensivo", 3, 10},   // 3 turnos, 10% del HP máximo de daño por turno
    {2, "Veneno", "Ofensivo", 4, 8},       // 4 turnos, 8% del HP máximo de daño por turno
    {3, "Sangrado", "Ofensivo", 2, 15},    // 2 turnos, 15 de daño fijo por turno
    {4, "Paralisis", "Estado", 2, 30},     // 2 turnos, 30% de probabilidad de perder turno
    {5, "Evasion", "Defensivo", 1, 25},    // 1 turno, 25% de probabilidad de esquivar ataque
    {6, "Congelacion", "Estado", 2, 40},   // 2 turnos, 40% de probabilidad de perder turno
    {7, "Desorientacion Arcana", "Estado", 2, 20} // 2 turnos, 20% de probabilidad de perder turno
};

// =========================================================
// CATÁLOGO DE EFECTOS LEGENDARIOS
// =========================================================
std::vector<Efecto> listaEfectosLegendarios = {
    {201, "Corte del Rey Dragon", "Ofensivo", 1, 110},   // Excalibur: 110% de daño según el ataque
    {202, "Supernova", "Ofensivo", 1, 90},               // Bastón: 90% de daño según el ataque
    {203, "Flecha Divina", "Ofensivo", 1, 90},           // Arco: 90% de daño según el ataque
    {204, "Dios del Caos", "Estado", 1, 25}              // Lancelot: 25% paralisis + 25% curación
};