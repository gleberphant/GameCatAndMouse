#include "scene.h"
#include "main.h"


void initScene(ScenesType sceneType){

    // Inicializa a cena conforme o tipo atual.
    switch (sceneType ) {
        case INTRO:
            initSceneIntro();
            break;
        case OVER:
            initSceneOver();
            break;
        case GAME:
            initSceneGame();
            break;
        default:
            break;
    }
}


/**
 * @brief Manages the current scene's lifecycle, including initialization, execution, and cleanup.
 */
void runScene(ScenesType sceneType) {
    // Carrega a cena conforme o tipo atual.
    initScene(sceneType);    

    // Prepara a música para ser tocada
    PlayMusicStream(currentScene->music);
    SetMusicVolume(currentScene->music, volumeMaster);

    // Executa o loop principal da cena
    sceneLoop();

    // Finaliza a cena
    unloadScene(currentScene);

    // Libera memória associada à cena
    if (currentScene != NULL) {
        free(currentScene);
        currentScene = NULL;
    }
}


/**
 * @brief Executes the loop for the active scene, handling its logic and rendering.
 */
void sceneLoop() {

    while (currentSceneType == currentScene->type) {

        sceneInputHandler(currentScene);
        UpdateMusicStream(currentScene->music);
        drawScene(currentScene);

    }
}


SceneData* loadSceneData(const char* backgroundPath, const char* musicPath, ScenesType type, Font* font) {

    TraceLog(LOG_DEBUG, " == CARREGANDO SCENE ");

    SceneData* scene = malloc(sizeof(SceneData));

    if (scene == NULL) {
        TraceLog(LOG_ERROR, "  !!  error ao carregar SCENE !!");
        return NULL;
    }

    scene->music = LoadMusicStream(musicPath);
    scene->background = LoadTexture(backgroundPath);
    scene->font = font;
    scene->type = type;

    SetMusicVolume(scene->music, 1.0f);

    return scene;
}


void sceneInputHandler(SceneData *scene) {
    if (WindowShouldClose()) {
        currentSceneType = EXIT;
        return;
    }

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // seleciona a proxima cena
        switch (scene->type) {
            case INTRO:
                currentSceneType = GAME;
            break;
            case GAME:
                currentSceneType = OVER;
            break;
            case OVER:
                currentSceneType = INTRO;
            break;
            default:
                currentSceneType = scene->type;
            break;

        }
        return;
    }

    if (IsKeyReleased(KEY_KP_ADD)) {
        volumeMaster += 0.05f;
        SetMasterVolume(volumeMaster);
    }

    if (IsKeyReleased(KEY_KP_SUBTRACT)) {
        volumeMaster -= 0.05f;
        SetMasterVolume(volumeMaster);
    }

}

void playSceneLoop(SceneData *scene) {

    // preparação da cena
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);

    // loop cena
    while (currentSceneType == scene->type) {

        sceneInputHandler(scene);

        UpdateMusicStream(scene->music);

        scene->draw();

    }
    // fechar cena
    StopMusicStream(scene->music);
    unloadScene(scene);
}

void unloadScene(SceneData* scene) {
    UnloadMusicStream(scene->music);
    UnloadTexture(scene->background);
}

// utils
Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space) {
    const Vector2 textMeasure = MeasureTextEx(font, text, fontSize, space);
    return (Rectangle){(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};
}
