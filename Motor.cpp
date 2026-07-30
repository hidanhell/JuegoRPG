// =========================================================
// BLOQUE M: MOTOR PRINCIPAL DEL JUEGO (v1.4)
// =========================================================
#include "utilidades.h"
#include "IO.h"
#include "SaveGame.h"
#include "personajes.h"
#include "combate.h"       
#include "loot.h"
#include "tienda.h"
#include "Rng.h"
#include <iostream>
#include <map>
#include <cctype>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include "catalogoObjetos.h"
#include "Armas.h"
#include "Artefactos.h" 
#include "Reliquias.h"
#include "Efectos.h"
#include "Monstruos.h"
#include "Habilidades.h"
#include "Recursos.h"
#include "Zonas.h"         
using namespace std;

int main() {
    string nombreUser;
    int tipoClase;

    // --- Inicio del juego con Lore ---
    mostrarCabecera("EL REINO DE LAS SOMBRAS v1.31");

    cout << "Tu cuerpo yace en medio de la aldea, derrotado tras una gran batalla." << endl;
    cout << "No recuerdas nada... ni como llegaste aqui." << endl;
    cout << "Cierras los ojos y ves a un caballero dragon luchando con furia." << endl;
    cout << "Abres los ojos: una figura negra se acerca lentamente entre las ruinas." << endl;
    cout << "Cierras los ojos de nuevo: un dragon lanza un ataque de fuego brutal." << endl;
    cout << "El tiempo se desvanece... no sabes cuanto permaneces inconsciente." << endl;
    cout << "\nFinalmente despiertas en una humilde casa." << endl;
    cout << "Desde la cama ves una mesa con dos sillas y una chimenea apagada," << endl;
    cout << "aun con brasas rojas consumiendose" << endl;
    cout << "Junto a la cama descansa un cofre polvoriento, y frente a ti la puerta." << endl;

    cout << "\n Quieres abrir el cofre antes de salir? (S/N): ";
    char decision; cin >> decision;

    // Primero pedimos nombre y clase para poder asignar el arma correcta
    limpiarPantalla();
    cout << "Al salir de la casa, un viento frio golpea tu rostro." << endl;
    cout << "Tu mente comienza a aclararse... recuerdas tu nombre y tu senda." << endl;
    cout << "Dime tu nombre: "; 
    limpiarBuffer(); getline(cin, nombreUser);
    cout << "Elige tu senda (1. Guerrero, 2. Mago, 3. Cazador): "; cin >> tipoClase;

    Personaje p(nombreUser, tipoClase);

    // --- Cofre inicial: arma según clase ---
    if (aMinuscula(decision) == 's') {
        Arma inicial(0, "-", 0, "Comun", 0, "Ninguno", 0, 0);

        if (p.clase == "Guerrero") {
            inicial = Arma(1, "Espada Oxidada", 12, "Comun", 0, "Guerrero", 1, 0);
            cout << "\nEntre el polvo encuentras una Espada Oxidada y una Pocion Baja." << endl;
        } else if (p.clase == "Mago") {
            inicial = Arma(7, "Varita Astillada", 12, "Comun", 0, "Mago", 1, 0);
            cout << "\nEntre el polvo encuentras una Varita Astillada y una Pocion Baja." << endl;
        } else { // Cazador
            inicial = Arma(13, "Arco de Caza", 12, "Comun", 0, "Cazador", 1, 0);
            cout << "\nEntre el polvo encuentras un Arco de Caza y una Pocion Baja." << endl;
        }

        p.armaEquipada = inicial;
        p.inventario.push_back(listaConsumibles[0]);
        esperarTecla();
    } else {
        cout << "\nDecides salir sin abrir el cofre. El mundo sera mas hostil sin ayuda inicial." << endl;
        esperarTecla();
    }

    while (true) {
        limpiarPantalla();
        
        // NUEVO: Evaluación unificada de la casilla actual a través del Gestor de Zonas
        InfoCasilla casillaActual = GestorZonas::evaluarCasilla(p.posX, p.posY);
        
        cout << "ESTAS EN: " << casillaActual.nombreZona << endl;
        cout << "POSICION: [X: " << p.posX << " | Y: " << p.posY << "]" << endl;

        // --- HUD ---
        cout << "=============== " << p.nombre << " (" << p.clase;
        if (p.tieneSubclase15) {
            int idSubclase = p.habilidadesIds.back();
            if (auto h = obtenerHabilidadPorId(idSubclase)) cout << " ===> " << h->nombre;
        }
        cout << ") ===============\n";

        mostrarBarraHP(p.hp, p.hpMax);

        cout << "ATQ: " << p.ataqueBase 
             << " | DEF: " << p.defensaBase 
             << " | VEL: " << p.velocidadBase 
             << " | CRIT: " << p.bonusCritico << "%\n";

        cout << "ORO: " << p.oro 
             << " | NIVEL: " << p.nivel 
             << " | EXP: " << p.exp << "/" << (p.nivel * 50) << "\n";

        cout << "----------------------------------------------------" << endl;

        // --- Control de muerte ---
        if (p.hp <= 0) {
            p.reaparecer(); 
            continue; 
        }

        // Detección de Tienda usando el gestor de zonas
        if (casillaActual.esTienda) { 
            cout << "[-] TIENDA CERCANA: Presiona 'T' para comerciar." << endl;
        }

        if (p.posY >= 230) cout << "[ALERTA] El aire hierve... Lancelot esta muy cerca." << endl;

        cout << "CONTROLES: (W-A-S-D) Mover | (P) Status | (G) Guardar/Cargar | (Q) Salir" << endl;
        
        char input; cin >> input; input = aMinuscula(input);

        if (input == 'w') {
            if (p.posY < 241) {
                p.posY++;

                // NUEVO: Evaluamos qué hay en la nueva casilla
                InfoCasilla nuevaCasilla = GestorZonas::evaluarCasilla(p.posX, p.posY);

                // Si hay un jefe forzado (Valdrame en Y=60), se salta el azar y pelea
                if (!p.valdrameDerrotado && nuevaCasilla.esJefeObligatorio) {
                    limpiarPantalla();
                    mostrarCabecera(nuevaCasilla.nombreZona);
                    iniciarCombate(p, p.posY);
                } else {
                    // Flujo normal si no hay jefe forzado
                    // Diálogo ambiental al moverse (15% de probabilidad)
                    lanzarDialogoAmbiental(p.posY);

                    // 30% de probabilidad de combate aleatorio
                    if (p.posY < 241 && Rng::get().probabilidad(30)) {
                        iniciarCombate(p, p.posY);
                    }
                }
            }
        }
        else if (input == 's') { if (p.posY > 0) p.posY--; }
        else if (input == 'a') { p.posX--; }
        else if (input == 'd') { p.posX++; }
        else if (input == 't') {
            if (casillaActual.esTienda) {
                entrarTienda(p);
            }
        }
        else if (input == 'p') {
            limpiarPantalla();

            mostrarCabecera("STATUS DE " + p.nombre);

            // Clase y subclase
            cout << "Clase: " << p.clase;
            if (p.tieneSubclase15) {
                int idSubclase = p.habilidadesIds.back();
                if (auto h = obtenerHabilidadPorId(idSubclase)) cout << " → " << h->nombre;
            }
            cout << "\nNivel: " << p.nivel 
                 << " | EXP: " << p.exp << "/" << (p.nivel * 50) 
                 << " | Oro: " << p.oro << "\n";

            // Atributos base
            cout << "\n=============== Atributos Base ===============\n";
            cout << "Fuerza: " << p.fuerza 
                 << " | Destreza: " << p.destreza 
                 << " | Inteligencia: " << p.inteligencia 
                 << " | Vitalidad: " << p.vitalidad << "\n";

            // Estadísticas de combate
            cout << "\n=============== Estadisticas de Combate ===============\n";
            mostrarBarraHP(p.hp, p.hpMax);
            cout << "Ataque: " << p.ataqueBase 
                 << " | Defensa: " << p.defensaBase 
                 << " | Velocidad: " << p.velocidadBase 
                 << " | Critico: " << p.bonusCritico << "%\n";

            // Equipamiento
            cout << "\n=============== Equipamiento ===============\n";
            cout << "Arma: " << p.armaEquipada.nombre 
                 << " (+" << p.armaEquipada.poder << " ATQ)";
            if (auto ef = obtenerEfectoPorId(p.armaEquipada.efectoId))
                cout << " [Efecto: " << ef->nombre << "]";
            else
                cout << " [Sin efecto]";

            cout << "\nArtefacto: " << p.artefactoEquipado.nombre
                 << " (+" << p.artefactoEquipado.defensa << " DEF)";
            if (auto ef = obtenerEfectoPorId(p.artefactoEquipado.efectoId))
                cout << " [Efecto: " << ef->nombre << "]";
            else
                cout << " [Sin efecto]";

            // Inventario abreviado
            cout << "\n\n=============== Inventario (max 10) ===============\n";
            if (p.inventario.empty()) cout << "Vacio\n";
            else {
                map<string,string> abrevs = {
                    {"Pocion Baja","PB"}, {"Pocion Media","PM"}, {"Pocion Alta","PA"},
                    {"Elixir de Caparazon","EC"}, {"Elixir de Furia","EF"}, {"Elixir de Vida","EV"}
                };
                map<string,int> conteo;
                for (auto &it : p.inventario) {
                    if (abrevs.count(it.nombre)) conteo[abrevs[it.nombre]]++;
                    else conteo[it.nombre]++;
                }
                bool primero = true;
                for (auto &kv : conteo) {
                    if (!primero) cout << " | ";
                    cout << kv.first << " x" << kv.second;
                    primero = false;
                }
                cout << "\n";
            }

            // Reliquias
            cout << "\n=============== Reliquias ===============\n";
            if (p.reliquias.empty()) cout << "Ninguna\n";
            else for (string r : p.reliquias) cout << "- " << r << "\n";

            // Efectos de estado
            cout << "\n=============== Efectos de Estado ===============\n";
            if (p.efectos.empty()) cout << "Ninguno\n";
            else for (auto &e : p.efectos) cout << e.nombre << " (" << e.duracion << " turnos)\n";

            cout << "\nPresiona ENTER para volver...";
            limpiarBuffer(); cin.get();
        }
        else if (input == 'g') {
            limpiarPantalla();
            mostrarCabecera("GUARDADO / CARGA");
            cout << "1. Guardar partida" << endl;
            cout << "2. Cargar partida" << endl;
            cout << "3. Nueva partida" << endl;
            cout << "4. Volver" << endl;
            cout << "Seleccion: ";
            int opc; if (!(cin >> opc)) { limpiarBuffer(); continue; }
            limpiarBuffer();
            if (opc == 1) {
                cout << "Nombre de archivo (sin extension): ";
                string nombreArchivo = IO::entrada().leerLinea();
                if (nombreArchivo.empty()) nombreArchivo = "slot1";
                string ruta = string("savegames/") + nombreArchivo + ".json";
                if (guardarPartida(p, p.posY, ruta)) cout << "[OK] Partida guardada en: " << ruta << endl;
                else cout << "[ERROR] No se pudo guardar la partida." << endl;
                esperarTecla();
            } else if (opc == 2) {
                namespace fs = std::filesystem;
                string savesDir = "savegames";
                struct SaveInfo { string name; string mtime; uintmax_t size; };
                vector<SaveInfo> saves;
                if (fs::exists(savesDir) && fs::is_directory(savesDir)) {
                    for (auto &entry : fs::directory_iterator(savesDir)) {
                        if (entry.is_regular_file()) {
                            auto pth = entry.path();
                            if (pth.extension() == ".json") {
                                string name = pth.stem().string();
                                // last write time -> human readable
                                string mtimeStr = "?";
                                try {
                                    auto ftime = fs::last_write_time(pth);
                                    auto sctp = std::chrono::system_clock::now() + (ftime - fs::file_time_type::clock::now());
                                    std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
                                    std::tm tm = *std::localtime(&cftime);
                                    std::ostringstream oss;
                                    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
                                    mtimeStr = oss.str();
                                } catch (...) {}

                                uintmax_t fsize = 0;
                                try { fsize = fs::file_size(pth); } catch (...) { fsize = 0; }

                                saves.push_back({name, mtimeStr, fsize});
                            }
                        }
                    }
                }

                if (saves.empty()) {
                    cout << "[INFO] No hay partidas guardadas." << endl;
                    esperarTecla();
                } else {
                    cout << "Partidas disponibles:\n";
                    for (size_t i = 0; i < saves.size(); ++i) {
                        auto &s = saves[i];
                        cout << (i + 1) << ". " << s.name << " (";
                        if (s.size >= 1024) cout << (s.size / 1024) << " KB";
                        else cout << s.size << " B";
                        cout << ", " << s.mtime << ")\n";
                    }
                    cout << "Ingresa numero para cargar, o nombre (sin extension). ENTER para cancelar: ";
                    string choice = IO::entrada().leerLinea();
                    if (!choice.empty()) {
                        string nombreArchivo;
                        bool onlyDigits = true;
                        for (char c : choice) if (!isdigit((unsigned char)c)) { onlyDigits = false; break; }
                        if (onlyDigits) {
                            int idx = stoi(choice);
                            if (idx >= 1 && static_cast<size_t>(idx) <= saves.size()) {
                                nombreArchivo = saves[static_cast<size_t>(idx - 1)].name;
                            } else {
                                cout << "[ERROR] Indice invalido." << endl;
                                esperarTecla();
                                nombreArchivo.clear();
                            }
                        } else {
                            nombreArchivo = choice;
                        }

                        if (!nombreArchivo.empty()) {
                            string ruta = savesDir + "/" + nombreArchivo + ".json";
                            if (cargarPartida(p, p.posY, ruta)) cout << "[OK] Partida cargada: " << ruta << endl;
                            else cout << "[ERROR] No se pudo cargar la partida." << endl;
                            esperarTecla();
                        }
                    }
                }
            } else if (opc == 3) {
                cout << "Crear nueva partida y perder progreso actual? (S/N): ";
                char r; cin >> r; r = aMinuscula(r);
                if (r == 's') {
                    nuevaPartida(p, 0);
                    cout << "[OK] Nueva partida creada." << endl;
                } else cout << "Operacion cancelada." << endl;
                esperarTecla();
            } else {
                // volver
            }
        }
        else if (input == 'q') {
            cout << "¿Estas seguro de que quieres abandonar la mision? (S/N): ";
            char conf; cin >> conf; 
            if (tolower(conf) == 's') break;
        }

        // --- Límites del mapa ---
        if (p.posX > 10) p.posX = 10;
        if (p.posX < -10) p.posX = -10;

        // --- Encuentro final con Lancelot ---
        if (p.posY == 241) {
            limpiarPantalla();
            mostrarCabecera("EL TRONO DEL REY DRAGON");
            cout << "Lancelot te espera sentado en un trono de huesos." << endl;
            cout << "Antes de avanzar, ves un Cofre Divino." << endl;
            cout << "--Abrirlo-- (S/N): ";
            char cL; cin >> cL;
            if (tolower(cL) == 's') {
                Arma legend(0, "-", 0, "Comun", 0, "Ninguno", 0, 0);
                if (p.clase == "Guerrero") legend = excalibur;
                else if (p.clase == "Mago") legend = bastonDragon;
                else legend = arcoAlma;

                p.armaEquipada = legend;
                for (int i = 0; i < 3; i++) p.inventario.push_back(listaConsumibles[2]); 
                cout << "\n--HAS OBTENIDO " << legend.nombre << " Y 3 POCIONES ALTAS--" << endl;
                esperarTecla();
            }

            cout << "\n--Es el momento de la venganza-- (S/N): ";
            char pelear; cin >> pelear;
            if (tolower(pelear) == 's') {
                iniciarCombate(p, 241);

                if (p.hp > 0 && p.posY == 241) {
                    limpiarPantalla();
                    cout << "--Lancelot ha caido! El cielo se aclara por primera vez en decadas." << endl;
                    cout << "Has completado tu destino, " << p.nombre << "." << endl;
                    esperarTecla();
                    break;
                }
            } else {
                p.posY = 239;
            }
        }
    }
    return 0;
}