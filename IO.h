#ifndef IO_H
#define IO_H

#include "IRenderizador.h"
#include "IEntrada.h"

// Service locator para I/O. Por defecto usa ConsolaRenderizador y ConsolaEntrada.
// Para tests: IO::establecerRender(&miRenderFalso) / IO::establecerEntrada(&miEntradaFalsa).
// Pasar nullptr restaura la implementacion de consola.
namespace IO {
    IRenderizador& render();
    IEntrada&      entrada();
    void establecerRender(IRenderizador* instancia);
    void establecerEntrada(IEntrada* instancia);
}

#endif
