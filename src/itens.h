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
    bool collision;
    Rectangle pointOfCollision; // onde foi que houve a colisão
    Vector2 position;
    Rectangle collisionBox; // retângulo para verificar a colisão
    ItemType type;
    Texture2D sprite;
    int life;
} Item;


// construtor
Item* getItem(Vector2 initPos, const char* sprite, ItemType type);

//desenhar
void drawItem(Item* target);


#endif