#ifndef IENTRADA_H
#define IENTRADA_H

#include <string>

class IEntrada {
public:
    virtual ~IEntrada() = default;
    virtual char leerTecla() = 0;
    virtual int leerOpcion() = 0;
    virtual std::string leerLinea() = 0;
    virtual void limpiarBuffer() = 0;
};

#endif
