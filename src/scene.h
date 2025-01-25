//
// Created by glebe on 22/01/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "main.h"
#include "raylib.h"

// ENUMS
typedef enum ScenesType{
    INTRO,
    MENU,
    GAME,
    OVER,
    EXIT
} ScenesType;


typedef struct SceneData {
    Music music;
    Texture2D background;
    Font* font;
    ScenesType sceneType, nextScene;
    void (*draw)();
} SceneData;


// ---- utils

SceneData* loadSceneData(const char* backgroundPath, const char* musicPath, ScenesType type, Font* font);

void sceneInputHandler();

void drawScene(SceneData *scene);

void playSceneLoop(SceneData *scene);

void unloadScene(SceneData* scene);

Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space);
bool loadMusic(Music *music, const char *filepath);
#endif //UTILS_H
