#include "IO.h"
#include <iostream>
#include <limits>
#include <string>

// =========================================================
// Implementaciones de consola (Infrastructure layer)
// =========================================================

class ConsolaRenderizador : public IRenderizador {
public:
    void limpiar() override {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void escribir(const std::string& texto) override {
        std::cout << texto;
    }

    void mostrarBarraHP(int hp, int hpMax) override {
        const int LARGO = 20;
        int llenos = (hpMax > 0) ? (hp * LARGO / hpMax) : 0;
        if (llenos < 0) llenos = 0;
        if (llenos > LARGO) llenos = LARGO;

        std::cout << "[";
        for (int i = 0; i < LARGO; i++)
            std::cout << (i < llenos ? "#" : ".");
        std::cout << "] " << hp << "/" << hpMax << " HP" << std::endl;
    }

    void mostrarCabecera(const std::string& titulo) override {
        std::cout << "==== " << titulo << " ====" << std::endl;
    }

    void esperarTecla() override {
        std::cout << "Presiona ENTER para continuar...";
#ifdef _WIN32
        system("pause >nul");
#else
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
#endif
    }
};

class ConsolaEntrada : public IEntrada {
public:
    char leerTecla() override {
        char c; std::cin >> c; return c;
    }

    int leerOpcion() override {
        int n; std::cin >> n; return n;
    }

    std::string leerLinea() override {
        std::string s; std::getline(std::cin, s); return s;
    }

    void limpiarBuffer() override {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// =========================================================
// Service locator
// =========================================================

static ConsolaRenderizador defaultRender;
static ConsolaEntrada      defaultEntrada;

static IRenderizador* renderActual  = &defaultRender;
static IEntrada*      entradaActual = &defaultEntrada;

IRenderizador& IO::render()  { return *renderActual; }
IEntrada&      IO::entrada() { return *entradaActual; }

void IO::establecerRender(IRenderizador* i)  { renderActual  = i ? i : &defaultRender; }
void IO::establecerEntrada(IEntrada* i)      { entradaActual = i ? i : &defaultEntrada; }
