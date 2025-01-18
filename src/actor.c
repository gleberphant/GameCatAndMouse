#include "actor.h"
#include "main.h"


// inicia um ator
void setActor(Actor* target, Vector2 initPos, char* sprite){
    TraceLog(LOG_DEBUG, "-- Loading Object %s", sprite);
            
    target->spriteA2[MOVE]   = getAnimation( TextFormat("%s%s", sprite, "_walk.png") );
    
    target->spriteA2[STOP]   = getAnimation( TextFormat("%s%s", sprite, "_stop.png") );

    target->spriteA2[SPECIAL] = getAnimation( TextFormat("%s%s", sprite, "_special.png") );

    target->collisionBox = target->spriteA2[MOVE] ->frameRec;
    
    target->collisionBox.x = initPos.x;
    target->collisionBox.y = initPos.y;

    target->position = initPos;
    target->velocity = Vector2Zero();


    target->action = STOP;

    target->direction = 0.0f;
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
void drawActor(Actor* target){

    Rectangle drawRect = {
        target->collisionBox.x,
        target->collisionBox.y ,
        target->spriteA2[target->action]->frameRec.width,
        target->spriteA2[target->action]->frameRec.height
    };
    // centraliza

    //drawRect.x += (target->collisionBox.width)/2;
    //drawRect.y += (target->collisionBox.height)/2;

    if(target->direction == 90 || target->direction  == 270){
        target->collisionBox.width = drawRect.height-20;
        target->collisionBox.height = drawRect.width-20;
    }
    else{
        target->collisionBox.width  = drawRect.width-20;
        target->collisionBox.height = drawRect.height-20;
    }

    drawRect.x += (target->collisionBox.width)/2;
    drawRect.y += (target->collisionBox.height)/2;

    DrawTexturePro(
        target->spriteA2[target->action]->spritesheet,
        target->spriteA2[target->action]->frameRec,
        drawRect,
        (Vector2){ drawRect.width/2, drawRect.height/2 },
        target->direction, // angulo de rotação em graus
        WHITE
    );

    if(debugMode){
        // Código de depuração
        BeginBlendMode(BLEND_ADDITIVE);
        DrawRectangleRec(target->collisionBox,LIGHTGRAY);
        DrawLineV(
            target->position,
            Vector2Add(target->position, Vector2Scale(Vector2Normalize(target->velocity) , 30.0f)), WHITE);
        EndBlendMode();
    }

    updateAnimationFrame (target->spriteA2[target->action] );
    

}