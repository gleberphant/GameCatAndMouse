#include "scene.h"
#include "scene_intro.h"
#include "scene_over.h"
#include "scene_game.h"
#include "main.h"


SceneData* initScene(ScenesType sceneType){

    SceneData* temp;
    // Inicializa a cena conforme o tipo atual.
    switch (sceneType ) {
        case INTRO:
            temp = initSceneIntro();
            break;
        case OVER:
            temp = initSceneOver();
            break;
        default:
            temp = NULL;
            break;
    }
    return temp;
}


/**
 * @brief Handles input events for the current scene.
 * @param scene The current scene to process input events.
 */
void inputScene(SceneData *scene) {

    if (WindowShouldClose()) {
        currentSceneType = EXIT;
        return;
    }

    // DEBUG OPTIONS
    if (IsKeyReleased(KEY_F1)) {
        debugMode = !debugMode;
    }

    if (IsKeyReleased(KEY_KP_ADD)) {
        volumeMaster += 0.05f;
        SetMasterVolume(volumeMaster);
    }

    if (IsKeyReleased(KEY_KP_SUBTRACT)) {
        volumeMaster -= 0.05f;
        SetMasterVolume(volumeMaster);
    }

    scene->input();
}

void updateScene(SceneData *scene) {
    UpdateMusicStream(scene->music);
    scene->update();
}

void drawScene(SceneData *scene) {
    scene->draw();
}
/**
 * @brief Executes the main loop of the current scene.
 * @param scene The current scene to execute
 */
void loopScene(SceneData *scene) {

    // preparação da cena
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);
    TraceLog(LOG_DEBUG, TextFormat(" == Play music " ));
    // loop cena
    while (currentSceneType == scene->type) {

        inputScene(scene);

        updateScene(scene);

        drawScene(scene);

    }
    

}

void unloadScene(SceneData* scene) {
    UnloadMusicStream(scene->music);
    UnloadTexture(scene->background);
}


/**
 * @brief Manages the current scene's lifecycle, including initialization, execution, and cleanup.
 */
void runScene(ScenesType sceneType) {

    // Preparando a cena
    TraceLog(LOG_DEBUG, " == INICIANDO SCENE ");
    SceneData* scene = initScene(sceneType);
    currentScene = scene;
        
    // Prepara a música para ser tocada
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);


    // Executa o loop principal da cena
    TraceLog(LOG_DEBUG, " == SCENE LOOP ");
    loopScene(scene);

    // Finaliza a cena
    TraceLog(LOG_DEBUG, " == FINALIZANDO SCENE  ");
    StopMusicStream(scene->music);

    unloadScene(scene);

    // Libera memória associada à cena
    if (scene != NULL) {
        free(scene);
        scene = NULL;
    }
}

// utils
SceneData* loadSceneData(const char* backgroundPath, const char* musicPath, ScenesType type, Font* font) {

    TraceLog(LOG_DEBUG, TextFormat(" == Load Scene Data %d bytes ", sizeof(SceneData) ));
    SceneData* scene = malloc( sizeof(SceneData) );

    if (scene == NULL) {
        TraceLog(LOG_ERROR, "  !!  error ao carregar SCENE !!");
        return NULL;
    }
    
    scene->music = LoadMusicStream(musicPath);
    scene->background = LoadTexture(backgroundPath);
    scene->font = font;
    scene->type = type;
    scene->nextScene = OVER;

    SetMusicVolume(scene->music, 1.0f);

    return scene;
}

Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space) {
    const Vector2 textMeasure = MeasureTextEx(font, text, fontSize, space);
    return (Rectangle){(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};
}
