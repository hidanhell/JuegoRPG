#include "Efectos.h"
#include <vector>

// =========================================================
// CATÁLOGO DE EFECTOS COMUNES
// Formato: {ID, Nombre, Tipo, Duración (Turnos), Intensidad}
// =========================================================
std::vector<Efecto> listaEfectos = {
    {1, "Quemadura", "Ofensivo", 3, 10},         // 10% del HP max por turno
    {2, "Veneno", "Ofensivo", 4, 8},             // 8% del HP max por turno
    {3, "Sangrado", "Ofensivo", 2, 15},          // 15 de daño fijo por turno
    {4, "Paralisis", "Estado", 2, 30},           // 30% prob. de perder turno
    {5, "Evasion", "Defensivo", 1, 25},          // 25% prob. de esquivar ataque
    {6, "Congelacion", "Estado", 2, 40},         // 40% prob. de perder turno
    {7, "Desorientacion Arcana", "Estado", 2, 20} // 20% prob. de perder turno
};

// =========================================================
// CATÁLOGO DE EFECTOS LEGENDARIOS
// =========================================================
std::vector<Efecto> listaEfectosLegendarios = {
    {201, "Corte del Rey Dragon", "Ofensivo", 1, 110}, // Excalibur: 110% daño
    {202, "Supernova", "Ofensivo", 1, 90},             // Bástón: 90% daño
    {203, "Flecha Divina", "Ofensivo", 1, 90},         // Arco: 90% daño
    {204, "Dios del Caos", "Estado", 1, 25}            // Lancelot: 25% parálisis/curación
};