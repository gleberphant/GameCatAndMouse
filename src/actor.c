#include "actor.h"
#include "maps.h"
#include "main.h"

// inicia um ator
void setActor(Actor* self, Vector2 initPos, Texture2D* spritesheet){
    TraceLog(LOG_DEBUG, "-- Loading Object");

    // carrega os spritesheet com animação de cada ação
    self->spriteA2[STOP]    = getAnimation( &spritesheet[STOP] );
    self->spriteA2[STOP]->type = STOP;
    self->spriteA2[MOVE]    = getAnimation( &spritesheet[MOVE] );
    self->spriteA2[MOVE]->type = MOVE;
    self->spriteA2[SPECIAL] = getAnimation( &spritesheet[SPECIAL] );
    self->spriteA2[SPECIAL]->type = SPECIAL;

    // retângulo de colisão
    self->collisionBox.x = initPos.x;
    self->collisionBox.y = initPos.y;
    self->collisionBox.width  = self->spriteA2[STOP]->frameRec.width;
    self->collisionBox.height = self->spriteA2[STOP]->frameRec.height;

    // centraliza posição no sprite
    self->position = initPos;

    // centraliza posição no sprite
    self->position.x += (self->collisionBox.width/2);
    self->position.y += (self->collisionBox.height/2);

    // define valores default dos atributos
    self->velocity = (Vector2){ 0.0f, 0.0f };
    self->action = STOP;
    self->direction = 0.0f;
    self->life = DEFAULT_LIFE;
}


void actionStop(Actor* self)  {

}

void actionMove(Actor* self, Rectangle* arena) {

    self->velocity = Vector2Rotate((Vector2){0.0f,-10.0f}, self->direction*DEG2RAD);
    self->position = Vector2Add(self->position, self->velocity); //nova posição

    // VERIFICA SE JOGADOR DENTRO DA ARENA
    if(isInside(self, arena)){

        self->collisionBox.x = self->position.x-(self->collisionBox.width/2); //newPosX
        self->collisionBox.y = self->position.y-(self->collisionBox.height/2); //newPosY

    }else{
        self->position = Vector2Subtract(self->position, self->velocity); //nova posição
    }
}

 void actionSpecial(Actor* self, Actor* target)  {

 }


// desenhar ator
void drawActor(Actor* self){

    Rectangle drawRect = {
        self->collisionBox.x,
        self->collisionBox.y ,
        self->spriteA2[self->action]->frameRec.width,
        self->spriteA2[self->action]->frameRec.height
    };

    if(self->direction == 90 || self->direction  == 270){
        self->collisionBox.width = drawRect.height-20;
        self->collisionBox.height = drawRect.width-20;
    }
    else{
        self->collisionBox.width  = drawRect.width-20;
        self->collisionBox.height = drawRect.height-20;
    }

    drawRect.x += (self->collisionBox.width)/2;
    drawRect.y += (self->collisionBox.height)/2;



    //DESENHA SPRITE
    DrawTexturePro(
        *self->spriteA2[self->action]->spritesheet,
        self->spriteA2[self->action]->frameRec,
        drawRect,
        (Vector2){ drawRect.width/2, drawRect.height/2 },
        self->direction, // angulo de rotação em graus
        WHITE
    );

    // DESENHA "SANGUE" - ponto de colisão
    if (self->collision == true)
        DrawCircle(
            (int)self->pointOfCollision.x,
            (int)self->pointOfCollision.y,
            10,
            RED);
    self->collision = false;

    if ( updateAnimationFrame (self->spriteA2[self->action] )) self->action = STOP; ;


    // DESENHOS DE DEPURAÇÃO
    if(debugMode){
        // ÁREA DE COLISÃO
        DrawRectangleLinesEx(self->collisionBox,2.0f,DARKGREEN);

        // DIREÇÃO DO MOVIMENTO
        DrawLineEx(
         self->position,
          Vector2Add(self->position, Vector2Scale(self->velocity, 5.0f)),
          2.0f,
          BLUE);

        // mouse
        DrawLineV(
            self->position,
             GetMousePosition(), RED);
    }


}

/*
Matriz de rotação 2D

    | x |   | cos(θ) -sin(θ) |
    | y | * | sin(θ)  cos(θ) |

    x' = x * cos(θ) - y * sin(θ)
    y' = x * sin(θ) + y * cos(θ)

*/