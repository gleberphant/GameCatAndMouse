#ifndef ITENS_H_

#define ITENS_H_

#include "raylib.h"
#include "raymath.h"

#include <stdlib.h>

#define ITEM_DEFAULT_LIFE 10
#define ITEM_DEFAULT_WIDTH 32
#define ITEM_DEFAULT CHEESE


typedef enum ItemType { CHEESE, STROWBERRY, TRAP } ItemType;


typedef struct Item{
    Vector2 position;
    Rectangle box;
    ItemType type;
    Texture2D sprite;
    int life;
} Item;


// construtor
Item* getItem(Vector2 initPos, char* sprite, ItemType type);

//desenhar
void drawItem(Item* target);


#endif