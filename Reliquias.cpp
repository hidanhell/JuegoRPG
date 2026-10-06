#include "Reliquias.h"
#include "Enums.h"
#include "Personajes.h"
#include <iostream>
#include <algorithm>

Reliquia::Reliquia(int i, std::string n, std::string e, std::string r)
    : id(i), nombre(n), efecto(e), rareza(rarezaFromString(r).value_or(Rareza::Comun)) {}

void Reliquia::mostrarInfo() const {
    std::cout << "ID: " << id
              << " | Reliquia: " << nombre
              << " | Efecto: " << efecto
              << " | Rareza: " << rarezaToString(rareza)
              << std::endl;
}

bool reliquiaCompatibleConClase(const Reliquia& reliquia, Clase clase) {
    if (reliquia.nombre == "Garra del Berserker" || reliquia.nombre == "Hombreras del Coloso") {
        return clase == Clase::Guerrero;
    }
    if (reliquia.nombre == "Ojo del Arcano" || reliquia.nombre == "Gema del Eco") {
        return clase == Clase::Mago;
    }
    if (reliquia.nombre == "Pluma del Viento" || reliquia.nombre == "Carcaj Inagotable") {
        return clase == Clase::Cazador;
    }
    return true;
}

bool aplicarBonosReliquia(Personaje& personaje, const Reliquia& reliquia) {
    if (reliquia.nombre == "Anillo del Asesino") {
        personaje.bonusCritico += 5;
    } else if (reliquia.nombre == "Medallon del Guardian") {
        personaje.defensaBase += 10;
    } else if (reliquia.nombre == "Garra del Berserker" && personaje.clase == Clase::Guerrero) {
        personaje.fuerza += 8;
        personaje.ataqueBase += 8;
    } else if (reliquia.nombre == "Ojo del Arcano" && personaje.clase == Clase::Mago) {
        personaje.inteligencia += 8;
        personaje.ataqueBase += 8;
    } else if (reliquia.nombre == "Pluma del Viento" && personaje.clase == Clase::Cazador) {
        personaje.destreza += 8;
        personaje.ataqueBase += 8;
    } else if (reliquia.nombre == "Botas del Relampago") {
        personaje.velocidadBase += 10;
    } else if (reliquia.nombre == "Anillo de la Vitalidad") {
        personaje.hpMax += 50;
        personaje.hp = std::min(personaje.hpMax, personaje.hp + 50);
    } else {
        return false;
    }
    return true;
}

void retirarBonosReliquia(Personaje& personaje, const Reliquia& reliquia) {
    if (reliquia.nombre == "Anillo del Asesino") {
        personaje.bonusCritico -= 5;
    } else if (reliquia.nombre == "Medallon del Guardian") {
        personaje.defensaBase -= 10;
    } else if (reliquia.nombre == "Garra del Berserker" && personaje.clase == Clase::Guerrero) {
        personaje.fuerza -= 8;
        personaje.ataqueBase -= 8;
    } else if (reliquia.nombre == "Ojo del Arcano" && personaje.clase == Clase::Mago) {
        personaje.inteligencia -= 8;
        personaje.ataqueBase -= 8;
    } else if (reliquia.nombre == "Pluma del Viento" && personaje.clase == Clase::Cazador) {
        personaje.destreza -= 8;
        personaje.ataqueBase -= 8;
    } else if (reliquia.nombre == "Botas del Relampago") {
        personaje.velocidadBase -= 10;
    } else if (reliquia.nombre == "Anillo de la Vitalidad") {
        personaje.hpMax = std::max(1, personaje.hpMax - 50);
        personaje.hp = std::min(personaje.hp, personaje.hpMax);
    }
}
