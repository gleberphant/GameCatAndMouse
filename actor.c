#include "actor.h"


const char* getString(char* str1, char* str2){
    return TextFormat("%s%s", str1, str2);
}

// inicia um ator
void setObject(Object* target, Vector2 initPos, char* sprite){
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

// desenhar ator
void drawObject(Object* target){ 

    target->box.width  = target->spriteA2[target->action]->frameRec.width;
    target->box.height = target->spriteA2[target->action]->frameRec.height;

    // DrawRectangleRec(target->box,LIGHTGRAY);

    DrawTexturePro(
        target->spriteA2[target->action]->spritesheet,
        target->spriteA2[target->action]->frameRec,
        (Rectangle){ target->box.x+(target->box.width/2), target->box.y+(target->box.height/2), target->box.width, target->box.height},
        (Vector2){ target->box.width/2, target->box.height/2 },
        target->direction*90,
        WHITE
    );


    

    updateAnimationFrame (target->spriteA2[target->action] );
    

}