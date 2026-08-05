#include <iostream>
#include "SaveGame.h"
#include "Personajes.h"
#include "Efectos.h"

int main() {
    Personaje p1("Tester", 0);
    p1.nombre = "Tester";
    p1.clase = Clase::Guerrero;
    p1.nivel = 5;
    p1.fuerza = 12;
    p1.vitalidad = 8;
    p1.posY = 42;
    p1.armaEquipada.id = 1;
    p1.artefactoEquipado.id = 2;
    p1.reliquias.push_back("Colmillo de Vampiro");
    p1.efectos.push_back(Efecto(10, "Veneno", "Ofensivo", 3, 2));

    p1.actualizarEstadisticas();

    const std::string ruta = "output/test_roundtrip.json";
    if (!guardarPartida(p1, p1.posY, ruta)) {
        std::cout << "Fallo al guardar partida" << std::endl;
        return 2;
    }

    Personaje p2("Copia", 0);
    int y = 0;
    if (!cargarPartida(p2, y, ruta)) {
        std::cout << "Fallo al cargar partida" << std::endl;
        return 3;
    }

    std::cout << "--- Valores originales ---\n";
    std::cout << "clase: " << claseToString(p1.clase) << " | arma_id: " << p1.armaEquipada.id << " | art_id: " << p1.artefactoEquipado.id << " | reliquias: " << p1.reliquias.size() << " | efectos: " << p1.efectos.size() << "\n";
    std::cout << "--- Valores cargados ---\n";
    std::cout << "clase: " << claseToString(p2.clase) << " | arma_id: " << p2.armaEquipada.id << " | art_id: " << p2.artefactoEquipado.id << " | reliquias: " << p2.reliquias.size() << " | efectos: " << p2.efectos.size() << "\n";

    bool ok = true;
    ok &= (p1.clase == p2.clase);
    ok &= (p1.armaEquipada.id == p2.armaEquipada.id);
    ok &= (p1.artefactoEquipado.id == p2.artefactoEquipado.id);
    ok &= (!p2.reliquias.empty());
    ok &= (!p2.efectos.empty());

    if (ok) std::cout << "TEST OK: Guardar/Cargar roundtrip válido" << std::endl;
    else std::cout << "TEST FAIL: Discrepancia en roundtrip" << std::endl;

    return ok ? 0 : 1;
}
