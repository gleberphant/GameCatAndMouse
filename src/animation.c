#include "animation.h"

#include <math.h>
#include <stdlib.h>

/**
 * @brief Obtém uma nova animação.
 * 
 * Esta função inicializa uma nova estrutura de animação com base no spritesheet fornecido.
 * 
 * @param spritesheet O spritesheet da animação.
 * @return Animation* Retorna um ponteiro para a animação inicializada.
 */
Animation* getAnimation(Texture2D* spritesheet){

    Animation* spriteA = malloc(sizeof(Animation));

    spriteA->spritesheet = spritesheet;
    spriteA->totalFrames = (int)ceilf((float)spriteA->spritesheet->width / ACTOR_DEFAULT_WIDTH);
    spriteA->length = ANIMATION_DEFAULT_TIME; // duração padra da animação
    spriteA->frameDelay = (int)ceilf((GAME_DEFAULT_FPS/(float)spriteA->totalFrames) * spriteA->length);
    spriteA->currentFrame = 0;
    spriteA->frameCount = 0;
    spriteA->repeat = 0;

    spriteA->type = 0;
    spriteA->frameRec = (Rectangle)
    {
        .x = 0, 
        .y = 0, 
        .width = ACTOR_DEFAULT_WIDTH, 
        .height = (float)spriteA->spritesheet->height
    };

    return spriteA;

}

/**
 * @brief Atualiza o quadro da animação.
 * 
 * Esta função atualiza o quadro atual da animação com base no tempo decorrido.
 * 
 * @param spriteA A animação a ser atualizada.
 * @return bool Retorna true se a animação terminou, false caso contrário.
 */
bool updateAnimationFrame(Animation *spriteA){

    spriteA->frameRec.x = spriteA->frameRec.width * (float)spriteA->currentFrame;

    spriteA->frameCount++;

    if(spriteA->frameCount > spriteA->frameDelay){
        spriteA->frameCount = 0;
        spriteA->currentFrame++;

        // repeating animation
        if( spriteA->currentFrame >= spriteA->totalFrames ){

            spriteA->repeat++;

            if (spriteA->type == 0 || spriteA->type == 1) {
                spriteA->currentFrame = 0;
            }
            else if (spriteA->repeat > 10) {
                spriteA->repeat = 0;;
                return true;
            }


        }
    }
    return false;
}