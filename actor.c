
#include "actor.h"

// inicia um ator
void setObject(Object* target, Vector2 initPos, char* sprite){
    TraceLog(LOG_INFO, "- Loading Object %s", sprite);

    //target->sprite = LoadTexture(sprite);

    target->spriteA = setAnimation(sprite);

    target->box = target->spriteA->frameRec;
    
    target->box.x = initPos.x;
    target->box.y = initPos.y;

    target->position = initPos;
    target->move = Vector2Zero();

    target->direction = UP;

    target->life = DEFAULT_LIFE;
}

Animation* setAnimation(char* spritesheet){

    Animation* spriteA;

    spriteA = malloc(sizeof(Animation));

    spriteA->spritesheet = LoadTexture(spritesheet);
    spriteA->totalFrames = spriteA->spritesheet.width / PLAYER_DEFAULT_WIDTH;
    spriteA->length = 1.0f; // 1 segundo de duração
    spriteA->frameDelay = 1;
    spriteA->currentFrame = 0;
    spriteA->frameCount = 0;
    spriteA->frameRec = (Rectangle)
    {
        .x = 0, 
        .y = 0, 
        .width = PLAYER_DEFAULT_WIDTH, 
        .height = spriteA->spritesheet.height 
    };

    return spriteA;

}


void updateAnimation(Animation *spriteA){

    spriteA->frameCount++;

    if(spriteA->frameCount >(int) spriteA->frameDelay){
        spriteA->frameCount = 0;
        spriteA->currentFrame++;

        // repeating animation
        if( spriteA->currentFrame >= spriteA->totalFrames ){
            spriteA->currentFrame = 0;
        }
    }

    spriteA->frameRec.x = spriteA->frameRec.width * spriteA->currentFrame;

}


// desenhar ator
void drawObject(Object* target){ 

    Animation* spriteA = target->spriteA;

    DrawTexturePro(
        spriteA->spritesheet,
        spriteA->frameRec,
        (Rectangle){ target->box.x+(target->box.width/2), target->box.y+(target->box.height/2), target->box.width, target->box.height},
        (Vector2){ target->box.width/2, target->box.height/2 },
        target->direction,
        WHITE
    );


}