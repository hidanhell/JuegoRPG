#include "Consumibles.h"

// =========================================================
// CATALOGO DE CONSUMIBLES
// =========================================================
// NOTA: los campos nuevos (curaEstados, recursoRestaurado, danoDirecto,
// efectoAlUsar, huidaGarantizada) tienen valor por defecto (false/0), asi
// que los items viejos que no los usan simplemente no los escriben en
// la lista y siguen funcionando exactamente igual que antes.
//
// efectoAlUsar usa los mismos IDs que Efectos.h / Efectos_Data.cpp:
//   1 Quemadura | 2 Veneno | 3 Sangrado | 4 Paralisis | 6 Congelacion | 7 Desorientacion Arcana
// =========================================================
std::vector<Consumible> listaConsumibles = {
    // --- POCIONES DE VIDA (Escaladas por Zona) ---
    // ID, Nombre, Desc, Cura, Atk, Def, Vel, Dur, Zona, Precio
    {1, "Pocion Baja", "Cura levemente", 50, 0, 0, 0, 0, 1, 20},
    {2, "Pocion Media", "Cura moderadamente", 150, 0, 0, 0, 0, 2, 60},
    {3, "Pocion Alta", "Cura gran parte de la vida", 400, 0, 0, 0, 0, 3, 150},
    {4, "Pocion de Dragon", "Restaura toda la salud", 2000, 0, 0, 0, 0, 4, 400},

    // --- ELIXIRES DE ESTADISTICAS (Buffs temporales) ---
    // Zona 1: version base de cada elixir de stat.
    {10, "Elixir de Furia", "+15 Ataque (3 turnos)", 0, 15, 0, 0, 3, 1, 100},
    {11, "Elixir de Caparazon", "+15 Defensa (3 turnos)", 0, 0, 15, 0, 3, 1, 100},
    {12, "Elixir de Viento", "+15 Velocidad (3 turnos)", 0, 0, 0, 15, 3, 1, 100},
    {13, "Destilado de Heroe", "Buff completo (5 turnos)", 100, 25, 25, 25, 5, 4, 500},

    // =====================================================
    // NUEVO: versiones escaladas de los elixires de stat para las
    // zonas 2 y 3. Antes Furia/Caparazon/Viento (zona 1, +15) eran
    // los UNICOS elixires de stat en todo el juego hasta llegar al
    // Destilado de zona 4 -- es decir, el mismo +15 seguia cayendo
    // en zona 3 contra enemigos de nivel 11-15, sintiendose cada vez
    // mas debil. Se agregan aqui versiones "Mayor" (zona 2, +22) y
    // "Superior" (zona 3, +30), con la misma proporcion de escalado
    // que ya usas en armas y artefactos por zona.
    // =====================================================
    {14, "Elixir de Furia Mayor",        "+22 Ataque (3 turnos)",    0, 22, 0,  0,  3, 2, 180},
    {15, "Elixir de Caparazon Mayor",    "+22 Defensa (3 turnos)",   0, 0,  22, 0,  3, 2, 180},
    {16, "Elixir de Viento Mayor",       "+22 Velocidad (3 turnos)", 0, 0,  0,  22, 3, 2, 180},
    {17, "Elixir de Furia Superior",     "+30 Ataque (3 turnos)",    0, 30, 0,  0,  3, 3, 300},
    {18, "Elixir de Caparazon Superior", "+30 Defensa (3 turnos)",   0, 0,  30, 0,  3, 3, 300},
    {19, "Elixir de Viento Superior",    "+30 Velocidad (3 turnos)", 0, 0,  0,  30, 3, 3, 300},

    // =====================================================
    // NUEVO: Antidoto. El juego ya tiene 6 efectos negativos
    // (Quemadura, Veneno, Sangrado, Paralisis, Congelacion,
    // Desorientacion Arcana) y ningun item los contrarrestaba -- la
    // unica defensa era aguantar el dano o esperar a que expiraran
    // solos. curaEstados=true limpia p.efectos por completo al usarlo.
    // =====================================================
    {20, "Antidoto", "Elimina todos tus efectos de estado activos", 0, 0, 0, 0, 0, 1, 70,
     /*curaEstados*/ true},

    // =====================================================
    // NUEVO: Tonico de Voluntad. Restaura recurso de clase (Ira,
    // Mana o Enfoque segun corresponda) de forma instantanea. Es
    // especialmente util para el Guerrero, que solo genera Ira
    // peleando y no tenia ninguna forma de emergencia de conseguirla.
    // =====================================================
    {21, "Tonico de Voluntad",       "Restaura 30 de tu recurso de clase", 0, 0, 0, 0, 0, 2, 140,
     /*curaEstados*/ false, /*recursoRestaurado*/ 30},
    {22, "Tonico de Voluntad Mayor", "Restaura 55 de tu recurso de clase", 0, 0, 0, 0, 0, 3, 260,
     /*curaEstados*/ false, /*recursoRestaurado*/ 55},

    // =====================================================
    // NUEVO: Bombas de alquimista. Primer consumible OFENSIVO del
    // juego -- antes el menu de inventario en combate solo servia
    // para curarte o buffearte, nunca para atacar directamente.
    // danoDirecto se resta al hp del enemigo; efectoAlUsar aplica un
    // estado de forma garantizada (no con probabilidad como el proc
    // de arma), ya que es un objeto que el jugador elige gastar.
    // =====================================================
    {23, "Bomba Incendiaria", "35 de dano directo + Quemadura garantizada", 0, 0, 0, 0, 0, 2, 120,
     /*curaEstados*/ false, /*recursoRestaurado*/ 0, /*danoDirecto*/ 35, /*efectoAlUsar*/ 1},
    {24, "Bomba Toxica", "30 de dano directo + Veneno garantizado", 0, 0, 0, 0, 0, 2, 120,
     /*curaEstados*/ false, /*recursoRestaurado*/ 0, /*danoDirecto*/ 30, /*efectoAlUsar*/ 2},

    // =====================================================
    // NUEVO: Amuleto de Huida. Item de emergencia -- usarlo garantiza
    // escapar del combate al instante, sin tirar la probabilidad
    // normal de la opcion "Huir". Pensado para cuando la RNG de huida
    // falla varias veces seguidas contra un enemigo demasiado fuerte.
    // =====================================================
    {25, "Amuleto de Huida", "Garantiza escapar del combate al instante", 0, 0, 0, 0, 0, 1, 160,
     /*curaEstados*/ false, /*recursoRestaurado*/ 0, /*danoDirecto*/ 0, /*efectoAlUsar*/ 0, /*huidaGarantizada*/ true}
};