#ifndef ACTOR_H_
#define ACTOR_H_

#include "raylib.h"
#include "raymath.h"


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_LIFE 50
#define PLAYER_DEFAULT_WIDTH 64
#define ENEMY_DEFAULT_WIDTH 128

// TIPOS DE AÇÕES
typedef enum ActionType { MOVE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, STOP } ActionType;
typedef enum DirectionType { UP=0, DOWN=180, LEFT=270, RIGHT=90 } DirectionType ;

// TIPOS DE ATORES
typedef enum ObjectType { PLAYER, ENEMY, ITEM } ObjectType;

// ANIMAÇÃO
typedef struct Animation{
    
    int totalFrames;
    int currentFrame;
    int frameCount;

    float length; // duração da animação
    float frameDelay; // fps da animação = totalFrames / length
   
    Rectangle frameRec;
    Texture2D spritesheet;

} Animation;

// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Object{
    Vector2 position;
    Vector2 move;
    float direction;
    Rectangle box;
    ObjectType type;
    Texture2D sprite;
    ActionType action;
    Animation* spriteA;
    int life;
} Object;



void setObject(Object* target, Vector2 initPos, char* sprite);

Animation* setAnimation(char* spritesheet);

void updateAnimation(Animation *self);

void drawObject(Object* target);


#endif