#ifndef MAIN_H_

#define MAIN_H_

#include "raylib.h"
#include "raymath.h"



#include "animation.h"
#include "actor.h"
#include "lists.h"
#include "itens.h"
#include "maps.h"

// constantes
#define W_WIDTH 800
#define W_HEIGHT 600
#define MAP_BORDER 10
#define FONT_SIZE 32
#define FONT_SPACE 2.0f

#define PLAYER_INIT_POS (Vector2){ 380.0f, 300.0f }


// ENUMS
typedef enum GameStatus{
    INTRO,
    MENU,
    GAME,
    OVER,
    EXIT
} GameStatus;

// VARIÁVEIS GLOBAIS
extern GameStatus gameScene;
extern Font gameFont;


extern bool debugMode;
extern float volume;
extern int score;
extern int level;

extern char textBuffer[100];


#endif