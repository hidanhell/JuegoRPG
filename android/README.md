# La Aldea en las Sombras — Port Android (NDK)

Port del RPG de consola C++17 a Android con UI de texto nativa. El motor
C++ **no se duplica**: los `.cpp` de la raíz del repo se compilan in-situ
como `libjuegorphg.so` vía CMake + NDK.

## Arquitectura

```
 Kotlin UI (GameView)  <--polling texto / push input-->  GameEngine (JNI)
                                                              |
                                                     libjuegorphg.so
                                                              |
                     redirección POSIX de stdin/stdout (pipes)
                                                              |
                            Motor C++ (iniciarJuego) en hilo propio
```

- **Salida**: `stdout`/`stderr` del motor se redirigen a un pipe; un hilo
  lector los vuelca a un buffer que la UI consulta por polling
  (`nativeObtenerOutput`). Captura tanto `cout` directo como las llamadas
  ya migradas a `IO::render()`.
- **Entrada**: la UI encola líneas (`nativeEnviarInput`); un hilo feeder
  las escribe en el pipe de `stdin`, alimentando tanto `cin` directo como
  `IO::entrada()`. Una sola fuente de verdad, sin doble consumo.
- `AndroidRenderizador` / `AndroidEntrada` se registran en el service
  locator `IO::`, así la futura migración de `cout`/`cin` a `IO::` no
  requiere tocar el bridge.

Detalle completo en `app/src/main/cpp/jni_bridge.cpp`.

## Requisitos

- Android Studio (Hedgehog+), AGP 8.6.x
- NDK r25c (`25.2.9519653`) y CMake 3.22.1 (SDK Manager)
- Gradle wrapper: generar con `gradle wrapper` (los binarios no se comitean)
- **Gradle JDK 17**: en Android Studio recientes el JDK embebido por defecto
  puede ser una versión nueva (p. ej. 21/25) que Gradle aún no soporta para
  ejecutarse. Si al sincronizar ves `Incompatible Gradle JVM version`, ve a
  `Settings → Build, Execution, Deployment → Build Tools → Gradle` y cambia
  **Gradle JDK** a una 17 (p. ej. `jbr-17`). Esto solo afecta el JVM que
  ejecuta Gradle, no el de compilación de la app.

## Primer build

```bash
cd android
cp local.properties.example local.properties   # ajustar sdk.dir
gradle wrapper                                  # genera gradlew (una vez)
./gradlew :app:assembleDebug
```

O abrir la carpeta `android/` directamente en Android Studio.

## ABIs

`arm64-v8a` (dispositivos reales) y `x86_64` (emulador). Añadir
`armeabi-v7a` en `app/build.gradle` para gama baja.
