#include "itens.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

/*
 * o spritesheet pode ter qualquer altura, desde que cada item seja separado por 64 pixels.
 * a altura do spritesheet é o raio da area de colisão.
 */

Item* getItem(Vector2 initPos, Texture2D *spritesheetArray, ItemType type) {
    Item* item = malloc(sizeof(Item));

    // define o spritesheet
    item->spritesheet = &spritesheetArray[type];

    // calcula total de sprites no spritesheet
    int spriteCounts = (int)ceilf((float) item->spritesheet->width / 64);

    // se o spritesheet possuir mais que um sprite selecionar um aleatoriamente
    if ( spriteCounts > 1)
        item->spriteFrame = (Rectangle){ (float)(64 * GetRandomValue(0, spriteCounts-1)), 0, 64, item->spritesheet->height };
    else item->spriteFrame = (Rectangle){ 0, 0, 64,  item->spritesheet->height };

    // posição do item
    item->position = initPos;
    
    // define a area de colisão centralizada
    item->collisionBox = getCollisionBox(item->position);


    // seta o tipo de item e a vida inicial
    item->type = type;
    item->life = 300 ; // tempo em segundos (segundos = life*30)

    // retorna o novo item
    return item;
}


Texture2D* getItemSpriteSheetArray(const char *spritepathList[]) {

    Texture2D* itemSpriteSheet = malloc(sizeof(Texture2D) * 4);

    itemSpriteSheet[CHEESE] = LoadTexture(spritepathList[CHEESE]);
    itemSpriteSheet[STRAWBERRY] = LoadTexture(spritepathList[STRAWBERRY]);
    itemSpriteSheet[TRAP] = LoadTexture(spritepathList[TRAP]);
    itemSpriteSheet[END_ITEM] = (Texture2D){0};

    return itemSpriteSheet;
};

Rectangle getCollisionBox(const Vector2 pos) {
    Rectangle temp;
    temp.width  = 40;//item->spriteFrame.width -32;
    temp.height = 40;//item->spriteFrame.height;
    temp.x = pos.x - (temp.width/2);
    temp.y = pos.y - (temp.height/2);

    return temp;
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