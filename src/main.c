/* TODO
 *   - carregar mapa de arquivo
 *   - aperfeiçoar para rodar em browser.
 */

#include "main.h"
#include "animation.h"
#include "scene.h"
#include "scene_game.h"

SceneData* currentScene;
ScenesType currentSceneType = INTRO;
bool debugMode = false;
Font gameFont;

/**
 * @brief Initializes the game resources and sets the initial game state.
 */
void initGame() {
    // config screen
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");
    SetTargetFPS(30);

    // Loadind screen
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Carregando...", 10, 10, 60, WHITE);
    EndDrawing();

    // config audio
    InitAudioDevice();
    SetMasterVolume(1.0f);
    
    // set variables
    SetTraceLogLevel(LOG_DEBUG);   // Set debugging level for logs
    gameFont = GetFontDefault();   // Use the default font
    currentSceneType = INTRO;      // Start with the INTRO scene

    
}

/**
 * @brief Executes the primary loop controlling the transitions between scenes in the application.
 */
void gameLoop() {
    while (currentSceneType != EXIT) {

        if (currentSceneType == GAME) {
            runSceneGame();
            currentSceneType = OVER;
        } else {
            runScene(currentSceneType);
        }
    }
}



/**
 * @brief Unloads game resources and closes the application.
 */
void closeGame() {
    CloseAudioDevice();
    CloseWindow();
}


/**
 * @brief Entry point of the application.
 * @return An integer value, typically 0, indicating the successful execution of the program.
 */
int main(int argc , char *argv[]) {
 
    initGame();          // Inicializa o estado e os recursos do jogo
 
    gameLoop();      // Executa o loop principal do jogo
 
    closeGame();        // Libera os recursos utilizados no jogo

    return 0;            // Retorna 0 para indicar execução bem-sucedida
}