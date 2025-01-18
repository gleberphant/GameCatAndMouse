#include "actor.h"
#include "main.h"

const char* getString(char* str1, char* str2){
    return TextFormat("%s%s", str1, str2);
}

// inicia um ator
void setObject(Actor* target, Vector2 initPos, char* sprite){
    TraceLog(LOG_DEBUG, "- Loading Object %s", sprite);
            
    target->spriteA2[MOVE]   = getAnimation( getString(sprite, "_walk.png") );
    
    target->spriteA2[STOP]   = getAnimation( getString(sprite, "_stop.png") );

    target->spriteA2[SPECIAL] = getAnimation( getString(sprite, "_special.png") );

    target->box = target->spriteA2[MOVE] ->frameRec;
    
    target->box.x = initPos.x;
    target->box.y = initPos.y;

    target->position = initPos;
    target->move = Vector2Zero();

    target->direction = UP;
    target->action = STOP;

    target->life = DEFAULT_LIFE;
}


/* 
Matriz de rotação 2D
    
    | x |   | cos(θ) -sin(θ) |
    | y | * | sin(θ)  cos(θ) | 

    x' = x * cos(θ) - y * sin(θ)
    y' = x * sin(θ) + y * cos(θ)

*/

// desenhar ator
void drawObject(Actor* target){ 

    if(target->direction == LEFT || target->direction  == RIGHT){
        target->box.width = target->spriteA2[target->action]->frameRec.height;
        target->box.height = target->spriteA2[target->action]->frameRec.width;
    }
    else{
        target->box.width  = target->spriteA2[target->action]->frameRec.width;
        target->box.height = target->spriteA2[target->action]->frameRec.height;
    }
    
    if(debugMode){
        // Código de depuração
        DrawRectangleRec(target->box,LIGHTGRAY);
    }

    DrawTexturePro(
        target->spriteA2[target->action]->spritesheet,
        target->spriteA2[target->action]->frameRec,
        (Rectangle){ target->box.x+(target->box.width/2), target->box.y+(target->box.height/2), target->box.width, target->box.height},
        (Vector2){ target->box.width/2, target->box.height/2 },
        target->direction*90, // angulo de rotação em graus
        WHITE
    );


    

    updateAnimationFrame (target->spriteA2[target->action] );
    

}