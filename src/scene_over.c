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


    // configurando tecla para sair.
    SetExitKey(KEY_ESCAPE);

    return scene;
}


void inputSceneOver() {

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) ) {
        // seleciona a proxima cena
        currentSceneType = currentScene->nextScene;
        return;
    }

    return;
}

void updateSceneOver() {
    return;
}


/* função de desenho da cena intro */
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


