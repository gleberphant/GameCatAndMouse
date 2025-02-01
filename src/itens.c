#include "itens.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"

/*
 * o spritesheet pode ter qualquer altura, desde que cada item seja separado por 64 pixels.
 * a altura do spritesheet é o raio da area de colisão.
 */

/**
 * @brief Obtém o retângulo de colisão de um item.
 * 
 * Esta função calcula o retângulo de colisão de um item com base na sua posição.
 * 
 * @param position A posição do item.
 * @return Rectangle Retorna o retângulo de colisão do item.
 */
Rectangle getItemCollisionBox(Vector2 position) {
    return (Rectangle){
        .width  = 40, //item->spriteFrame.width -32;
        .height = 40, //item->spriteFrame.height;
        .x = position.x - 20,
        .y = position.y - 20
    };

}

/**
 * @brief Obtém a posição de um item a partir do seu retângulo de colisão.
 * 
 * Esta função calcula a posição centralizada de um item com base no seu retângulo de colisão.
 * 
 * @param box O retângulo de colisão do item.
 * @return Vector2 Retorna a posição do item.
 */
Vector2 getItemPosition(Rectangle box) {
    return (Vector2){
        .x = box.x + (box.width/2),
        .y = box.y + (box.height/2)
    };
}

/**
 * @brief Define a posição de um item.
 * 
 * Esta função define a posição e o retângulo de colisão de um item.
 * 
 * @param item O item a ser posicionado.
 * @param position A nova posição do item.
 */
void setItemPosition(Item* item, Vector2 position) {

    item->collisionBox = (Rectangle){
        .x = position.x,
        .y = position.y,
        .width = 40,
        .height = 40
    };

    item->position = getItemPosition(item->collisionBox);

}


void itemGetHit(Actor *target, Item *item, Sound *sound) {
    switch (item->type) {
        case CHEESE:
            PlaySound(sound[SOUND_EAT_CHEESE]);
        item->life = -1;
        score += 5;
        break;

        case STRAWBERRY:
            PlaySound(sound[SOUND_EAT_STRAWBERRY]);

        item->life = -1;
        target->life = 100;

        break;

        case TRAP:
            PlaySound(sound[SOUND_GET_HURT]);
        item->life = -1;
        target->life -= 10;
        target->action = STUN;
        break;

        default:
            TraceLog(LOG_DEBUG, "item desconhecido");
        item->life = -1;
        break;
    }
}


ItemType getRandomItemType() {

    switch (GetRandomValue(0, 10))
    {
        case 0: case 1: case 2: case 3: case 4:
            return CHEESE;
        case 5: case 6: case 7: case 8:
            return TRAP;
        case 9: case 10:
            return STRAWBERRY;
        default:
            return CHEESE;
    }
    return CHEESE;
}

/**
 * @brief Carrega um novo item.
 * 
 * Esta função carrega um novo item com base na posição inicial, spritesheet e tipo fornecidos.
 * 
 * @param initPos A posição inicial do item.
 * @param spritesheetArray O array de spritesheets dos itens.
 * @param type O tipo do item.
 * @return Item* Retorna um ponteiro para o item carregado.
 */
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

/**
 * @brief Carrega um array de spritesheets de itens.
 * 
 * Esta função carrega um array de spritesheets de itens a partir de uma lista de caminhos de arquivos.
 * 
 * @param spritepathList A lista de caminhos de arquivos dos spritesheets.
 * @return Texture2D* Retorna um ponteiro para o array de spritesheets carregado.
 */
Texture2D* loadItemSpriteSheetArray(const char *spritepathList[]) {

    Texture2D* itemSpriteSheet = malloc(sizeof(Texture2D) * 4 );

    itemSpriteSheet[CHEESE] = LoadTexture(spritepathList[CHEESE]);
    itemSpriteSheet[STRAWBERRY] = LoadTexture(spritepathList[STRAWBERRY]);
    itemSpriteSheet[TRAP] = LoadTexture(spritepathList[TRAP]);
    itemSpriteSheet[END_ITEM] = (Texture2D){0};

    return itemSpriteSheet;
};

/**
 * @brief Descarrega um array de spritesheets de itens.
 * 
 * Esta função libera os recursos alocados para um array de spritesheets de itens.
 * 
 * @param itemSpriteSheet O array de spritesheets a ser descarregado.
 */
void unloadItemSpriteSheet(Texture2D *itemSpriteSheet) {
    UnloadTexture(itemSpriteSheet[CHEESE]);
    UnloadTexture(itemSpriteSheet[STRAWBERRY]);
    UnloadTexture(itemSpriteSheet[TRAP]);
    //UnloadTexture(itemSpriteSheet[END_ITEM]);
    free(itemSpriteSheet);
}

/**
 * @brief Desenha um item na tela.
 * 
 * Esta função renderiza um item na tela.
 * 
 * @param self O item a ser desenhado.
 */
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