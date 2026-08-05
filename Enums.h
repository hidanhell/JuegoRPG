#pragma once

#include <string>
#include <optional>

enum class Clase {
    Ninguno,
    Guerrero,
    Mago,
    Cazador
};

enum class Rareza {
    Ninguno,
    Comun,
    Raro,
    Epico,
    Legendaria,
    Elite
};

inline std::string claseToString(Clase c) {
    switch (c) {
        case Clase::Guerrero: return "Guerrero";
        case Clase::Mago: return "Mago";
        case Clase::Cazador: return "Cazador";
        default: return "Ninguno";
    }
}

inline std::string rarezaToString(Rareza r) {
    switch (r) {
        case Rareza::Comun: return "Comun";
        case Rareza::Raro: return "Raro";
        case Rareza::Epico: return "Epico";
        case Rareza::Legendaria: return "Legendaria";
        case Rareza::Elite: return "Elite";
        default: return "Ninguno";
    }
}

inline std::optional<Clase> claseFromString(const std::string& s) {
    if (s == "Guerrero") return Clase::Guerrero;
    if (s == "Mago") return Clase::Mago;
    if (s == "Cazador") return Clase::Cazador;
    return std::nullopt;
}

inline std::optional<Rareza> rarezaFromString(const std::string& s) {
    if (s == "Comun") return Rareza::Comun;
    if (s == "Raro") return Rareza::Raro;
    if (s == "Epico") return Rareza::Epico;
    if (s == "Legendaria") return Rareza::Legendaria;
    if (s == "Elite") return Rareza::Elite;
    return std::nullopt;
}
