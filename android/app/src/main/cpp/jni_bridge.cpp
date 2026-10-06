// =========================================================
// jni_bridge.cpp
// Puente JNI entre el motor C++ de "La Aldea en las Sombras"
// y la capa Kotlin/Android.
//
// ESTRATEGIA DE TRANSPORTE (leer antes de tocar nada):
//
//   El gameplay actual (Motor.cpp, Combate.cpp, Tienda.cpp, ...)
//   escribe con `cout` y lee con `cin` DIRECTAMENTE. La migracion
//   de esos cout/cin al service locator IO:: es trabajo futuro.
//
//   Para que el juego sea jugable HOY sin reescribir 3.700 lineas,
//   redirigimos los descriptores POSIX stdout/stderr y stdin a
//   pipes internos:
//
//     stdout(fd 1) + stderr(fd 2)  ->  pipe  ->  hilo lector  ->  g_output
//     g_inputQueue  ->  hilo feeder  ->  pipe  ->  stdin(fd 0)  ->  cin
//
//   Asi TODO el texto (cout directo Y las llamadas ya migradas a
//   IO::render()) termina en un unico buffer g_output, y TODO el
//   input (cin directo Y IO::entrada()) sale de una unica cola
//   g_inputQueue. Una sola fuente de verdad, sin duplicacion.
//
//   Ademas registramos AndroidRenderizador / AndroidEntrada via
//   IO::establecer*() para que, a medida que el gameplay migre a
//   IO::, esas llamadas usen el MISMO pipe (cout/cin internos) sin
//   requerir ningun cambio en este bridge.
//
// MODELO DE HILOS:
//   - Hilo del motor: corre iniciarJuego() (bucle principal). Bloquea
//     en cin cuando espera input; cin bloquea sobre el pipe vacio.
//   - Hilo lector de stdout: vuelca el pipe en g_output.
//   - Hilo feeder de stdin: espera (condition_variable) a que la UI
//     envie input y lo escribe en el pipe de stdin.
//   - La UI (Kotlin) NUNCA es llamada desde C++: usa polling
//     (nativeObtenerOutput) + push (nativeEnviarInput). No hace falta
//     AttachCurrentThread ni GlobalRef.
// =========================================================

#include <jni.h>
#include <android/log.h>

#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <deque>
#include <iostream>
#include <limits>
#include <mutex>
#include <string>
#include <thread>

#include <unistd.h>
#include <fcntl.h>

#include "IO.h"
#include "IRenderizador.h"
#include "IEntrada.h"

#define LOG_TAG "JuegoRPG-JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Declarada en Motor.cpp bajo -DANDROID_BUILD.
void iniciarJuego();

// =========================================================
// Estado compartido
// =========================================================
namespace {

// ---- Buffer de salida (texto que la UI aun no ha consumido) ----
std::mutex              g_outputMutex;
std::string             g_output;

// ---- Cola de entrada (lineas que la UI ha enviado) ----
std::mutex              g_inputMutex;
std::condition_variable g_inputCv;
std::deque<std::string> g_inputQueue;

// ---- Ciclo de vida ----
std::atomic<bool>       g_running{false};
std::thread             g_engineThread;
std::thread             g_stdoutReader;
std::thread             g_stdinFeeder;

// ---- Pipes de redireccion ----
int g_stdoutPipe[2] = {-1, -1};   // [0]=read, [1]=write (=stdout)
int g_stdinPipe[2]  = {-1, -1};   // [0]=read (=stdin), [1]=write
int g_savedStdout   = -1;
int g_savedStdin    = -1;

void appendOutput(const std::string& s) {
    std::lock_guard<std::mutex> lock(g_outputMutex);
    g_output += s;
}

// Espera bloqueante a la siguiente linea enviada por la UI.
// Devuelve false si el juego se esta cerrando.
bool popInputLine(std::string& out) {
    std::unique_lock<std::mutex> lock(g_inputMutex);
    g_inputCv.wait(lock, [] { return !g_inputQueue.empty() || !g_running.load(); });
    if (!g_running.load() && g_inputQueue.empty()) return false;
    out = g_inputQueue.front();
    g_inputQueue.pop_front();
    return true;
}

} // namespace

// =========================================================
// Implementaciones Android de las interfaces SOLID.
//
// Enrutan a traves de std::cout / std::cin, que estan
// redirigidos a los pipes. De este modo comparten la MISMA
// tuberia que el cout/cin directo del gameplay: una sola
// fuente de verdad, cero duplicacion de texto o de input.
//
// La unica diferencia real frente a la implementacion de
// consola es limpiar(): en vez de system("clear") emitimos
// el sentinela '\f' (form feed) que la UI interpreta como
// "vaciar la pantalla".
// =========================================================
class AndroidRenderizador : public IRenderizador {
public:
    void limpiar() override {
        // Sentinela de limpieza para la capa Kotlin (ver GameView).
        std::cout << '\f';
        std::cout.flush();
    }

