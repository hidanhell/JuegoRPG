#include "Rng.h"
#include "IAleatorio.h"
#include <random>
#include <ctime>

class RngSistema : public IAleatorio {
    std::mt19937 motor;
public:
    RngSistema() : motor(static_cast<unsigned>(std::time(nullptr))) {}

    int entre(int min, int max) override {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(motor);
    }

    bool probabilidad(int porcentaje) override {
        return entre(0, 99) < porcentaje;
    }
};

// Static locals (no globales de scope de archivo): el orden de construccion
// de globales ENTRE distintos .cpp no esta definido en C++. listaArmas
// (CatalogoObjetos.cpp) es un global cuyo constructor llama Rng::get() para
// las armas "Raro" (efecto aleatorio); si su .cpp se inicializaba antes que
// este, defaultRng/instanciaActual (como globales de archivo) podian estar
// sin construir todavia -> null deref. Los static locales se inicializan de
// forma perezosa en el primer uso real, sin importar el orden entre TUs.
static RngSistema& defaultRngInstancia() {
    static RngSistema instancia;
    return instancia;
}

static IAleatorio*& instanciaActualRef() {
    static IAleatorio* instanciaActual = &defaultRngInstancia();
    return instanciaActual;
}

IAleatorio& Rng::get() {
    return *instanciaActualRef();
}

void Rng::establecer(IAleatorio* instancia) {
    instanciaActualRef() = (instancia != nullptr) ? instancia : &defaultRngInstancia();
}
