#ifndef IALEATORIO_H
#define IALEATORIO_H

// =========================================================
// INTERFAZ GENERAL DE GENERACIÓN ALEATORIA (RNG)
// =========================================================
class IAleatorio {
public:
    virtual ~IAleatorio() = default;

    // Retorna un entero uniforme en el rango [min, max] inclusive
    virtual int entre(int min, int max) = 0;

    // Evaluacion booleana con una probabilidad dada (0 a 100%)
    virtual bool probabilidad(int porcentaje) = 0;
};

#endif // IALEATORIO_H