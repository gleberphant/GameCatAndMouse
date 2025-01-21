#ifndef ITENS_H_

#define ITENS_H_

#include "raylib.h"


#define ITEM_DEFAULT_LIFE 120
#define ITEM_DEFAULT_WIDTH 32
#define ITEM_DEFAULT CHEESE


typedef enum ItemType { CHEESE=0, STRAWBERRY=1, TRAP=2, END_ITEM=3 } ItemType;


typedef struct Item{
    bool collision;
    Rectangle pointOfCollision; // onde foi que houve a colisão
    Vector2 position;
    Rectangle collisionBox; // retângulo para verificar a colisão
    ItemType type;
    Texture2D *spritesheet;
    Rectangle spriteFrame;
    int life;
} Item;


// construtor
Item* getItem(Vector2 initPos, Texture2D* spritesheet, ItemType type);

//desenhar
void drawItem(Item* self);


#endif