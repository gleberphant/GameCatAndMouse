//
// Created by glebe on 22/01/2025.
//

//
// Created by gleber on 22/01/2025.
//

#include "scene.h"
#include "main.h"


// ---- utils
Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space) {
    const Vector2 textMeasure = MeasureTextEx(font, text, fontSize, space);
    return (Rectangle){(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};
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
    scene->sceneType = type;

    SetMusicVolume(scene->music, 1.0f);

    return scene;
}

void sceneInputHandler(SceneData *scene) {
    // INPUT HANDLE
    if (WindowShouldClose()) {
        currentSceneType = EXIT;
        return;
    }

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        // seleciona a proxima cena
        switch (scene->sceneType) {
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
                currentSceneType = scene->sceneType;
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


void drawScene(SceneData *scene) {

    scene->draw();

}


void playSceneLoop(SceneData *scene) {

    // preparação da cena
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);

    // loop cena
    while (currentSceneType == scene->sceneType) {

        sceneInputHandler(scene);

        UpdateMusicStream(scene->music);

        drawScene(scene);

    }

    // fechar cena
    unloadScene(scene);
}

void unloadScene(SceneData* scene) {
    // DESCARREGAR MUSICA
    StopMusicStream(scene->music);
    UnloadMusicStream(scene->music);

    // DESCARREGAR IMAGENS
    UnloadTexture(scene->background);
}

// carregar um objeto musica com tratamento de error
bool loadMusic(Music *music, const char *filepath) {
    *music = LoadMusicStream(filepath);
    if (music->stream.buffer == NULL) {
        TraceLog(LOG_ERROR, ":: ERRO AO CARREGAR A MUSICA");
        return false;
    }
    return true;
}
