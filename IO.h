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
}

#endif // IO_H