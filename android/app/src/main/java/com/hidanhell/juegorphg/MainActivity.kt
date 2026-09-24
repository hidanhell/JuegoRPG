package com.hidanhell.juegorphg

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

/**
 * Activity principal. Aloja la [GameView] (terminal de texto) y liga
 * su ciclo de vida al del motor nativo.
 *
 * El motor corre en un hilo de background propio (ver jni_bridge.cpp),
 * asi que el UI thread nunca se bloquea esperando al juego.
 */
class MainActivity : AppCompatActivity() {

    private lateinit var gameView: GameView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        gameView = findViewById(R.id.game_view)
        gameView.startGame()
    }

    override fun onDestroy() {
        gameView.stopGame()
        super.onDestroy()
    }
}
