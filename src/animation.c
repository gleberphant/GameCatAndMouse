#include "animation.h"



Animation* getAnimation(Texture2D* spritesheet){

    Animation* spriteA = malloc(sizeof(Animation));

    spriteA->spritesheet = spritesheet;
    spriteA->totalFrames = (int)ceilf((float)spriteA->spritesheet->width / ACTOR_DEFAULT_WIDTH);
    spriteA->length = ANIMATION_DEFAULT_TIME; // duração padra da animação
    spriteA->frameDelay = (int)ceilf((GAME_DEFAULT_FPS/(float)spriteA->totalFrames) * spriteA->length);
    spriteA->currentFrame = 0;
    spriteA->frameCount = 0;
    spriteA->frameRec = (Rectangle)
    {
        .x = 0, 
        .y = 0, 
        .width = ACTOR_DEFAULT_WIDTH, 
        .height = (float)spriteA->spritesheet->height
    };

    return spriteA;

}


void updateAnimationFrame(Animation *spriteA){

    spriteA->frameCount++;

    if(spriteA->frameCount >(int) spriteA->frameDelay){
        spriteA->frameCount = 0;
        spriteA->currentFrame++;

        // repeating animation
        if( spriteA->currentFrame >= spriteA->totalFrames ){
            spriteA->currentFrame = 0;
        }
    }

    spriteA->frameRec.x = spriteA->frameRec.width * (float)spriteA->currentFrame;

}