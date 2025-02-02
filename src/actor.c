#include "actor.h"
#include "maps.h"
#include "main.h"


// inicia um ator

/**
 * @brief Carrega um novo ator.
 * 
 * Esta função inicializa um novo ator com base na posição inicial e no spritesheet fornecidos.
 * 
 * @param initPos A posição inicial do ator.
 * @param spritesheet O spritesheet do ator.
 * @return Actor* Retorna um ponteiro para o ator inicializado.
 */
Actor* loadNewActor(Vector2 initPos, Texture2D* spritesheet) {
    TraceLog(LOG_DEBUG, "== Iniciando novo ACTOR");
    Actor* newActor = malloc(sizeof(Actor));
    setActor(newActor, initPos, spritesheet);
    return newActor;
}

/**
 * @brief Carrega um array de spritesheets de ator.
 * 
 * Esta função carrega um array de spritesheets de ator a partir de um caminho base.
 * 
 * @param sprite O caminho base para os spritesheets.
 * @return Texture2D* Retorna um ponteiro para o array de spritesheets carregado.
 */
Texture2D* loadActorSpriteSheetArray( const char* sprite){

    Texture2D* spriteSheetArray = malloc( sizeof(Texture2D) * 4 );

    spriteSheetArray[STOP]    = LoadTexture( TextFormat("%s%s", sprite, "_stop.png"));
    spriteSheetArray[MOVE]    = LoadTexture( TextFormat("%s%s", sprite, "_walk.png"));
    spriteSheetArray[STUN] = LoadTexture( TextFormat("%s%s", sprite, "_special.png"));
    spriteSheetArray[END]     = (Texture2D)  { 0 };

    return spriteSheetArray;

}

/**
 * @brief Descarrega um array de spritesheets de ator.
 * 
 * Esta função libera os recursos alocados para um array de spritesheets de ator.
 * 
 * @param actorSpriteSheetArray O array de spritesheets a ser descarregado.
 */
void unloadActorSpriteSheet(Texture2D *actorSpriteSheetArray) {
    UnloadTexture(actorSpriteSheetArray[STOP]);
    UnloadTexture(actorSpriteSheetArray[MOVE]);
    UnloadTexture(actorSpriteSheetArray[STUN]);
    
    free(actorSpriteSheetArray);
}

/**
 * @brief Define as propriedades de um ator.
 * 
 * Esta função define as propriedades de um ator, incluindo a posição e o spritesheet.
 * 
 * @param self O ator a ser definido.
 * @param initPos A posição inicial do ator.
 * @param spritesheet O spritesheet do ator.
 */
void setActor(Actor* self, Vector2 initPos, Texture2D* spritesheet){

    // carrega os spritesheet com animação de cada ação
    self->spriteA2[STOP]    = getAnimation( &spritesheet[STOP] );
    self->spriteA2[STOP]->type = STOP;
    self->spriteA2[MOVE]    = getAnimation( &spritesheet[MOVE] );
    self->spriteA2[MOVE]->type = MOVE;
    self->spriteA2[STUN] = getAnimation( &spritesheet[STUN] );
    self->spriteA2[STUN]->type = STUN;

    // define posição e colission box. passa posição centralizada no tile
    setActorInitPosition(self, (Vector2){ initPos.x + (float)TILE_SIZE/2, initPos.y + (float)TILE_SIZE/2 });

    // define valores default dos atributos

    self->action = STOP;
    self->speed = 5.0f;
    self->direction = 0.0f;
    self->life = DEFAULT_LIFE;
    self->behavior = GetRandomValue(ATTACK, CRAZY);
}

/**
 * @brief Define a posição de um ator.
 * 
 * Esta função define a posição e o retângulo de colisão de um ator.
 * 
 * @param self O ator a ser posicionado.
 * @param position A nova posição do ator. posição centralizada aos tiles.
 */
void setActorInitPosition(Actor* self, Vector2 position) {
    // recebe posição centralizada do sprite
    self->position = position;

    // retângulo de colisão
    self->hitBox = getActorHitBox(self, self->behavior != PLAYER ? true : false);

    // old pos
    self->oldPosition = self->position;
}

/**
 * @brief Obtém o retângulo de colisão de um ator a partir de uma posição.
 * 
 * Esta função calcula o retângulo de colisão de um ator com base na sua posição.
 * 
 * @param position A posição do ator.
 * @return Rectangle Retorna o retângulo de colisão do ator.
 */
