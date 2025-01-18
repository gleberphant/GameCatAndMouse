#ifndef ANIMATION_H_

#define ANIMATION_H_

#include "raylib.h"
#include "raymath.h"


#include <math.h>
#include <stdlib.h>


#define ACTOR_DEFAULT_WIDTH 64
#define ENEMY_DEFAULT_WIDTH 128
#define ANIMATION_DEFAULT_TIME 0.5f
#define GAME_DEFAULT_FPS 30.0f

// ANIMAÇÃO
typedef struct Animation{
    int repeat;
    int totalFrames;
    int currentFrame;
    int frameCount;
    int frameDelay; // quantidade de frames para atualizar um frame da animação
    int type;
    float length; // duração da animação em segundos
   
    Rectangle frameRec;
    Texture2D* spritesheet;

} Animation;

Animation* getAnimation(Texture2D* spritesheet);

bool updateAnimationFrame(Animation *self);


#endif