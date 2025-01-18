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
typedef enum DirectionType { UP=0, RIGHT=90, DOWN=180, LEFT=270 } DirectionType ;
typedef enum ActorType { PLAYER, ENEMY, ITEM } ActorType;


// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Actor{
    int life;
    float direction;
    
    Vector2 position;
    Vector2 velocity;
    
    Rectangle collisionBox;
    ActorType type;
    ActionType action;
    Animation* spriteA2[3];
    
} Actor;


void setActor(Actor* target, Vector2 initPos, char* sprite);

void drawActor(Actor* target);


#endif