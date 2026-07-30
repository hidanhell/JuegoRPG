#ifndef ZONAS_H
#define ZONAS_H

#include <string>

// Estructura para definir qué hay en una coordenada específica del mapa
struct InfoCasilla {
    std::string nombreZona;    // Nombre descriptivo de la zona o lugar actual
    bool esTienda;             // Indica si hay un mercader disponible
    bool esJefeObligatorio;    // Indica si se activa un jefe de manera forzosa
    std::string idJefe;        // Identificador del jefe (ej. "ARZOBISPO VALDRAME")
};

namespace GestorZonas {
    // Determina la zona numérica (1 a 5) según la posición Y
    int obtenerZonaPorY(int y);

    // Devuelve el nombre de la zona general según la posición Y
    std::string obtenerNombreZona(int y);

    // Evalúa la casilla actual ignorando el eje X si Y llega a un punto clave (como el 60)
    InfoCasilla evaluarCasilla(int x, int y);
}

#endif