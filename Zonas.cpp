#include "Zonas.h"

namespace GestorZonas {

    int obtenerZonaPorY(int y) {
        if (y <= 60) return 1;
        if (y <= 120) return 2;
        if (y <= 180) return 3;
        if (y <= 240) return 4;
        return 5; // Castillo / Trono final (Lancelot)
    }

    std::string obtenerNombreZona(int y) {
        if (y <= 60) return "Aldea en las Sombras";
        if (y <= 120) return "Bosque Oscuro";
        if (y <= 180) return "Pantano Siniestro";
        if (y <= 240) return "Tierra Desolada";
        return "Castillo Abandonado";
    }

    InfoCasilla evaluarCasilla(int /*x*/, int y) {
        InfoCasilla info;
        info.nombreZona = obtenerNombreZona(y);
        info.esTienda = false;
        info.esJefeObligatorio = false;
        info.idJefe = "";

        // 1. Tiendas fijas en coordenadas Y específicas
        if (y == 30 || y == 90 || y == 150 || y == 210) {
            info.esTienda = true;
        }

        // 2. Zona especial / Iglesia en el bloque Y = 60 (Jefe obligatorio: Valdrame)
        // Se activa sin importar la coordenada X del jugador.
        if (y == 60) {
            info.esJefeObligatorio = true;
            info.idJefe = "ARZOBISPO VALDRAME";
            info.nombreZona = "La Iglesia Abandonada";
        }

        return info;
    }
}