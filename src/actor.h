#ifndef ACTOR_H_

#define ACTOR_H_

#include "raylib.h"
#include "raymath.h"


#include "animation.h"

#define DEFAULT_LIFE 50
#define PLAYER_DEFAULT_WIDTH 64
#define ENEMY_DEFAULT_WIDTH 128

// TIPOS 
typedef enum ActionType { STOP=0, MOVE=1, STUN=2, END=3  } ActionType;
typedef enum DirectionType { UP=0, RIGHT=90, DOWN=180, LEFT=270 } DirectionType ;
typedef enum ActorType { PLAYER=0, ATTACK=1, GUARD_CHEESE=2, GUARD_STRAW=3, SLEEPER=4, CRAZY=5 } ActorType;


// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Actor{
    int life, count;
    float direction;
    bool collision;

    Vector2 position; // posição centralizada em relação ao tilemap.
    Vector2 oldPosition;
    float speed;
    
    Rectangle hitBox;  // box de colisão com os objetos.
    ActorType behavior;
    ActionType action;
    Animation* spriteA2[3];
    
} Actor;

Actor* loadNewActor(Vector2 initPos, Texture2D* spritesheet);
Texture2D* loadActorSpriteSheetArray( const char* sprite);
void unloadActorSpriteSheet(Texture2D *actorSpriteSheetArray);
void setActor(Actor* self, Vector2 initPos, Texture2D* spritesheet);

void setActorInitPosition(Actor* self, Vector2 position);
Rectangle getActorHitBox(Actor* target, bool cat);

Vector2 getActorPosition(Rectangle box);



void actionStop(Actor* self);
void actionMove(Actor* self) ;
void actionSpecial(Actor* self, Actor* target);
void getHurt(Actor* self);

void drawActor(Actor* self);


#endif