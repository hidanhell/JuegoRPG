package com.hidanhell.juegorphg

import android.content.Context
import android.util.AttributeSet
import android.view.LayoutInflater
import android.view.inputmethod.EditorInfo
import android.widget.Button
import android.widget.EditText
import android.widget.FrameLayout
import android.widget.ScrollView
import android.widget.TextView

/**
 * Vista de terminal de texto para el RPG.
 *
 * Muestra la salida del motor en un [TextView] con scroll y ofrece
 * dos vias de input:
 *   - Botones numericos 1-5 (la mayoria de menus del juego usan
 *     opciones numericas).
 *   - Un [EditText] libre para texto arbitrario (nombre del
 *     personaje, W/A/S/D de movimiento, S/N de confirmaciones...).
 *
 * No contiene logica de juego: solo traduce texto <-> motor a traves
 * de [GameEngine]. El texto se maneja como UTF-8 de extremo a extremo
 * (NewStringUTF en C++, String de Kotlin en UTF-16 internamente).
 */
class GameView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyle: Int = 0
) : FrameLayout(context, attrs, defStyle) {

    private val engine = GameEngine()

    private val outputView: TextView
    private val scrollView: ScrollView
    private val inputField: EditText

    // Buffer local del texto ya renderizado. Se recorta para no crecer
    // sin limite en partidas largas.
    private val screen = StringBuilder()

    init {
        LayoutInflater.from(context).inflate(R.layout.view_game, this, true)

        scrollView = findViewById(R.id.scroll_output)
        outputView = findViewById(R.id.text_output)
        inputField = findViewById(R.id.input_field)

        // Botones numericos 1..5
        intArrayOf(
            R.id.btn_1, R.id.btn_2, R.id.btn_3, R.id.btn_4, R.id.btn_5
        ).forEachIndexed { index, id ->
            findViewById<Button>(id).setOnClickListener {
                send((index + 1).toString())
            }
        }

        findViewById<Button>(R.id.btn_enter).setOnClickListener {
            send(inputField.text.toString())
            inputField.text.clear()
        }

        // Enviar tambien con la tecla "done" del teclado.
        inputField.setOnEditorActionListener { _, actionId, _ ->
            if (actionId == EditorInfo.IME_ACTION_DONE || actionId == EditorInfo.IME_ACTION_SEND) {
                send(inputField.text.toString())
                inputField.text.clear()
                true
            } else {
                false
            }
        }
    }

    /** Arranca el motor. Llamar desde onCreate/onStart de la Activity. */
    fun startGame() {
        // Context.filesDir es privado a la app y escribible sin permisos
        // especiales; el CWD del proceso (que es lo que usa el motor en
        // desktop) no lo es en Android.
        val saveDir = context.filesDir.resolve("savegames").absolutePath
        engine.start(
            saveDir = saveDir,
            onOutput = { chunk -> appendOutput(chunk) },
            onFinished = { appendOutput("\n\n[ El juego ha terminado ]\n") }
        )
    }

    /** Detiene el motor. Llamar desde onDestroy de la Activity. */
    fun stopGame() {
        engine.stop()
    }

    private fun send(text: String) {
        // Eco local del input para que el jugador vea lo que envio.
        appendOutput("> $text\n")
        engine.send(text)
    }

    /**
     * Aplica un fragmento de salida del motor. Interpreta el sentinela
     * de limpieza de pantalla ([GameEngine.CLEAR_SCREEN] = '\f').
     */
    private fun appendOutput(chunk: String) {
        val clearIdx = chunk.lastIndexOf(GameEngine.CLEAR_SCREEN)
        if (clearIdx >= 0) {
            // Todo lo anterior al ultimo '\f' se descarta (pantalla limpia);
            // se conserva lo que venga despues.
            screen.setLength(0)
            screen.append(chunk.substring(clearIdx + 1))
        } else {
            screen.append(chunk)
        }

        // Recorte de seguridad para partidas largas.
        if (screen.length > MAX_CHARS) {
            screen.delete(0, screen.length - MAX_CHARS)
        }

        outputView.text = screen.toString()
        scrollView.post { scrollView.fullScroll(ScrollView.FOCUS_DOWN) }
    }

    private companion object {
        const val MAX_CHARS = 40_000
    }
}
