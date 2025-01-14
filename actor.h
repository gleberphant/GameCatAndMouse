#ifndef H_ACTOR

#define H_ACTOR

#include "raylib.h"

// TIPOS DE AÇÕES
typedef enum { MOVE, STOP } ActionType;

// TIPOS DE ATORES
typedef enum { PLAYER, ENEMY, ITEM } ObjectType;

// ATORES. SERVE PARA JOGADOR, INIMIGOS E ITENS
typedef struct Object{
    Vector2 position;
    Vector2 moviment;
    Vector2 size;
    Rectangle box;
    ObjectType type;
    Texture2D sprite;
    ActionType action;
    int life;
} Object;


#endif