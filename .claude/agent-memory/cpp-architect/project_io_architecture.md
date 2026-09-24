---
name: project-io-architecture
description: How console I/O is structured (and entangled) in the JuegoRPG codebase — the central refactoring/porting obstacle
metadata:
  type: project
---

I/O is fully synchronous, blocking, and inlined throughout the codebase — there is no presentation abstraction.

**Fact:** `std::cout` / `std::cin` calls are embedded directly inside business logic everywhere: `Motor.cpp::main()` (HUD render + WASD `cin >> input`), `Combate.cpp::iniciarCombate()` (combat HUD, menus, `cin >> opc`), `Personajes.cpp::subirNivel()` (level-up menus prompt the user with `cin >> eleccion` mid-calculation), `Tienda.cpp::entrarTienda()`, `Loot.cpp::gestionarLoot()`. Cross-platform helpers live in `Utilidades.cpp`: `limpiarPantalla()` calls `system("cls"/"clear")`, `esperarTecla()` calls `system("pause")` on Windows / `cin.get()` elsewhere, plus `mostrarBarraHP`, `mostrarCabecera`, `limpiarBuffer`. These `system()` calls and raw `cin/cout` are the main blockers for Android port and for unit testing.

**Why:** Game grew organically (hobby project). No `IRenderizador` / input-port abstraction exists; logic and presentation were never separated.

**How to apply:** Any refactor toward testability or Android must first introduce an output/input port (e.g. `IRenderizador` / `IEntrada`) and route all `cout/cin/system()` through it. `subirNivel()` is the worst offender: it mixes stat math with interactive choice prompts, so it cannot run headless. See [[project-juego-rpg]].
