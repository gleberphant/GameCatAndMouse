#include "scene_over.h"
#include "scene.h"
#include "main.h"

static char *textBuffer = "GAME OVER";
static Rectangle recText;


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
    
    // configurando texto de fundo;
    recText = getTextRect(textBuffer, *currentScene->font, FONT_SIZE, FONT_SPACE);

    // configurando tecla para sair.
    SetExitKey(KEY_ESCAPE);

    return scene;
}


void inputSceneOver() {
    return;
}

void updateSceneOver() {
    return;
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


