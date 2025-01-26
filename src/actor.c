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
    self->oldPosition = self->position;
    self->action = STOP;
    self->speed = 4.0f;
    self->direction = 0.0f;
    self->life = DEFAULT_LIFE;
}


void actionStop(Actor* self)  {
    return;
}

void actionMove(Actor* self, Rectangle* arena) {

    //Vector2 movement = Vector2Rotate((Vector2){0.0f, -self->speed}, self->direction*DEG2RAD);

    Vector2 movement = (Vector2)
    {
        self->speed*cosf((self->direction)), 
        self->speed*sinf((self->direction))
        }; 


    self->action = MOVE;
    self->oldPosition = self->position;
    self->position = Vector2Add(self->position, movement); //nova posição

    // VERIFICA SE JOGADOR DENTRO DA ARENA
    if(!isInside(self, arena)){
        self->position = self->oldPosition; //nova posição
    }

}

 void actionSpecial(Actor* self, Actor* target)  {
    return;
 }


// desenhar ator
void drawActor(Actor* self){

    // atualiza altura e largura do box de colisão
    Rectangle drawRect = {
        .width = self->spriteA2[self->action]->frameRec.width,
        .height = self->spriteA2[self->action]->frameRec.height
    };

    if(self->direction*RAD2DEG == 0  || self->direction*RAD2DEG == 180 ){
        self->collisionBox.width = drawRect.height - 20;
        self->collisionBox.height = drawRect.width - 20;
    }
    else{
        self->collisionBox.width  = drawRect.width  - 20;
        self->collisionBox.height = drawRect.height - 20;
    }

    drawRect.x = self->position.x;
    drawRect.y = self->position.y;

    // atualizar posição do collision box
    self->collisionBox.x = self->position.x-(self->collisionBox.width/2); //newPosX
    self->collisionBox.y = self->position.y-(self->collisionBox.height/2); //newPosY

    //DESENHA SPRITE
    DrawTexturePro(
        *self->spriteA2[self->action]->spritesheet,
        self->spriteA2[self->action]->frameRec,
        drawRect,
        (Vector2){ drawRect.width/2, drawRect.height/2 },
        (self->direction*RAD2DEG)+90, // angulo de rotação em graus
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
        DrawRectangleLinesEx(self->collisionBox,5.0f,DARKGREEN);
        DrawRectangleLinesEx(drawRect,2.0f,RED);

        // DIREÇÃO DO MOVIMENTO
        DrawLineEx(
         self->position,
          self->oldPosition,
          2.0f,
          RED);

        // mouse
        DrawLineV(
            self->position,
             GetMousePosition(), RED);

        DrawCircle(
            (int)self->position.x,
            (int)self->position.y,
            5,
            BLUE);
    }


}

/*
Matriz de rotação 2D

    | x |   | cos(θ) -sin(θ) |
    | y | * | sin(θ)  cos(θ) |

    x' = x * cos(θ) - y * sin(θ)
    y' = x * sin(θ) + y * cos(θ)

*/