#ifndef IO_H
#define IO_H

#include "IRenderizador.h"
#include "IEntrada.h"
#include <string>

struct Personaje;

// =========================================================
// SERVICE LOCATOR DE I/O (Infraestructura)
// =========================================================
// Mantiene las instancias activas de renderizado y entrada.
// Por defecto utiliza la consola (ConsolaRenderizador / ConsolaEntrada).
// Para pruebas o interfaces alternativas: IO::establecerRender(&miRender).
namespace IO {
    IRenderizador& render();
    IEntrada&      entrada();
    void establecerRender(IRenderizador* instancia);
    void establecerEntrada(IEntrada* instancia);

    // Directorio base para guardar/cargar partidas. Por defecto "savegames"
    // (relativo al CWD, valido en desktop). En Android el CWD del proceso no
    // es escribible: el puente JNI llama establecerDirectorioGuardado() con
    // Context.getFilesDir() antes de arrancar el motor.
    std::string directorioGuardado();
    void establecerDirectorioGuardado(const std::string& dir);
}

#endif // IO_H