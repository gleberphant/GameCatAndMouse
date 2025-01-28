#include "itens.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

/*
 * o spritesheet pode ter qualquer altura, desde que cada item seja separado por 64 pixels.
 * a altura do spritesheet é o raio da area de colisão.
 */

Item* getItem(Vector2 initPos, Texture2D* spritesheet, ItemType type) {
    Item* item = malloc(sizeof(Item));

    // define o spritesheet
    item->spritesheet = spritesheet;

    // calcula total de sprites no spritesheet
    int spriteCounts = (int)ceilf((float) item->spritesheet->width / 64);

    // se o spritesheet possuir mais que um sprite selecionar um aleatoriamente
    if ( spriteCounts > 1)
        item->spriteFrame = (Rectangle){ (float)(64 * GetRandomValue(0, spriteCounts-1)), 0, 64, spritesheet->height };
    else item->spriteFrame = (Rectangle){ 0, 0, 64,  spritesheet->height };

    //item->spriteFrame = (Rectangle){ 0, 0, 64,  spritesheet->height };
    
    // posição do item
    item->position = initPos;
    
    // define a area de colisão centralizada

    item->collisionBox.width  = 40;//item->spriteFrame.width -32;
    item->collisionBox.height = 40;//item->spriteFrame.height;
    item->collisionBox.x = item->position.x - (item->collisionBox.width/2);
    item->collisionBox.y = item->position.y - (item->collisionBox.height/2);




    // seta o tipo de item e a vida inicial
    item->type = type;
    item->life = 10; // tempo em segundos (segundos = life/30)

    // retorna o novo item
    return item;
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