Rectangle getActorHitBox(Actor* target, bool cat) {


    return (Rectangle)  {
        .width = 52,
        .height = 52,
        .x = cat && target->action == MOVE ? target->position.x - 26 + ( 20 * cosf(target->direction)) : target->position.x - 26,
        .y = cat && target->action == MOVE ? target->position.y - 26 + ( 20 * sinf(target->direction)) : target->position.y - 26,
    };

}

/**
 * @brief Obtém a posição de um ator a partir do seu retângulo de colisão.
 * 
 * Esta função calcula a posição centralizada de um ator com base no seu retângulo de colisão.
 * 
 * @param box O retângulo de colisão do ator.
 * @return Vector2 Retorna a posição do ator.
 */
Vector2 getActorPosition(Rectangle box) {
    return (Vector2){
        .x = box.x + box.width / 2,
        .y = box.y + box.height / 2,
        };
}

//------

/**
 * @brief Ação de parar do ator.
 * 
 * Esta função define a ação do ator como parar.
 * 
 * @param self O ator a ser atualizado.
 */
void actionStop(Actor* self)  {
    self->hitBox = getActorHitBox(self, self->behavior != PLAYER ? true : false);
    return;
}

/**
 * @brief Ação de mover do ator.
 * 
 * Esta função atualiza a posição do ator com base na sua velocidade e direção.
 * 
 * @param self O ator a ser atualizado.
 */
void actionMove(Actor* self) {

    self->action = MOVE;
    self->oldPosition = self->position;

    // calcula nova posição
    self->position.x += self->speed * cosf(self->direction);
    self->position.y += self->speed * sinf(self->direction);


    // VERIFICA SE colisão com as parades do mapa
    if(checkMapCollision(self)){
        self->position = self->oldPosition; //nova posição
    }

    // atualizar posição do hit de acordo com nova posição para o canto superior da imagem
    self->hitBox = getActorHitBox(self, self->behavior != PLAYER ? true : false);


}

/**
 * @brief Ação especial do ator.
 * 
 * Esta função define a ação do ator como especial.
 * 
 * @param self O ator a ser atualizado.
 * @param target O alvo da ação especial.
 */
void actionSpecial(Actor* self, Actor* target)  {
    self->action = STUN;

}

/**
 * @brief ator sofreu um ataque
 *
 * Esta função causa dano no ator
 *
 * @param self O ator atacado.
 */
void getHurt(Actor* self)  {
    self->life -= 8;

}

/**
 * @brief Desenha um ator na tela.
 * 
 * Esta função renderiza um ator na tela.
 * 
 * @param self O ator a ser desenhado.
 */
void drawActor(Actor* self){

    // define area de desenho. com posição centralizada 
    Rectangle drawRect = {
        .width = self->spriteA2[self->action]->frameRec.width,
        .height = self->spriteA2[self->action]->frameRec.height,
        .x = self->position.x,
        .y = self->position.y
    };




    //DESENHA SPRITE
    DrawTexturePro(
        *self->spriteA2[self->action]->spritesheet,
        self->spriteA2[self->action]->frameRec,
        drawRect,
        (Vector2){ drawRect.width/2, drawRect.height/2 },
        (self->direction*RAD2DEG) + 90, // angulo de rotação em graus
        self-> life < 20 ? (Color){255,100,100,255} :  WHITE // SE LIFE BAIXO DESENHA VERMELHO
    );

    // DESENHA EFEITO DE PODER
    if(self->life == 100) {
        DrawCircleGradient(
            (int)self->position.x, (int)self->position.y,
            32.0f,
            Fade(WHITE, 0.0f), Fade(SKYBLUE, 0.6f));
    }

    if(self->collision)
    {
        DrawCircle(
        (int)self->position.x,
        (int)self->position.y,
        10,
        RED);
        self->collision = false;
    }

    // atualiza frame da animação
    if ( updateAnimationFrame (self->spriteA2[self->action] )) self->action = STOP; ;


    // DESENHOS DE DEPURAÇÃO
    if(debugMode){

        // ÁREA DE COLISÃO
        DrawRectangleLinesEx(self->hitBox,5.0f,DARKGREEN);
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

        // posição no maptile
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