    void escribir(const std::string& texto) override {
        std::cout << texto;
        std::cout.flush();
    }

    void mostrarBarraHP(int hp, int hpMax) override {
        const int LARGO = 20;
        int llenos = (hpMax > 0) ? (hp * LARGO / hpMax) : 0;
        if (llenos < 0) llenos = 0;
        if (llenos > LARGO) llenos = LARGO;

        std::string barra = "[";
        for (int i = 0; i < LARGO; ++i) barra += (i < llenos ? '#' : '.');
        barra += "] " + std::to_string(hp) + "/" + std::to_string(hpMax) + " HP\n";
        std::cout << barra;
        std::cout.flush();
    }

    void mostrarCabecera(const std::string& titulo) override {
        std::cout << "==== " << titulo << " ====\n";
        std::cout.flush();
    }

    void esperarTecla() override {
        std::cout << "Presiona ENTER para continuar...";
        std::cout.flush();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

class AndroidEntrada : public IEntrada {
public:
    char leerTecla() override {
        char c = 0;
        std::cin >> c;
        return c;
    }

    int leerOpcion() override {
        int n = 0;
        std::cin >> n;
        return n;
    }

    std::string leerLinea() override {
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    void limpiarBuffer() override {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
};

namespace {

AndroidRenderizador g_androidRender;
AndroidEntrada      g_androidEntrada;

// ---------------------------------------------------------
// Hilos de servicio
// ---------------------------------------------------------

// Lee del pipe de stdout y acumula en g_output.
void stdoutReaderLoop() {
    char buf[512];
    while (g_running.load()) {
        ssize_t n = read(g_stdoutPipe[0], buf, sizeof(buf));
        if (n > 0) {
            appendOutput(std::string(buf, static_cast<size_t>(n)));
        } else if (n == 0) {
            break; // pipe cerrado
        } else {
            if (errno == EINTR) continue;
            break;
        }
    }
}

// Espera input de la UI y lo escribe en el pipe de stdin (alimenta cin).
void stdinFeederLoop() {
    while (g_running.load()) {
        std::string line;
        if (!popInputLine(line)) break;
        line += '\n';
        ssize_t off = 0;
        ssize_t len = static_cast<ssize_t>(line.size());
        while (off < len) {
            ssize_t w = write(g_stdinPipe[1], line.data() + off, static_cast<size_t>(len - off));
            if (w > 0) { off += w; }
            else if (errno == EINTR) { continue; }
            else { break; }
        }
    }
}

// Redirige stdout/stderr/stdin a pipes internos. Devuelve false si falla.
bool instalarRedireccion() {
    if (pipe(g_stdoutPipe) != 0) { LOGE("pipe(stdout) fallo"); return false; }
    if (pipe(g_stdinPipe)  != 0) { LOGE("pipe(stdin) fallo");  return false; }

    // Sin buffering en stdout para que la UI vea el texto al instante.
    setvbuf(stdout, nullptr, _IONBF, 0);

    g_savedStdout = dup(STDOUT_FILENO);
    g_savedStdin  = dup(STDIN_FILENO);

    if (dup2(g_stdoutPipe[1], STDOUT_FILENO) < 0) { LOGE("dup2(stdout) fallo"); return false; }
    if (dup2(g_stdoutPipe[1], STDERR_FILENO) < 0) { LOGE("dup2(stderr) fallo"); return false; }
    if (dup2(g_stdinPipe[0],  STDIN_FILENO)  < 0) { LOGE("dup2(stdin) fallo");  return false; }

    // Reconectar los iostream de C++ con los FD ya redirigidos.
    std::ios::sync_with_stdio(true);
    std::cin.clear();
    std::cout.clear();
    return true;
}

void cerrarFd(int& fd) {
    if (fd >= 0) { close(fd); fd = -1; }
}

// Entry point del hilo del motor.
void engineThreadMain() {
    LOGI("Motor: iniciando iniciarJuego()");
    iniciarJuego();
    LOGI("Motor: iniciarJuego() ha terminado");
    // El juego termino por si mismo (break final). Marcamos parada.
    g_running.store(false);
    g_inputCv.notify_all();
}

} // namespace

// =========================================================
// API JNI
//   Paquete Kotlin: com.hidanhell.juegorphg
//   Clase:          GameEngine
// =========================================================
extern "C" {

JNIEXPORT void JNICALL
Java_com_hidanhell_juegorphg_GameEngine_nativeStartGame(JNIEnv* env, jobject /*thiz*/, jstring saveDir) {
    if (g_running.exchange(true)) {
        LOGI("nativeStartGame: ya en ejecucion, ignorado");
        return;
    }

    // Directorio escribible de la app (Context.filesDir en Kotlin) donde
    // guardarPartida/cargarPartida leeran y escribiran en este build.
    const char* dirUtf = env->GetStringUTFChars(saveDir, nullptr);
    IO::establecerDirectorioGuardado(dirUtf != nullptr ? dirUtf : "");
    if (dirUtf != nullptr) env->ReleaseStringUTFChars(saveDir, dirUtf);

    {
        std::lock_guard<std::mutex> lock(g_outputMutex);
        g_output.clear();
    }
    {
        std::lock_guard<std::mutex> lock(g_inputMutex);
        g_inputQueue.clear();
    }

    if (!instalarRedireccion()) {
        LOGE("nativeStartGame: fallo la redireccion de FD");
        g_running.store(false);
        return;
    }

    // Registrar las implementaciones Android en el service locator.
    // (Las llamadas ya migradas a IO:: compartiran el mismo pipe.)
    IO::establecerRender(&g_androidRender);
    IO::establecerEntrada(&g_androidEntrada);

    g_stdoutReader = std::thread(stdoutReaderLoop);
    g_stdinFeeder  = std::thread(stdinFeederLoop);
    g_engineThread = std::thread(engineThreadMain);

    LOGI("nativeStartGame: hilos lanzados");
}

// Recoge y VACIA el buffer de salida acumulado. La UI llama esto por
// polling. Devuelve "" si no hay texto nuevo.
JNIEXPORT jstring JNICALL
Java_com_hidanhell_juegorphg_GameEngine_nativeObtenerOutput(JNIEnv* env, jobject /*thiz*/) {
    std::string chunk;
    {
        std::lock_guard<std::mutex> lock(g_outputMutex);
        chunk.swap(g_output);
    }
    return env->NewStringUTF(chunk.c_str());
}

// Encola una linea de input procedente de la UI (boton, teclado numerico...).
JNIEXPORT void JNICALL
Java_com_hidanhell_juegorphg_GameEngine_nativeEnviarInput(JNIEnv* env, jobject /*thiz*/, jstring texto) {
    const char* utf = env->GetStringUTFChars(texto, nullptr);
    std::string s = (utf != nullptr) ? utf : "";
    if (utf != nullptr) env->ReleaseStringUTFChars(texto, utf);

    {
        std::lock_guard<std::mutex> lock(g_inputMutex);
        g_inputQueue.push_back(s);
    }
    g_inputCv.notify_one();
}

// true mientras el motor sigue corriendo.
JNIEXPORT jboolean JNICALL
Java_com_hidanhell_juegorphg_GameEngine_nativeEstaActivo(JNIEnv* /*env*/, jobject /*thiz*/) {
    return g_running.load() ? JNI_TRUE : JNI_FALSE;
}

// Detiene el juego y libera hilos/FD. Idempotente.
JNIEXPORT void JNICALL
Java_com_hidanhell_juegorphg_GameEngine_nativeDetener(JNIEnv* /*env*/, jobject /*thiz*/) {
    if (!g_running.exchange(false)) return;

    // Desbloquear cualquier hilo dormido.
    g_inputCv.notify_all();

    // Cerrar extremos de escritura provoca EOF en los lectores.
    cerrarFd(g_stdinPipe[1]);
    cerrarFd(g_stdoutPipe[1]);

    if (g_engineThread.joinable()) g_engineThread.detach(); // puede seguir bloqueado en cin
    if (g_stdinFeeder.joinable())  g_stdinFeeder.join();
    if (g_stdoutReader.joinable()) g_stdoutReader.join();

    // Restaurar FD originales.
    if (g_savedStdout >= 0) { dup2(g_savedStdout, STDOUT_FILENO); cerrarFd(g_savedStdout); }
    if (g_savedStdin  >= 0) { dup2(g_savedStdin,  STDIN_FILENO);  cerrarFd(g_savedStdin);  }
    cerrarFd(g_stdinPipe[0]);
    cerrarFd(g_stdoutPipe[0]);

    IO::establecerRender(nullptr);
    IO::establecerEntrada(nullptr);
    LOGI("nativeDetener: motor detenido");
}

} // extern "C"
