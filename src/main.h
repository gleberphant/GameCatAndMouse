#ifndef MAIN_H_

#define MAIN_H_

#include "raylib.h"
#include "scene.h"
#include <stdlib.h>

// constantes
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAP_BORDER 10
#define FONT_SIZE 32
#define FONT_SPACE 2.0f
#define PLAYER_INIT_POS (Vector2){ 380.0f, 300.0f }

// declaração forwad
typedef enum ScenesType ScenesType;  // Declaração forward
typedef struct SceneData SceneData;  // Declaração forward


typedef struct savefile{
    int score, level;
} savefile;


// VARIÁVEIS GLOBAIS
extern ScenesType currentSceneType;
extern SceneData* currentScene;
extern Font gameFont;
extern bool debugMode;
extern float volumeMaster;
extern int score;
extern int level;

void initGame();
void gameLoop();
void closeGame();


#endif