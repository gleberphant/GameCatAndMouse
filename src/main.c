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
 * @brief Inicializa os recursos do jogo e define o estado inicial do jogo.
 */
void initGame() {
    // configura a tela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");
    SetTargetFPS(30);

    // Tela de carregamento
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Carregando...", 10, 10, 60, WHITE);
    EndDrawing();

    // configura o áudio
    InitAudioDevice();
    SetMasterVolume(1.0f);
    
    // define variáveis
    SetTraceLogLevel(LOG_DEBUG);   // Define o nível de depuração para logs
    gameFont = GetFontDefault();   // Usa a fonte padrão
    currentSceneType = INTRO;      // Começa com a cena INTRO

    
}

/**
 * @brief Executa o loop principal controlando as transições entre cenas no aplicativo.
 */
void gameLoop() {
    while (currentSceneType != EXIT) {

        if (currentSceneType == GAME) {
            runSceneGame();
            currentSceneType = OVER;
            TraceLog(LOG_INFO, "Game Over");
        } else {
            TraceLog(LOG_DEBUG, "Iniciando Cena %d", currentSceneType);
            runScene(currentSceneType);
            TraceLog(LOG_DEBUG, "próxima cena = %d", currentSceneType);
        }
    }
}

/**
 * @brief Descarrega os recursos do jogo e fecha o aplicativo.
 */
void closeGame() {
    CloseAudioDevice();
    CloseWindow();
}

/**
 * @brief Ponto de entrada do aplicativo.
 * @return Um valor inteiro, tipicamente 0, indicando a execução bem-sucedida do programa.
 */
int main(int argc , char *argv[]) {
 
    initGame();          // Inicializa o estado e os recursos do jogo
 
    gameLoop();      // Executa o loop principal do jogo
 
    closeGame();        // Libera os recursos utilizados no jogo

    return 0;            // Retorna 0 para indicar execução bem-sucedida
}