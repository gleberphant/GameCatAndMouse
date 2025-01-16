#include "itens.h"

Item* getItem(Vector2 initPos, char* sprite, ItemType type){
    Texture2D temp = LoadTexture(sprite);

    return &(Item){
        .sprite = temp,
        .position = initPos,
        .box = (Rectangle){initPos.x , initPos.y , temp.width, temp.height},
        .type = type,
        .life = ITEM_DEFAULT_LIFE
    };
}

void drawItem(Item* target){

    DrawTexture(target->sprite, target->box.x, target->box.y, WHITE);
}