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
    ScenesType type, nextScene;
    void (*draw)();
    void (*update)();
    void (*input)();

} SceneData;

SceneData* initScene(ScenesType sceneType);

void inputScene(SceneData *scene);

void updateScene(SceneData *scene);

void drawScene(SceneData *scene);

void loopScene(SceneData *scene);

void unloadScene(SceneData *scene);

void runScene(ScenesType sceneType);

//--------- UTILS -------------------------------------

SceneData* loadSceneData(const char* backgroundPath, const char* musicPath, ScenesType type, Font* font);

Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space);


#endif //UTILS_H
