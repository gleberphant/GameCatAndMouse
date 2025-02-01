#ifndef MAIN_H_

#define MAIN_H_

#include "raylib.h"
#include <stdlib.h>

// constantes
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MAP_BORDER 10
#define FONT_SIZE 32
#define FONT_SPACE 2.0f
#define PLAYER_INIT_POS (Vector2){ 380.0f, 300.0f }

#define SOUND_EAT_CHEESE 0
#define SOUND_EAT_STRAWBERRY 1
#define SOUND_GET_HURT 2

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