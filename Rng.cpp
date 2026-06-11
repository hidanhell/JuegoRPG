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

static RngSistema defaultRng;
static IAleatorio* instanciaActual = &defaultRng;

IAleatorio& Rng::get() {
    return *instanciaActual;
}

void Rng::establecer(IAleatorio* instancia) {
    instanciaActual = (instancia != nullptr) ? instancia : &defaultRng;
}
