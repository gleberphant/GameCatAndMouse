#ifndef ACTOR_H_

#define ACTOR_H_

#include "raylib.h"
#include "raymath.h"


#include "animation.h"
#include <string.h>


#define DEFAULT_LIFE 50
#define PLAYER_DEFAULT_WIDTH 64
#define ENEMY_DEFAULT_WIDTH 128

// TIPOS 
typedef enum ActionType { MOVE=0, STOP=1, SPECIAL=2  } ActionType;
typedef enum DirectionType { UP=0, RIGHT=1, DOWN=2, LEFT=3 } DirectionType ;
typedef enum ObjectType { PLAYER, ENEMY, ITEM } ObjectType;


// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Object{
    int life;
    float direction;
    
    Vector2 position;
    Vector2 move;
    
    Rectangle box;
    ObjectType type;
    ActionType action;
    //Animation* spriteA;
    Animation* spriteA2[3];
    
} Object;


void setObject(Object* target, Vector2 initPos, char* sprite);

void drawObject(Object* target);


#endif