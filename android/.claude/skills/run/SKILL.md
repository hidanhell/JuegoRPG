---
name: run
description: Compila, instala y lanza "La Aldea en las Sombras" (port Android/NDK) en el emulador/dispositivo conectado, y revisa logcat en busca de crashes nativos (SIGSEGV, FATAL EXCEPTION) del motor C++.
---

# Run: La Aldea en las Sombras (Android)

Automatiza el ciclo build -> install -> lanzar -> verificar crash que se usa
para probar cambios en el motor C++ (`libjuegorphg.so`) o la UI Kotlin.

Paquete: `com.hidanhell.juegorphg` · Activity: `.MainActivity`

## 0. Prerequisito: dispositivo conectado

```bash
adb devices
```

Si no aparece ningún `device` (emulador o físico), avisa al usuario y detente
— no tiene sentido compilar sin nada donde instalar. No arranques un
emulador tú mismo salvo que el usuario lo pida explícitamente.

## 1. Build + install

Por defecto usa build incremental (rápido). Solo hace `clean` primero si:
- el usuario lo pide explícitamente ("compilación limpia", "clean build"), o
- los args de la skill incluyen "clean"/"limpio", o
- se tocó `CMakeLists.txt`, `build.gradle`, `gradle.properties` o el
  `ANDROID_STL`/flags del linker en esta sesión (un cambio de config nativa
  puede no invalidar la cache incremental de CMake correctamente).

```bash
cd android   # si el cwd no es ya android/

# build incremental (caso normal):
./gradlew installDebug

# o, si aplica alguna condicion de arriba:
./gradlew clean installDebug
```

Si el build falla, reporta el error tal cual (no lo reintentes a ciegas ni
apliques fixes sin que el usuario lo pida — esta skill es para *correr* la
app, no para depurar el build).

## 2. Lanzar con logcat limpio

```bash
adb logcat -c
adb shell am force-stop com.hidanhell.juegorphg
adb shell am start -n com.hidanhell.juegorphg/.MainActivity
sleep 4
```

## 3. Revisar crash nativo

El motor corre en un hilo C++ vía JNI (ver `app/src/main/cpp/jni_bridge.cpp`);
un crash ahí no siempre produce un stack trace Java normal. Busca las señales
reales:

```bash
adb logcat -d -v brief | grep -iE "FATAL|SIGSEGV|SIGABRT|AndroidRuntime.*Exception|backtrace|juegorphg.*crash"
```

- **Si hay match**: hubo un crash nativo. Reporta el stack trace completo
  (incluye offsets y nombres de función si `logcat` los resolvió) y NO lo
  arregles sin que el usuario te lo pida — solo repórtalo con claridad, como
  harías con cualquier resultado de "correr la app".
- **Si no hay match**: revisa que el motor efectivamente arrancó, buscando
  las líneas del bridge JNI:

```bash
adb logcat -d -v brief | grep "JuegoRPG-JNI"
```

Deberías ver `nativeStartGame: hilos lanzados` y
`Motor: iniciando iniciarJuego()`. Si no aparecen, la app pudo quedarse
colgada antes de arrancar el motor (revisa el resto del logcat).

## 4. Confirmación visual (opcional pero recomendado)

Una captura de pantalla confirma que la UI realmente renderiza texto del
motor (y no una pantalla en blanco/negra, que indicaría que el motor no
está mandando output aunque no haya crasheado):

```bash
adb exec-out screencap -p > /tmp/juegorpg_screen.png
```

Lee el PNG resultante con la herramienta Read para verlo.

## 5. Reporte al usuario

Resume en 2-4 líneas: si compiló, si instaló, si crasheó (con la causa si es
identificable) o si se ve el juego corriendo normalmente. No es necesario
listar cada comando ejecutado.
