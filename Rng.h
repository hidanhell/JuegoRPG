#ifndef RNG_H
#define RNG_H

#include "IAleatorio.h"

// Service locator para el RNG. Por defecto usa RngSistema (mt19937).
// En tests: Rng::establecer(&miRngFalso) para resultados deterministas.
namespace Rng {
    IAleatorio& get();
    void establecer(IAleatorio* instancia); // nullptr restaura el default
}

#endif
