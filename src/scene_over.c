#include "scene_over.h"
#include "main.h"

static char *textBuffer = "GAME OVER";
static Rectangle recText;

void initSceneOver() {
    // carregando a scene
    currentScene = loadSceneData(
        "resources/gameover.png",
        "resources/sounds/gameover.mp3",
        OVER,
        &gameFont
        );

    // setando a função de desenho
    currentScene->draw = drawSceneOver;

    // configurando texto de fundo;
    recText = getTextRect(textBuffer, *currentScene->font, FONT_SIZE, FONT_SPACE);

    // configurando tecla para sair.
    SetExitKey(KEY_ESCAPE);
}


/* função de desenho da cena intro */
void drawSceneOver() {
    BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(currentScene->background, 0, 0, WHITE);
        DrawRectangleRec(recText, BLACK);
        DrawTextEx(gameFont, textBuffer, (Vector2){recText.x, recText.y}, FONT_SIZE, FONT_SPACE, WHITE);
    EndDrawing();
}


