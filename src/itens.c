#include "itens.h"

Item* getItem(Vector2 initPos, char* sprite, ItemType type){
    Texture2D temp = LoadTexture(sprite);
    Item* item = malloc(sizeof(Item));

    item->sprite = temp;
    item->position = initPos;
    item->box = (Rectangle){initPos.x , initPos.y , temp.width, temp.height};
    item->type = type;
    item->life = ITEM_DEFAULT_LIFE;

    return item;
}

void drawItem(Item* target){

    DrawTexture(target->sprite, target->box.x, target->box.y, WHITE);
}