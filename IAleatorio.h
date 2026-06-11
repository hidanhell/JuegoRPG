#ifndef IALEATORIO_H
#define IALEATORIO_H

class IAleatorio {
public:
    virtual ~IAleatorio() = default;
    virtual int entre(int min, int max) = 0;      // [min, max] inclusive
    virtual bool probabilidad(int porcentaje) = 0; // true con porcentaje% de chance
};

#endif
