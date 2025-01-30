#include "scene_over.h"
#include "scene.h"
#include "main.h"

static char *textBuffer = "GAME OVER";
static Rectangle recText;


/**
 * @brief Inicializa a cena de game over.
 * 
 * Esta função carrega os recursos necessários para a cena de game over e define as funções de entrada, atualização e desenho.
 * 
 * @return SceneData* Retorna um ponteiro para os dados da cena inicializada.
 */
SceneData* initSceneOver() {
    TraceLog(LOG_DEBUG, " == INICIANDO SCENE_OVER");
    SceneData* scene;
    // carregando a scene
    scene = loadSceneData(
        "resources/gameover.png",
        "resources/sounds/gameover.mp3",
        OVER,
        &gameFont
        );
    
    // define proxima cena
    scene->nextScene = INTRO;

    // setando a função de desenho
    scene->draw = drawSceneOver;
    scene->input = inputSceneOver;
    scene->update = updateSceneOver;


    // configurando tecla para sair.
    SetExitKey(KEY_ESCAPE);

    return scene;
}


/**
 * @brief Processa eventos de entrada para a cena de game over.
 * 
 * Esta função lida com eventos de entrada, como pressionar a tecla Enter para iniciar a próxima cena.
 */
void inputSceneOver() {

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) ) {
        // seleciona a proxima cena
        currentSceneType = currentScene->nextScene;
        return;
    }

    return;
}


/**
 * @brief Atualiza a lógica da cena de game over.
 * 
 * Esta função atualiza a lógica da cena de game over.
 */
void updateSceneOver() {
    return;
}


/**
 * @brief Desenha a cena de game over na tela.
 * 
 * Esta função renderiza a cena de game over na tela.
 */
void drawSceneOver() {
    BeginDrawing();

        ClearBackground(BLACK);
        DrawTexturePro(currentScene->background,
        (Rectangle){0, 0, currentScene->background.width, currentScene->background.height},
        (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
        (Vector2){ 0, 0 },
        0,
        WHITE);

    EndDrawing();
}


