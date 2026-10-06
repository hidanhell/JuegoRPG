#ifndef IENTRADA_H
#define IENTRADA_H

#include <string>

// =========================================================
// INTERFAZ GENERAL DE ENTRADA DE USUARIO
// =========================================================
class IEntrada {
public:
    virtual ~IEntrada() = default;

    // Lectura de tecla individual
    virtual char leerTecla() = 0;

    // Lectura de opciones numéricas
    virtual int leerOpcion() = 0;

    // Lectura de líneas completas de texto
    virtual std::string leerLinea() = 0;

    // Limpieza de buffer de entrada
    virtual void limpiarBuffer() = 0;
};

#endif // IENTRADA_H