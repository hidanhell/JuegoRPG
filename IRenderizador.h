#ifndef IRENDERIZADOR_H
#define IRENDERIZADOR_H

#include <string>

// =========================================================
// INTERFAZ GENERAL DE RENDERIZADO
// =========================================================
class IRenderizador {
public:
    virtual ~IRenderizador() = default;

    // Limpieza de pantalla
    virtual void limpiar() = 0;

    // Emisión de texto
    virtual void escribir(const std::string& texto) = 0;

    // Renderizado de elementos de interfaz
    virtual void mostrarBarraHP(int hp, int hpMax) = 0;
    virtual void mostrarCabecera(const std::string& titulo) = 0;

    // Pausa de interacción
    virtual void esperarTecla() = 0;
};

#endif // IRENDERIZADOR_H