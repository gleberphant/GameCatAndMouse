#include <stdio.h>

/* TODO
 *   - aperfeiçoar comportamento de perseguição e adotar comportamentos aleatorios.
 *   - REFATORAR. MODULARIZAR CENA GAME LOP
 *   - carregar mapa de arquivo
 *   - implementar uma load screen
 *   - aperfeiçoar para rodar em browser.
 */

#include "main.h"
#include "raymath.h"


#include "animation.h"
#include "actor.h"
#include "lists.h"
#include "itens.h"
#include "maps.h"
#include "scene_over.h"
#include "scene_intro.h"
#include "scene_game.h"

SceneData* currentScene;
ScenesType currentSceneType = INTRO;
bool debugMode = true;
Font gameFont;


/**
 * @brief Executes the primary loop controlling the transitions between scenes in the application.
 */
void runGame() {
    while (currentSceneType != EXIT) {

        if (currentSceneType == GAME) {
            gameLoop();
            currentSceneType = OVER;
        } else {
            manageScene(currentSceneType);
        }
    }
}

/**
 * @brief Configures the display settings for the application.
 */
void configureScreen() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");
    SetTargetFPS(30);
}

/**
 * @brief Configures the audio system and sets the master volume level.
 * @param masterVolume The desired master volume level, a float value typically between 0.0 (silent) and 1.0 (maximum volume).
 */
void configureAudio() {
    InitAudioDevice();
    SetMasterVolume(1.0f);
}

/**
 * @brief Initializes the game resources and sets the initial game state.
 */
void initGame() {
    configureScreen();
    configureAudio();  // Using global variable volumeMaster

    SetTraceLogLevel(LOG_DEBUG);   // Set debugging level for logs
    gameFont = GetFontDefault();   // Use the default font
    currentSceneType = INTRO;      // Start with the INTRO scene

}


/**
 * @brief Unloads game resources and closes the application.
 */
void unloadGame() {
    CloseAudioDevice();
    CloseWindow();
}

void run(){

    initGame();          // Inicializa o estado e os recursos do jogo
 
    runGame();      // Executa o loop principal do jogo
 
    unloadGame();        // Libera os recursos utilizados no jogo

}

/**
 * @brief Entry point of the application.
 * @return An integer value, typically 0, indicating the successful execution of the program.
 */
int main() {
 
    run();              // Executa o jogo
    return 0;            // Retorna 0 para indicar execução bem-sucedida
}