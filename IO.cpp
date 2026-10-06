#include "IO.h"
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cctype>
#include <algorithm>
#include "Personajes.h"
#include "Consumibles.h"
#include "Recursos.h"
#include "SaveGame.h"
#include "CatalogoObjetos.h"

// =========================================================
// INFRAESTRUCTURA DE CONSOLA
// =========================================================

class ConsolaRenderizador : public IRenderizador {
public:
    void limpiar() override {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[H" << std::flush;
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
        for (int i = 0; i < LARGO; i++) {
            std::cout << (i < llenos ? "#" : ".");
        }
        std::cout << "] " << hp << "/" << hpMax << " HP\n";
    }

    void mostrarCabecera(const std::string& titulo) override {
        std::cout << "==== " << titulo << " ====\n";
    }

    void esperarTecla() override {
        std::cout << "Presiona ENTER para continuar...";
#ifdef _WIN32
    std::system("pause >nul");
#else
        std::cin.clear();
    if (std::cin.rdbuf()->in_avail() > 0) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::string linea;
    std::getline(std::cin, linea);
#endif
    }
};

class ConsolaEntrada : public IEntrada {
public:
    char leerTecla() override {
        char c;
        std::cin >> c;
        return c;
    }

    int leerOpcion() override {
        int n;
        std::cin >> n;
        return n;
    }

