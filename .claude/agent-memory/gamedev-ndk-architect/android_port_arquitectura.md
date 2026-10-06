---
name: android-port-arquitectura
description: Decisiones de arquitectura del port Android (NDK) de JuegoRPG — transporte por pipes, UI de texto, threading JNI
metadata:
  type: project
---

Port Android en `android/` (rama `feature/android-port`, iniciada 2026-07-02). Proyecto AGP 8.1.4, Kotlin 1.9.22, NDK r25c (25.2.9519653), CMake 3.22.1, minSdk 24, ABIs arm64-v8a + x86_64. Paquete `com.hidanhell.juegorphg`. Librería nativa `libjuegorphg.so`.

**Decisión clave — transporte por redirección POSIX de pipes** (en `app/src/main/cpp/jni_bridge.cpp`): como el gameplay aún usa `cout`/`cin` directos (ver [[project-juegorpg]]), en vez de reescribir 3.700 líneas se redirigen `stdout`/`stderr` y `stdin` a pipes internos. Hilo lector vuelca stdout a un buffer (UI lo consulta por polling con `nativeObtenerOutput`); hilo feeder toma la cola de input (`nativeEnviarInput` + `condition_variable`) y la escribe en el pipe de stdin. Una sola fuente de verdad, sin doble consumo. `AndroidRenderizador`/`AndroidEntrada` se registran en `IO::` y también enrutan por cout/cin, así la futura migración a `IO::` no toca el bridge.

**Why:** hacer el juego jugable HOY sin bloquear el port en la migración cout→IO. **How to apply:** si en el futuro se migra el gameplay a `IO::`, NO hay que quitar la redirección de pipes (sigue siendo compatible); revisar sólo que no se dupliquen lecturas si algún camino lee por los dos canales a la vez.

Otras decisiones: `main()` de Motor.cpp envuelto en `#ifdef ANDROID_BUILD` → expone `void iniciarJuego()`. CMakeLists usa `file(GLOB ... CONFIGURE_DEPENDS)` sobre `${GAME_ROOT}/*.cpp` (raíz = 5 niveles arriba de cpp/). Sentinela `'\f'` (0x0C) = limpiar pantalla, interpretado en GameView.kt. UI = TextView monospace + ScrollView + botones 1-5 + EditText libre (nombre/WASD/SN). Sin SDL/OpenGL. Modelo JNI = polling + push (sin callbacks nativos→Java, sin AttachCurrentThread).

**Pendiente para compilar en Android Studio:** generar gradle wrapper (`gradle wrapper`), crear `local.properties`, instalar NDK/CMake vía SDK Manager. Falta iconos de launcher (no declarados en manifest, opcional).

**Riesgo de integración con main (rev. 2026-08-31, port hecho sobre `522ffe0`, main avanzó a `9953959`):** El nuevo `Motor.cpp` de main añade menú save/load que usa `std::filesystem` con rutas RELATIVAS: escribe en `savegames/<nombre>.json` y hace `fs::directory_iterator("savegames")`. En Android el CWD no es escribible → esas ramas fallan/lanzan al rebasear el `#ifdef ANDROID_BUILD` sobre el nuevo Motor. Solución futura: en build ANDROID_BUILD anclar la ruta a `Context.getFilesDir()` pasado por JNI. El wrap `#ifdef` en sí es sano; el rebase dará conflicto resoluble (main() creció +160 líneas pero los puntos de anclaje —inicio de main y `return 0`— persisten). Ni Zonas/Recursos/SaveGame/Combate introducen threads, `std::exit`/`abort` ni fstream nuevos (fstream sólo en IO.cpp), así que el modelo "un solo hilo motor" del bridge se mantiene. `test_save.cpp` (nuevo, con su propio `main()`) SÍ entra al `.so` vía el GLOB: no da símbolo duplicado (el `main` de Motor está renombrado a `iniciarJuego`), pero mete un `main` muerto + bloat de test en el binario de release. Fix: excluirlo del GLOB. Includes del Motor con case inconsistente (`personajes.h` vs `Personajes.h`) compilan sólo por FS case-insensitive del host macOS.
