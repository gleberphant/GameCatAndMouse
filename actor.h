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
typedef enum ActorType { PLAYER, ENEMY, ITEM } ActorType;


// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Actor{
    int life;
    float direction;
    
    Vector2 position;
    Vector2 move;
    
    Rectangle box;
    ActorType type;
    ActionType action;
    Animation* spriteA2[3];
    
} Actor;


void setObject(Actor* target, Vector2 initPos, char* sprite);

void drawObject(Actor* target);


#endif