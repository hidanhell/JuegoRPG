#include <iostream>
#include <filesystem>
#include "SaveGame.h"
#include "Personajes.h"
#include "Efectos.h"

int main() {
    Personaje p1("Tester", 0);
    p1.nombre = "Tester";
    p1.clase = Clase::Guerrero;
    p1.nivel = 20;
    p1.fuerza = 12;
    p1.vitalidad = 8;
    p1.posY = 42;
    p1.armaEquipada.id = 1;
    p1.artefactoEquipado.id = 2;
    p1.reliquias.push_back("Colmillo de Vampiro");
    p1.habilidadesIds = {102, 202, 312};
    p1.ultimateUsada = true;
    p1.buffsActivos.push_back({"Elixir de prueba", 5, 3, 2, 2});
    p1.efectos.push_back(Efecto(10, "Veneno", "Ofensivo", 3, 2));

    p1.actualizarEstadisticas();
    p1.hpMax = 321;
    p1.hp = 280;
    p1.ataqueBase = 155;
    p1.defensaBase = 77;
    p1.velocidadBase = 44;
    p1.bonusCritico = 7;
    p1.recursoActual = 45;
    p1.recursoMax = 100;
    p1.tipoRecurso = "Ira";

    const std::string ruta = (std::filesystem::temp_directory_path() / "juegorp_roundtrip_test.json").string();
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
    ok &= (p1.hpMax == p2.hpMax && p1.hp == p2.hp);
    ok &= (p1.ataqueBase == p2.ataqueBase && p1.defensaBase == p2.defensaBase);
    ok &= (p1.velocidadBase == p2.velocidadBase && p1.bonusCritico == p2.bonusCritico);
    ok &= (p1.recursoActual == p2.recursoActual && p1.tipoRecurso == p2.tipoRecurso);
    ok &= (p2.ultimateUsada && p2.tieneUltimate20 && p2.tieneSubclase15 && p2.subclaseAplicada && p2.esBerserker);
    ok &= (p2.buffsActivos.size() == 1 && p2.buffsActivos.front().duracion == 2);

    if (ok) std::cout << "TEST OK: Guardar/Cargar roundtrip válido" << std::endl;
    else std::cout << "TEST FAIL: Discrepancia en roundtrip" << std::endl;

    std::filesystem::remove(ruta);
    return ok ? 0 : 1;
}