    std::string leerLinea() override {
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void limpiarBuffer() override {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

// =========================================================
// SERVICE LOCATOR (IO)
// =========================================================

static ConsolaRenderizador defaultRender;
static ConsolaEntrada      defaultEntrada;

static IRenderizador* renderActual  = &defaultRender;
static IEntrada*      entradaActual = &defaultEntrada;

IRenderizador& IO::render()  { return *renderActual; }
IEntrada&      IO::entrada() { return *entradaActual; }

void IO::establecerRender(IRenderizador* i)  { renderActual  = i ? i : &defaultRender; }
void IO::establecerEntrada(IEntrada* i)      { entradaActual = i ? i : &defaultEntrada; }

// =========================================================
// GUARDADO Y CARGA DE PARTIDAS
// =========================================================

static std::string escapeJson(const std::string &s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\b': out += "\\b"; break;
            case '\f': out += "\\f"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out += c; break;
        }
    }
    return out;
}

bool guardarPartida(const Personaje &p, int y, const std::string &ruta) {
    try {
        std::filesystem::path dir = std::filesystem::path(ruta).parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        std::ofstream f(ruta);
        if (!f.is_open()) return false;

        f << "{\n";
        f << "  \"save_version\": 2,\n";
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
        f << "  \"defensaBase\": " << p.defensaBase << ",\n";
        f << "  \"velocidadBase\": " << p.velocidadBase << ",\n";
        f << "  \"bonusCritico\": " << p.bonusCritico << ",\n";
        f << "  \"recursoActual\": " << p.recursoActual << ",\n";
        f << "  \"recursoMax\": " << p.recursoMax << ",\n";
        f << "  \"tipoRecurso\": \"" << escapeJson(p.tipoRecurso) << "\",\n";
        f << "  \"ultimateUsada\": " << (p.ultimateUsada ? 1 : 0) << ",\n";

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

                // Buffs temporales y duración restante
                f << "  \"buffs\": [\n";
                for (size_t i = 0; i < p.buffsActivos.size(); ++i) {
                        const auto& buff = p.buffsActivos[i];
                        f << "    {\"nombre\": \"" << escapeJson(buff.nombre)
                            << "\", \"ataque\": " << buff.bonusAtaque
                            << ", \"defensa\": " << buff.bonusDefensa
                            << ", \"velocidad\": " << buff.bonusVelocidad
                            << ", \"duracion\": " << buff.duracion << "}";
                        if (i + 1 < p.buffsActivos.size()) f << ",\n";
                        else f << "\n";
                }
                f << "  ],\n";

        // Equipo
        f << "  \"arma_id\": " << p.armaEquipada.id << ",\n";
        f << "  \"artefacto_id\": " << p.artefactoEquipado.id << ",\n";
        f << "  \"clase\": \"" << claseToString(p.clase) << "\",\n";

        // Reliquias
        f << "  \"reliquias\": [";
        for (size_t i = 0; i < p.reliquias.size(); ++i) {
            if (i) f << ", ";
            f << "\"" << escapeJson(p.reliquias[i]) << "\"";
        }
        f << "],\n";

        // Efectos activos
        f << "  \"efectos\": [\n";
        for (size_t i = 0; i < p.efectos.size(); ++i) {
            const auto &e = p.efectos[i];
            f << "    {\"id\": " << e.id << ", \"duracion\": " << e.duracion << ", \"intensidad\": " << e.intensidad << "}";
            if (i + 1 < p.efectos.size()) f << ",\n";
            else f << "\n";
        }
        f << "  ]\n";

        f << "}\n";
        f.close();
        return true;
    } catch (...) {
        return false;
    }
}

static std::string trim(const std::string &s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && isspace(static_cast<unsigned char>(s[b-1]))) --b;
    return s.substr(a, b - a);
}

bool cargarPartida(Personaje &p, int &y, const std::string &ruta) {
    std::ifstream f(ruta);
    if (!f.is_open()) return false;

    std::string line;
    p.inventario.clear();
    p.habilidadesIds.clear();
    p.reliquias.clear();
    p.efectos.clear();
    p.buffsActivos.clear();
    p.ultimateUsada = false;
    p.ultimoTurnoAtaco = false;
    p.turnosAgotado = 0;
    p.turnosEscudoCompanero = 0;
    bool defensaBaseCargada = false;
    bool recursoCargado = false;
    bool bonusCriticoCargado = false;

    while (std::getline(f, line)) {
        line = trim(line);
        if (line.find("\"posY\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) y = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"nombre\"") != std::string::npos) {
            auto first = line.find(':');
            if (first != std::string::npos) {
                auto q1 = line.find('"', first);
                auto q2 = line.find('"', q1 + 1);
                if (q1 != std::string::npos && q2 != std::string::npos) {
                    p.nombre = line.substr(q1 + 1, q2 - q1 - 1);
                }
            }
        } else if (line.find("\"nivel\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.nivel = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"hp\"") != std::string::npos && line.find("hpMax") == std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.hp = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"hpMax\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.hpMax = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"exp\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.exp = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"oro\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.oro = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"fuerza\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.fuerza = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"vitalidad\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.vitalidad = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"destreza\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.destreza = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"inteligencia\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.inteligencia = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"ataqueBase\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.ataqueBase = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"defensaBase\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                p.defensaBase = std::stoi(line.substr(pos + 1));
                defensaBaseCargada = true;
            }
        } else if (line.find("\"velocidadBase\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.velocidadBase = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"bonusCritico\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                p.bonusCritico = std::stoi(line.substr(pos + 1));
                bonusCriticoCargado = true;
            }
        } else if (line.find("\"recursoActual\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.recursoActual = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"recursoMax\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                p.recursoMax = std::stoi(line.substr(pos + 1));
                recursoCargado = true;
            }
        } else if (line.find("\"tipoRecurso\"") != std::string::npos) {
            auto colon = line.find(':');
            auto q1 = line.find('"', colon);
            auto q2 = line.find('"', q1 + 1);
            if (q1 != std::string::npos && q2 != std::string::npos) {
                p.tipoRecurso = line.substr(q1 + 1, q2 - q1 - 1);
                recursoCargado = true;
            }
        } else if (line.find("\"ultimateUsada\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.ultimateUsada = std::stoi(line.substr(pos + 1)) != 0;
        } else if (line.find("\"habilidades\"") != std::string::npos) {
            std::string arr = line;
            while (arr.find(']') == std::string::npos && std::getline(f, line)) arr += line;
            for (size_t i = 0; i < arr.size(); ++i) {
                if (isdigit(static_cast<unsigned char>(arr[i]))) {
                    size_t j = i;
                    while (j < arr.size() && isdigit(static_cast<unsigned char>(arr[j]))) ++j;
                    p.habilidadesIds.push_back(std::stoi(arr.substr(i, j - i)));
                    i = j;
                }
            }
        } else if (line.find("\"inventario\"") != std::string::npos) {
            while (std::getline(f, line)) {
                if (line.find(']') != std::string::npos) break;
                auto idpos = line.find("\"id\"");
                if (idpos != std::string::npos) {
                    auto colon = line.find(':', idpos);
                    int id = std::stoi(line.substr(colon + 1));
                    std::string nombre;
                    auto namepos = line.find("\"nombre\"");
                    if (namepos != std::string::npos) {
                        auto q1 = line.find('"', namepos + 8);
                        auto q2 = line.find('"', q1 + 1);
                        if (q1 != std::string::npos && q2 != std::string::npos) {
                            nombre = line.substr(q1 + 1, q2 - q1 - 1);
                        }
                    } else if (std::getline(f, line)) {
                        auto np = line.find("\"nombre\"");
                        if (np != std::string::npos) {
                            auto q1 = line.find('"', np + 8);
                            auto q2 = line.find('"', q1 + 1);
                            if (q1 != std::string::npos && q2 != std::string::npos) {
                                nombre = line.substr(q1 + 1, q2 - q1 - 1);
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
            auto pos = line.find(':');
            if (pos != std::string::npos) p.armaEquipada.id = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"artefacto_id\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) p.artefactoEquipado.id = std::stoi(line.substr(pos + 1));
        } else if (line.find("\"clase\"") != std::string::npos) {
            auto pos = line.find(':');
            if (pos != std::string::npos) {
                auto q1 = line.find('"', pos);
                auto q2 = line.find('"', q1 + 1);
                if (q1 != std::string::npos && q2 != std::string::npos) {
                    std::string valor = line.substr(q1 + 1, q2 - q1 - 1);
                    if (auto clsOpt = claseFromString(valor)) {
                        p.clase = *clsOpt;
                    }
                }
            }
        } else if (line.find("\"reliquias\"") != std::string::npos) {
            std::string arr = line;
            while (arr.find(']') == std::string::npos && std::getline(f, line)) arr += line;
            size_t colon = arr.find(':');
            if (colon != std::string::npos) arr = arr.substr(colon + 1);
            
            for (size_t i = 0; i < arr.size(); ++i) {
                if (arr[i] == '"') {
                    size_t j = i + 1;
                    while (j < arr.size() && arr[j] != '"') ++j;
                    if (j < arr.size()) {
                        p.reliquias.push_back(arr.substr(i + 1, j - (i + 1)));
                        i = j;
                    }
                }
            }
        } else if (line.find("\"buffs\"") != std::string::npos) {
            std::string block = line;
            while (block.find(']') == std::string::npos && std::getline(f, line)) block += line;
            size_t idx = 0;
            while (true) {
                const auto start = block.find('{', idx);
                if (start == std::string::npos) break;
                const auto end = block.find('}', start);
                if (end == std::string::npos) break;
                const std::string object = block.substr(start + 1, end - start - 1);
                BuffActivo buff;
                const auto readInt = [&object](const std::string& key) {
                    const auto keyPos = object.find("\"" + key + "\"");
                    if (keyPos == std::string::npos) return 0;
                    const auto colon = object.find(':', keyPos);
                    return colon == std::string::npos ? 0 : std::stoi(object.substr(colon + 1));
                };
                const auto nombrePos = object.find("\"nombre\"");
                if (nombrePos != std::string::npos) {
                    const auto colon = object.find(':', nombrePos);
                    const auto q1 = object.find('"', colon + 1);
                    const auto q2 = object.find('"', q1 + 1);
                    if (q1 != std::string::npos && q2 != std::string::npos) {
                        buff.nombre = object.substr(q1 + 1, q2 - q1 - 1);
                    }
                }
                buff.bonusAtaque = readInt("ataque");
                buff.bonusDefensa = readInt("defensa");
                buff.bonusVelocidad = readInt("velocidad");
                buff.duracion = readInt("duracion");
                p.buffsActivos.push_back(buff);
                idx = end + 1;
            }
        } else if (line.find("\"efectos\"") != std::string::npos) {
            std::string block;
            while (line.find(']') == std::string::npos && std::getline(f, line)) block += line;
            size_t idx = 0;
            while (true) {
                auto objStart = block.find('{', idx);
                if (objStart == std::string::npos) break;
                auto objEnd = block.find('}', objStart);
                if (objEnd == std::string::npos) break;
                std::string obj = block.substr(objStart + 1, objEnd - objStart - 1);
                int id = 0, dur = 0, inten = 0;
                
                for (size_t k = 0; k < obj.size(); ++k) {
                    if (obj.compare(k, 3, "id\"") == 0 || obj.compare(k, 2, "id") == 0) {
                        auto col = obj.find(':', k);
                        if (col != std::string::npos) id = std::stoi(obj.substr(col + 1));
                    }
                    if (obj.find("duracion", k) == k) {
                        auto col = obj.find(':', k);
                        if (col != std::string::npos) dur = std::stoi(obj.substr(col + 1));
                    }
                    if (obj.find("intensidad", k) == k) {
                        auto col = obj.find(':', k);
                        if (col != std::string::npos) inten = std::stoi(obj.substr(col + 1));
                    }
                }
                
                auto efOpt = obtenerEfectoPorId(id);
                Efecto ef;
                if (efOpt) ef = *efOpt;
                ef.id = id;
                ef.duracion = dur;
                ef.intensidad = inten;
                agregarEfectoActivo(p.efectos, ef);
                idx = objEnd + 1;
            }
        }
    }

    f.close();

    // Rehidratación de objetos equipados mediante catálogos
    if (auto armaOpt = obtenerArmaPorId(p.armaEquipada.id)) {
        p.armaEquipada = *armaOpt;
    }
    if (auto arteOpt = obtenerArtefactoPorId(p.artefactoEquipado.id)) {
        p.artefactoEquipado = *arteOpt;
    }

    if (!defensaBaseCargada) {
        const int defensaInicial = p.clase == Clase::Guerrero ? 28 : p.clase == Clase::Mago ? 4 : 6;
        p.defensaBase = defensaInicial + (std::max(1, p.nivel) - 1) * 3;
        if (std::find(p.habilidadesIds.begin(), p.habilidadesIds.end(), 1) != p.habilidadesIds.end() ||
            std::find(p.habilidadesIds.begin(), p.habilidadesIds.end(), 11) != p.habilidadesIds.end()) {
            p.defensaBase += 5;
        }
        if (std::find(p.habilidadesIds.begin(), p.habilidadesIds.end(), 201) != p.habilidadesIds.end()) {
            p.defensaBase = static_cast<int>(p.defensaBase * 1.40f);
        } else if (std::find(p.habilidadesIds.begin(), p.habilidadesIds.end(), 212) != p.habilidadesIds.end()) {
            p.defensaBase = static_cast<int>(p.defensaBase * 1.35f);
        }
        if (std::find(p.reliquias.begin(), p.reliquias.end(), "Medallon del Guardian") != p.reliquias.end()) {
            p.defensaBase += 10;
        }
    }
    if (!recursoCargado) inicializarRecurso(p);
    if (!bonusCriticoCargado && std::find(p.reliquias.begin(), p.reliquias.end(), "Anillo del Asesino") != p.reliquias.end()) {
        p.bonusCritico = 5;
    }

    p.tieneHabilidad10 = std::any_of(p.habilidadesIds.begin(), p.habilidadesIds.end(), [](int id) {
        return (id >= 101 && id <= 103) || (id >= 111 && id <= 113) || (id >= 121 && id <= 123);
    });
    p.tieneSubclase15 = std::any_of(p.habilidadesIds.begin(), p.habilidadesIds.end(), [](int id) { return id >= 201 && id <= 222; });
    p.subclaseAplicada = p.tieneSubclase15;
    p.esBerserker = std::find(p.habilidadesIds.begin(), p.habilidadesIds.end(), 202) != p.habilidadesIds.end();
    p.tieneUltimate20 = std::any_of(p.habilidadesIds.begin(), p.habilidadesIds.end(), [](int id) { return id >= 311 && id <= 332; });
    if (p.hp > p.hpMax) p.hp = p.hpMax;
    return true;
}

void nuevaPartida(Personaje &p, int yInicial) {
    p = Personaje("Hero", 0);
    p.nombre = "Hero";
    p.nivel = 1;
    p.exp = 0;
    p.oro = 0;
    p.fuerza = 5;
    p.vitalidad = 5;
    p.destreza = 5;
    p.inteligencia = 3;
    p.actualizarEstadisticas();
    p.hp = p.hpMax;
    p.habilidadesIds.clear();
    p.inventario.clear();
    p.posY = yInicial;
}