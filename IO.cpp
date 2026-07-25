#include "IO.h"
#include <iostream>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Personajes.h"
#include "Consumibles.h"
#include "SaveGame.h"

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

// ----------------------------
// Guardado / Carga de partidas
// ----------------------------

static std::string escapeJson(const std::string &s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c; break;
        }
    }
    return out;
}

bool guardarPartida(const Personaje &p, int y, const std::string &ruta) {
    try {
        std::filesystem::path dir = std::filesystem::path(ruta).parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) std::filesystem::create_directories(dir);

        std::ofstream f(ruta);
        if (!f.is_open()) return false;

        f << "{\n";
        f << "  \"save_version\": 1,\n";
        f << "  \"posY\": " << y << ",\n";
        f << "  \"nombre\": \"" << escapeJson(p.nombre) << "\",\n";
        f << "  \"nivel\": " << p.nivel << ",\n";
        f << "  \"hp\": " << p.hp << ",\n";
        f << "  \"hpMax\": " << p.hpMax << ",\n";
        f << "  \"exp\": " << p.exp << ",\n";
        f << "  \"oro\": " << p.oro << ",\n";
        f << "  \"fuerza\": " << p.fuerza << ",\n";
        f << "  \"vitalidad\": " << p.vitalidad << ",\n";
        f << "  \"destreza\": " << p.destreza << ",\n";
        f << "  \"inteligencia\": " << p.inteligencia << ",\n";
        f << "  \"ataqueBase\": " << p.ataqueBase << ",\n";
        f << "  \"velocidadBase\": " << p.velocidadBase << ",\n";

        // Habilidades
        f << "  \"habilidades\": [";
        for (size_t i = 0; i < p.habilidadesIds.size(); ++i) {
            if (i) f << ", ";
            f << p.habilidadesIds[i];
        }
        f << "],\n";

        // Inventario
        f << "  \"inventario\": [\n";
        for (size_t i = 0; i < p.inventario.size(); ++i) {
            const auto &it = p.inventario[i];
            f << "    {\"id\": " << it.id << ", \"nombre\": \"" << escapeJson(it.nombre) << "\"}";
            if (i + 1 < p.inventario.size()) f << ",\n";
            else f << "\n";
        }
        f << "  ],\n";

        // Equipo (arma id)
        f << "  \"arma_id\": " << p.armaEquipada.id << "\n";

        f << "}\n";
        f.close();
        return true;
    } catch (...) {
        return false;
    }
}

static std::string trim(const std::string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

bool cargarPartida(Personaje &p, int &y, const std::string &ruta) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;
    std::string line;
    p.inventario.clear();
    p.habilidadesIds.clear();

    while (std::getline(f, line)) {
        line = trim(line);
        if (line.find("\"posY\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) y = std::stoi(line.substr(pos+1));
        } else if (line.find("\"nombre\"") != std::string::npos) {
            auto first = line.find('"');
            auto second = line.find('"', first+1);
            auto third = line.find('"', second+1);
            auto fourth = line.find('"', third+1);
            if (third!=std::string::npos && fourth!=std::string::npos) p.nombre = line.substr(third+1, fourth-third-1);
        } else if (line.find("\"nivel\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.nivel = std::stoi(line.substr(pos+1));
        } else if (line.find("\"hp\"") != std::string::npos && line.find("hpMax") == std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.hp = std::stoi(line.substr(pos+1));
        } else if (line.find("\"hpMax\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.hpMax = std::stoi(line.substr(pos+1));
        } else if (line.find("\"exp\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.exp = std::stoi(line.substr(pos+1));
        } else if (line.find("\"oro\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.oro = std::stoi(line.substr(pos+1));
        } else if (line.find("\"fuerza\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.fuerza = std::stoi(line.substr(pos+1));
        } else if (line.find("\"vitalidad\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.vitalidad = std::stoi(line.substr(pos+1));
        } else if (line.find("\"destreza\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.destreza = std::stoi(line.substr(pos+1));
        } else if (line.find("\"inteligencia\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.inteligencia = std::stoi(line.substr(pos+1));
        } else if (line.find("\"ataqueBase\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.ataqueBase = std::stoi(line.substr(pos+1));
        } else if (line.find("\"velocidadBase\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.velocidadBase = std::stoi(line.substr(pos+1));
        } else if (line.find("\"habilidades\"") != std::string::npos) {
            // read array line(s) until ]
            std::string arr; while (line.find(']') == std::string::npos && std::getline(f, line)) { arr += line; }
            // extract numbers
            for (size_t i = 0; i < arr.size(); ++i) {
                if (isdigit((unsigned char)arr[i])) {
                    size_t j = i; while (j < arr.size() && isdigit((unsigned char)arr[j])) ++j;
                    p.habilidadesIds.push_back(std::stoi(arr.substr(i, j - i)));
                    i = j;
                }
            }
        } else if (line.find("\"inventario\"") != std::string::npos) {
            // read inventory objects until ]
            while (std::getline(f, line)) {
                if (line.find(']') != std::string::npos) break;
                auto idpos = line.find("\"id\"");
                if (idpos != std::string::npos) {
                    auto colon = line.find(':', idpos);
                    int id = std::stoi(line.substr(colon+1));
                    // find nombre in same or next line
                    std::string nombre;
                    auto namepos = line.find("\"nombre\"");
                    if (namepos != std::string::npos) {
                        auto q1 = line.find('"', namepos+8);
                        auto q2 = line.find('"', q1+1);
                        if (q1!=std::string::npos && q2!=std::string::npos) nombre = line.substr(q1+1, q2-q1-1);
                    } else {
                        // read next line for nombre
                        if (std::getline(f, line)) {
                            auto np = line.find("\"nombre\"");
                            if (np!=std::string::npos) {
                                auto q1 = line.find('"', np+8);
                                auto q2 = line.find('"', q1+1);
                                if (q1!=std::string::npos && q2!=std::string::npos) nombre = line.substr(q1+1, q2-q1-1);
                            }
                        }
                    }
                    Consumible it;
                    it.id = id;
                    it.nombre = nombre;
                    p.inventario.push_back(it);
                }
            }
        } else if (line.find("\"arma_id\"") != std::string::npos) {
            auto pos = line.find(':'); if (pos!=std::string::npos) p.armaEquipada.id = std::stoi(line.substr(pos+1));
        }
    }

    f.close();
    // After loading, recalc derived stats
    p.actualizarEstadisticas();
    if (p.hp > p.hpMax) p.hp = p.hpMax;
    return true;
}

void nuevaPartida(Personaje &p, int yInicial) {
    p = Personaje("Hero", 0);
    p.nombre = "Hero";
    p.nivel = 1;
    p.exp = 0;
    p.oro = 0;
    p.fuerza = 5; p.vitalidad = 5; p.destreza = 5; p.inteligencia = 3;
    p.actualizarEstadisticas();
    p.hp = p.hpMax;
    p.habilidadesIds.clear();
    p.inventario.clear();
    p.posY = yInicial;
}
