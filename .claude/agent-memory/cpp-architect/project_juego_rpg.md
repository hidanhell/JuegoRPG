---
name: project-juego-rpg
description: High-level facts about the "La Aldea en las Sombras" v1.31 C++17 console RPG project
metadata:
  type: project
---

"La Aldea en las Sombras" / "El Reino de las Sombras" v1.31 — turn-based text RPG in C++17.

**Fact:** Console-only game (stdin/stdout, no GUI, no SDL/SFML). Builds with g++ -std=c++17 across ~19 .cpp files (see .vscode/tasks.json for the canonical compile command). Output binaries: `Reino` (Mac), `Reino.exe`/`JuegoRPG.exe` (Windows). Entry point is `Motor.cpp::main()`.

**Why:** Hobby/learning RPG. User (Cesar) wants to port it to Android (working dir is under androidClaude/).

**How to apply:** No external dependencies — pure stdlib + iostream + <cstdlib> system(). No threads, no sleep, no conio/termios. Text contains UTF-8 (accents, ñ, →, ¿) in many data files — relevant for Android TextView encoding.

**UPDATE (2026-08-31, from origin/main commits 1f7b7fa/541873d/9953959):** Save-game/file I/O NOW EXISTS. `SaveGame.h` (new) + ~298 new lines in `IO.cpp` implement guardarPartida/cargarPartida via `<fstream>` (ofstream/ifstream) writing JSON-ish files to relative dir `savegames/*.json`. `Motor.cpp` gained a 'G' menu (guardar/cargar/nueva) using `<filesystem>` (directory_iterator, last_write_time), `<chrono>`, `<iomanip>` (put_time). Still SINGLE-THREADED, still blocks on cin, NO exit()/abort()/threads — JNI single-thread/never-exit model holds. Two caveats for Android: (1) `IO.cpp` calls system("cls"/"clear"/"pause >nul") — pre-existing since before the port, not new — should be neutralized on NDK; (2) relative "savegames/" path won't be writable from Android app cwd — save feature needs a path redirect to app filesDir. Version bumped v1.31 -> v1.4. Clase/Rareza are now `enum class` (Enums.h, new).

Module map: Motor (game loop/HUD/navigation), Combate (turn combat), Personajes (stats/inventory), Monstruos(_Data), Habilidades(Data/Monstruo), Efectos(_Data), Armas, Artefactos, Reliquias(Datos), Consumibles, CatalogoObjetos, Loot, Tienda, Utilidades (cross-platform helpers).
