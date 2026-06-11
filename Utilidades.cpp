#include "Utilidades.h"
#include "IO.h"
#include <cctype>

void mostrarBarraHP(int hp, int hpMax) {
    IO::render().mostrarBarraHP(hp, hpMax);
}

void mostrarCabecera(const std::string& titulo) {
    IO::render().mostrarCabecera(titulo);
}

void limpiarBuffer() {
    IO::entrada().limpiarBuffer();
}

char aMinuscula(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

void limpiarPantalla() {
    IO::render().limpiar();
}

void esperarTecla() {
    IO::render().esperarTecla();
}
