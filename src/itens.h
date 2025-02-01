#ifndef ITENS_H_

#define ITENS_H_

#include "raylib.h"
#include "actor.h"
#include "main.h"

#define ITEM_DEFAULT_LIFE 120
#define ITEM_DEFAULT_WIDTH 32


typedef enum ItemType { CHEESE=0, STRAWBERRY=1, TRAP=2, END_ITEM=3 } ItemType;


typedef struct Item{
    Rectangle collisionBox; // retângulo para verificar a colisão
    Vector2 position; // posição do item
    ItemType type;
    Texture2D *spritesheet;
    Rectangle spriteFrame;
    int life;
} Item;




Item* loadNewItem(Vector2 initPos, Texture2D* spritesheetArray, ItemType type);
void setItemPosition(Item* item, Vector2 position);

Rectangle getItemCollisionBox(Vector2 position);
Vector2 getItemPosition(Rectangle box);
void itemGetHit(Actor *target, Item *item, Sound *sound);
void drawItem(Item* self);
ItemType getRandomItemType();
Texture2D* loadItemSpriteSheetArray(const char *spritepathList[]);
void unloadItemSpriteSheet(Texture2D *itemSpriteSheet) ;


#endif