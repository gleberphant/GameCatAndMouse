#include "scene_intro.h"
#include "actor.h"
#include "main.h"

#include <stdio.h>


const char *textIntro = "[ENTER] para jogar [ESC] para sair.";
Rectangle recText;
int records[5];


/**
 * @brief Inicializa a cena de introdução.
 * 
 * Esta função carrega os recursos necessários para a cena de introdução e define as funções de entrada, atualização e desenho.
 * 
 * @return SceneData* Retorna um ponteiro para os dados da cena inicializada.
 */
SceneData* initSceneIntro() {
    SceneData* scene;
    // carregando a scene
    scene = loadSceneData(
        "resources/intro_bg.png",
        "resources/sounds/intro_music.ogg",
        INTRO,
        &gameFont
        );

    scene->nextScene = GAME;
    // setando a função de desenho
    scene->input = inputSceneIntro;
    scene->update = updateSceneIntro;
    scene->draw = drawSceneIntro;

    // configurando tecla de escape
    SetExitKey(KEY_ESCAPE);

    // carregar recorde de pontuação
    FILE* file = fopen("resources/score.data", "rb");

    if (file != NULL ) {

        fread(records, sizeof(int), 5, file);
    }

    fclose(file);

    return scene;
}

/**
 * @brief Processa eventos de entrada para a cena de introdução.
 * 
 * Esta função lida com eventos de entrada, como pressionar a tecla Enter ou clicar com o mouse para iniciar a próxima cena.
 */
void inputSceneIntro() {

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ) {
        // seleciona a proxima cena
        currentSceneType = currentScene->nextScene;
    }
 }

/**
 * @brief Atualiza a lógica da cena de introdução.
 * 
 * Esta função atualiza a lógica da cena de introdução.
 */
void updateSceneIntro() {

 }

/**
 * @brief Desenha a cena de introdução na tela.
 * 
 * Esta função renderiza a cena de introdução na tela.
 */
/* função de desenho da cena intro */
void drawSceneIntro() {
    BeginDrawing();
        // limpa background
        ClearBackground(BLACK);

        // desenha background
    DrawTexturePro(currentScene->background,
        (Rectangle){0, 0, currentScene->background.width, currentScene->background.height},
        (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
        (Vector2){ 0, 0 },
        0,
        WHITE);

        
        // desenha recorders
        DrawRectangleRec(
                (Rectangle){780, 220, 200, 220},
                ColorAlpha(DARKGRAY, 0.8f)
        );

        DrawText("RECORDES", 790, 225, 30, RAYWHITE);

        for (int i = 0; i < sizeof(records)/sizeof(records[0]); i++) {
            DrawText(TextFormat(" + %03d pts", records[i]), 790, 275+(i*25), 20, RAYWHITE);
        }



    EndDrawing();
}

