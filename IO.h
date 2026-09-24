#ifndef IO_H
#define IO_H

#include "IRenderizador.h"
#include "IEntrada.h"
#include <string>
struct Personaje;

// Service locator para I/O. Por defecto usa ConsolaRenderizador y ConsolaEntrada.
// Para tests: IO::establecerRender(&miRenderFalso) / IO::establecerEntrada(&miEntradaFalsa).
// Pasar nullptr restaura la implementacion de consola.
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

    // Guardado / Carga de partidas moved to SaveGame.h
}

#endif
