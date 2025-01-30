#include "actor.h"
#include "maps.h"
#include "main.h"


// inicia um ator

Actor* loadNewActor(Vector2 initPos, Texture2D* spritesheet) {
    TraceLog(LOG_DEBUG, "== Iniciando novo ACTOR");
    Actor* newActor = malloc(sizeof(Actor));
    setActor(newActor, initPos, spritesheet);
    return newActor;
}

Texture2D* loadActorSpriteSheetArray( const char* sprite){

    Texture2D* spriteSheetArray = malloc( sizeof(Texture2D) * 4 );

    spriteSheetArray[STOP]    = LoadTexture( TextFormat("%s%s", sprite, "_stop.png"));
    spriteSheetArray[MOVE]    = LoadTexture( TextFormat("%s%s", sprite, "_walk.png"));
    spriteSheetArray[SPECIAL] = LoadTexture( TextFormat("%s%s", sprite, "_special.png"));
    spriteSheetArray[END]     = (Texture2D)  { 0 };

    return spriteSheetArray;

}

void unloadActorSpriteSheet(Texture2D *actorSpriteSheetArray) {
    UnloadTexture(actorSpriteSheetArray[STOP]);
    UnloadTexture(actorSpriteSheetArray[MOVE]);
    UnloadTexture(actorSpriteSheetArray[SPECIAL]);
    free(actorSpriteSheetArray);
}


void setActor(Actor* self, Vector2 initPos, Texture2D* spritesheet){

    // carrega os spritesheet com animação de cada ação
    self->spriteA2[STOP]    = getAnimation( &spritesheet[STOP] );
    self->spriteA2[STOP]->type = STOP;
    self->spriteA2[MOVE]    = getAnimation( &spritesheet[MOVE] );
    self->spriteA2[MOVE]->type = MOVE;
    self->spriteA2[SPECIAL] = getAnimation( &spritesheet[SPECIAL] );
    self->spriteA2[SPECIAL]->type = SPECIAL;

    // define posição e colission box
    setActorPosition(self, initPos);

    // define valores default dos atributos

    self->action = STOP;
    self->speed = 5.0f;
    self->direction = 0.0f;
    self->life = DEFAULT_LIFE;
    self->behavior = GetRandomValue(ATTACK, CRAZY);
}

void setActorPosition(Actor* self, Vector2 position) {
    // retângulo de colisão
    self->collisionBox = (Rectangle){
        .x = position.x,
        .y = position.y,
        .width  = 64, //self->spriteA2[STOP]->frameRec.width;
        .height = 64 //self->spriteA2[STOP]->frameRec.height;
    };

    // centraliza posição no sprite
    self->position = (Vector2){
       .x = position.x + (self->collisionBox.width/2),
       .y = position.y + (self->collisionBox.height/2)
    };

    // old pos
    self->oldPosition = self->position;
}

// obter o retângulo de colisão a partir de uma posição
Rectangle getActorCollisionBox(Vector2 position) {
    return (Rectangle)  {
        .width = 64,
        .height = 64,
        .x = position.x - 32,
        .y = position.y - 32,
    };

}

// obtêm a posição centralizada a partir de um retangulo
Vector2 getActorPosition(Rectangle box) {
    return (Vector2){
        .x = box.x + box.width / 2,
        .y = box.y + box.height / 2,
        };
}

//------
void actionStop(Actor* self)  {
    self->action = STOP;
    return;
}

void actionMove(Actor* self) {

    self->action = MOVE;
    self->oldPosition = self->position;

    // calcula nova posição
    self->position = Vector2Add(
        self->position,
        (Vector2){
            .x = self->speed * cosf(self->direction),
            .y = self->speed * sinf(self->direction)
        }
    );

    // VERIFICA SE JOGADOR DENTRO DA ARENA
    if(checkMapCollision(self)){
        self->position = self->oldPosition; //nova posição
    }

}

 void actionSpecial(Actor* self, Actor* target)  {
    self->action = SPECIAL;

}


// desenhar ator
void drawActor(Actor* self){

    // atualiza altura e largura do box de colisão
    Rectangle drawRect = {
        .width = self->spriteA2[self->action]->frameRec.width,
        .height = self->spriteA2[self->action]->frameRec.height
    };

    // Vector2 size = Vector2Rotate((Vector2){drawRect.width-32, drawRect.height-3}, self->direction - (90 * DEG2RAD));
    //self->collisionBox.width     = fmax( abs(size.x), 32);
    //self->collisionBox.height    =  fmax( abs(size.y), 32);


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