#include "itens.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

/*
 * o spritesheet pode ter qualquer altura, desde que cada item seja separado por 64 pixels.
 * a altura do spritesheet é o raio da area de colisão.
 */

Rectangle getItemCollisionBox(Vector2 position) {
    return (Rectangle){
        .width  = 40, //item->spriteFrame.width -32;
        .height = 40, //item->spriteFrame.height;
        .x = position.x - 20,
        .y = position.y - 20
    };

}

Vector2 getItemPosition(Rectangle box) {
    return (Vector2){
        .x = box.x + (box.width/2),
        .y = box.y + (box.height/2)
    };
}

void setItemPosition(Item* item, Vector2 position) {

    item->collisionBox = (Rectangle){
        .x = position.x,
        .y = position.y,
        .width = 40,
        .height = 40
    };

    item->position = getItemPosition(item->collisionBox);

}

Item* loadNewItem(Vector2 initPos, Texture2D *spritesheetArray, ItemType type) {
    Item* item = malloc(sizeof(Item));

    // define o spritesheet
    item->spritesheet = &spritesheetArray[type];

    // calcula total de sprites no spritesheet
    int spriteCounts = (int)ceilf((float) item->spritesheet->width / 64);

    // se o spritesheet possuir mais que um sprite selecionar um aleatoriamente
    if ( spriteCounts > 1)
        item->spriteFrame = (Rectangle){ (float)(64 * GetRandomValue(0, spriteCounts-1)), 0, 64, item->spritesheet->height };
    else item->spriteFrame = (Rectangle){ 0, 0, 64,  item->spritesheet->height };

    // define posição e area de colisão centralizada
    setItemPosition(item, initPos);

    // seta o tipo de item e a vida inicial
    item->type = type;
    item->life = 300 ; // tempo em segundos (segundos = life*30)

    // retorna o novo item
    return item;
}


Texture2D* loadItemSpriteSheetArray(const char *spritepathList[]) {

    Texture2D* itemSpriteSheet = malloc(sizeof(Texture2D) * 4 );

    itemSpriteSheet[CHEESE] = LoadTexture(spritepathList[CHEESE]);
    itemSpriteSheet[STRAWBERRY] = LoadTexture(spritepathList[STRAWBERRY]);
    itemSpriteSheet[TRAP] = LoadTexture(spritepathList[TRAP]);
    itemSpriteSheet[END_ITEM] = (Texture2D){0};

    return itemSpriteSheet;
};

void unloadItemSpriteSheet(Texture2D *itemSpriteSheet) {
    UnloadTexture(itemSpriteSheet[CHEESE]);
    UnloadTexture(itemSpriteSheet[STRAWBERRY]);
    UnloadTexture(itemSpriteSheet[TRAP]);
    UnloadTexture(itemSpriteSheet[END_ITEM]);
    free(itemSpriteSheet);
}



void drawItem(Item* self){

    DrawTexturePro(
        *self->spritesheet,
        self->spriteFrame,
        (Rectangle){ self->position.x, self->position.y, self->spriteFrame.width, self->spriteFrame.height},
        (Vector2){(self->spriteFrame.width/2), (self->spriteFrame.height/2)},
        0.0f,
        WHITE
    );


    // DESENHOS DE DEPURAÇÃO
    if(debugMode){
        // ÁREA DE COLISÃO
        DrawRectangleLinesEx(self->collisionBox,2.0f,DARKGREEN);

    }
}