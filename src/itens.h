#ifndef ITENS_H_

#define ITENS_H_

#include "raylib.h"


#define ITEM_DEFAULT_LIFE 120
#define ITEM_DEFAULT_WIDTH 32
#define ITEM_DEFAULT CHEESE


typedef enum ItemType { CHEESE=0, STRAWBERRY=1, TRAP=2, END_ITEM=3 } ItemType;


typedef struct Item{
    Rectangle collisionBox; // retângulo para verificar a colisão
    Vector2 position; // posição do item
    ItemType type;
    Texture2D *spritesheet;
    Rectangle spriteFrame;
    int life;
} Item;




Item* getItem(Vector2 initPos, Texture2D* spritesheetArray, ItemType type);

Rectangle getCollisionBox(Vector2 pos);

void drawItem(Item* self);

Texture2D* getItemSpriteSheetArray(const char *spritepathList[]);
#endif