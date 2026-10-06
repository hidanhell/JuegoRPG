---
name: project-juegorpg
description: Qué es JuegoRPG (La Aldea en las Sombras) — RPG de texto C++17 de consola, con capa SOLID y port Android en curso
metadata:
  type: project
---

**"La Aldea en las Sombras"** — RPG por turnos de texto/consola, C++17, ~3.700 líneas, 21 archivos `.cpp` en la raíz del repo. Sin dependencias externas (stdlib + iostream). Versión 1.32. Autor original: hidanhell.

Capa SOLID ya mergeada en `main`: interfaces `IRenderizador`, `IEntrada`, `IAleatorio`; service locators `IO::` (IO.h/IO.cpp) y `Rng::` (Rng.h/Rng.cpp). Las funciones libres de `Utilidades.cpp` (`mostrarCabecera`, `limpiarPantalla`, `esperarTecla`, `mostrarBarraHP`, `limpiarBuffer`) YA enrutan por `IO::`. **Pero** el grueso del gameplay (Motor.cpp, Combate.cpp, Tienda.cpp, Loot.cpp, Personajes.cpp) todavía usa `cout`/`cin` directos — migrarlos a `IO::` es trabajo pendiente.

**Why:** proyecto personal de aprendizaje; el objetivo actual es llevarlo a Android manteniendo un único codebase compartido consola+móvil.

**How to apply:** al tocar I/O, prefiere `IO::render()`/`IO::entrada()` sobre cout/cin. El único `main()` está en Motor.cpp. Ver [[android-port-arquitectura]] para el port NDK.

Aviso de portabilidad: muchos `#include` usan minúsculas que no coinciden con el nombre real del archivo (p.ej. `"combate.h"` vs `Combate.h`). Compila en macOS/Windows (FS case-insensitive) pero rompería en un host case-sensitive.
