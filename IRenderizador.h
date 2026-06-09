#ifndef IRENDERIZADOR_H
#define IRENDERIZADOR_H

#include <string>

class IRenderizador {
public:
    virtual ~IRenderizador() = default;
    virtual void limpiar() = 0;
    virtual void escribir(const std::string& texto) = 0;
    virtual void mostrarBarraHP(int hp, int hpMax) = 0;
    virtual void mostrarCabecera(const std::string& titulo) = 0;
    virtual void esperarTecla() = 0;
};

#endif
