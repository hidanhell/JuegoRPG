# =========================================================
# CMakeLists.txt  -  libjuegorphg.so
# Compila el motor C++ de "La Aldea en las Sombras" para Android.
#
# El codigo fuente del motor vive en la RAIZ del repo (5 niveles
# por encima de este archivo). No se copia ni se duplica: se
# referencia in-situ para que el juego siga siendo un unico
# codebase compartido entre la build de consola y la de Android.
# =========================================================
cmake_minimum_required(VERSION 3.22.1)
project(juegorphg VERSION 1.32 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Raiz del repo:  android/app/src/main/cpp -> ../../../../..
get_filename_component(GAME_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../../.." ABSOLUTE)
message(STATUS "JuegoRPG: GAME_ROOT = ${GAME_ROOT}")

# -------------------------------------------------------------------
# Fuentes del motor. Se recogen con GLOB porque el juego anade .cpp
# con frecuencia y son el unico contenido .cpp de la raiz. Si se
# prefiere control explicito, sustituir por una lista manual.
# CONFIGURE_DEPENDS hace que Gradle reconfigure al anadir/quitar .cpp.
# -------------------------------------------------------------------
file(GLOB ENGINE_SOURCES CONFIGURE_DEPENDS "${GAME_ROOT}/*.cpp")

# test_save.cpp trae su propio main() de test (usado por el build de
# consola vía run_task, no por el juego) y no debe entrar en el .so de
# la app: peso muerto en release y una segunda definicion de entry point.
list(FILTER ENGINE_SOURCES EXCLUDE REGEX "test_save\\.cpp$")

add_library(juegorphg SHARED
    ${ENGINE_SOURCES}
    jni_bridge.cpp
)

# El motor debe encontrar sus headers en la raiz del repo.
target_include_directories(juegorphg PRIVATE ${GAME_ROOT})

# ANDROID_BUILD:
#   - Convierte main() de Motor.cpp en void iniciarJuego().
#   - Punto de extension para que otros .cpp detecten la build movil.
target_compile_definitions(juegorphg PRIVATE ANDROID_BUILD=1)

target_compile_options(juegorphg PRIVATE
    -Wall
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Debug>:-g>
)

# Librerias del sistema Android: log (logcat) para el bridge JNI.
find_library(log-lib log)
target_link_libraries(juegorphg ${log-lib})

# Alineacion de segmentos ELF a 16 KB, obligatoria desde nov-2025 para apps
# nuevas/actualizadas en Google Play (dispositivos con paginas de 16 KB).
# El NDK r25c usado aqui no lo hace por defecto (eso llego en NDK r27+).
target_link_options(juegorphg PRIVATE "-Wl,-z,max-page-size=16384")
