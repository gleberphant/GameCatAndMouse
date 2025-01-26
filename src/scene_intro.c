#include "scene_intro.h"
#include "actor.h"
#include "main.h"

#include <stdio.h>


const char *textIntro = "[ENTER] para jogar [ESC] para sair.";
Rectangle recText;
int records[5];

SceneData* initSceneIntro();

SceneData* initSceneIntro() {
    SceneData* scene;
    // carregando a scene
    scene = loadSceneData(
        "resources/intro_bg.png",
        "resources/sounds/intro_music.mp3",
        INTRO,
        &gameFont
        );

    scene->nextScene = GAME;
    // setando a função de desenho
    scene->input = inputSceneIntro;
    scene->update = updateSceneIntro;
    scene->draw = drawSceneIntro;

    // configurando texto de fundo;
    recText = getTextRect(textIntro, gameFont, FONT_SIZE, FONT_SPACE);

    // configurando tecla de escape
    SetExitKey(KEY_ESCAPE);

    // carregar recorde de pontuação
    FILE* file = fopen("score.dat", "rb");

    if (file != NULL ) {

        fread(records, sizeof(int), 5, file);
    }

    fclose(file);

    return scene;
}


void inputSceneIntro() {
    return;
 }

void updateSceneIntro() {
    return;
 }

/* função de desenho da cena intro */
void drawSceneIntro() {
    BeginDrawing();
        // limpa background
        ClearBackground(BLACK);

        // desenha background
        DrawTexture(currentScene->background, 0, 0, WHITE);
        
        // desenha recorders
        DrawRectangleRec(
                (Rectangle){580, 20, 200, 220},
                ColorAlpha(DARKGRAY, 0.8f)
                );

        DrawText("RECORDES", 590, 25, 30, RAYWHITE);

        for (int i = 0; i < sizeof(records)/sizeof(records[0]); i++) {
            DrawText(TextFormat(" + %03d pts", records[i]), 590, 70+(i*25), 20, RAYWHITE);
        }

        DrawRectangleRec(recText, ColorAlpha(DARKGRAY, 0.8f));

        // desenha texto titulo  
        DrawTextEx(gameFont, textIntro, (Vector2){recText.x, recText.y}, FONT_SIZE, FONT_SPACE, WHITE);

    EndDrawing();
}

