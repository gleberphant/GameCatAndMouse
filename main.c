/* TODO

- animar personagens
- mostrar score
- ELABORAR FUNÇÃO PARA ADICIONAR E REMOVER ITENS DA LISTA
- carregar mapa de arquivo
*/


#include "include/raylib.h"
#include "include/raymath.h"
#include "actor.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define MAP_BORDER 10
#define FONT_SIZE 32
#define FONT_SPACE 2.0f
#define DEFAULT_LIFE 10

// DECLARAÇÃO DE STRUCT E ENUM

typedef struct LinkedNode LinkedNode;
 
// ESTADOS QUE O JOGO PODE SE ENCONTRAR
typedef enum  { 
    INTRO, 
    MENU, 
    GAME, 
    OVER,
    EXIT 
} GameStatusaction;

// LINKED LIST DE OBJETOS
struct LinkedNode{
    Object* obj;
    LinkedNode* next;
};

// DEFINE UMA AÇÃO FEITA POR UM OBJETO 


// estrutura do mapa 
struct Map{
    Rectangle area;
    Color color;
    LinkedNode *enemyList;
    LinkedNode *itensList;
};

// VARIAVEIS GLOBAIS
LinkedNode *enemyListHead, *itemListHead;
Object *player, *enemy, *currentObj;
Rectangle arena;
GameStatusaction gameStage;
Font gameFont; 

float volume = 0.1f;
int score = 0;
bool death = false;
char text[100];


// MAPS
Vector2 mapEnemys[] = {
    (Vector2){20.0f, 200.0f},
    (Vector2){100.0f, 200.0f},
    (Vector2){320.0f, 100.0f}
};

Vector2 mapItens[] = {
    (Vector2){100.0f, 120.0f},
    (Vector2){300.0f, 80.0f},
    (Vector2){600.0f, 500.0f}
};

/* FUNÇÕES */

/* INICIADOR DE UM OBJETO */
void setObject(Object* target, Vector2 initPos, char* sprite){
    TraceLog(LOG_DEBUG, "- Loading Object %s", sprite);
    
    target->sprite = LoadTexture(sprite);
    target->box.width  = target->sprite.width;
    target->box.height = target->sprite.height;
    target->box.x = initPos.x;
    target->box.y = initPos.y;
    target->life = DEFAULT_LIFE;

    target->position = initPos;
    target->moviment = Vector2Zero();
    target->size.x = target->sprite.width;
    target->size.y = target->sprite.height;
    
}

/* 
inicia uma lista de OBJETOS - inimigos ou itens
recebe a posição inicial de cada objeto e o sprite dos objetos
*/ 
LinkedNode* initLinkedList( Vector2 initPosition[], char* sprite, short maxNodes)
{
    LinkedNode *newListNode, *headListNode = NULL;
  
    for (int i = 0 ; i < maxNodes; i++){

        newListNode = malloc(sizeof(LinkedNode));
        newListNode->obj = malloc(sizeof(Object));
        setObject(newListNode->obj, initPosition[i], sprite);
        newListNode->next = headListNode;
        headListNode = newListNode;
        
    }
    return headListNode;
}


void drawObject(Object* target){
 
    DrawTextureEx(
        target->sprite, 
        (Vector2){ target->box.x, target->box.y }, 
        0.0f, 
        1.0f, 
        WHITE
    );
}

void drawNodeList(LinkedNode* targetList){
    for(LinkedNode* currenteNode = targetList; currenteNode != NULL ; currenteNode = currenteNode->next)
    {
        if (currenteNode->obj->life < 1){
                continue;
        }

        drawObject(currenteNode->obj);
    }

       
}

// verifica se um objeto esta dentro de outro
bool isInside(Object* target, Rectangle *arena){
/* TODO passar um objeto RECTANGLE ao inves de um objeto inteiro*/
    Rectangle  targetBox ={
        target->position.x,
        target->position.y,
        target->size.x,
        target->size.y
    };

    // return CheckCollisionRecs(targetBox, *arena);
    if( targetBox.x < arena->x || targetBox.x > arena->width+arena->x-targetBox.width){
        return false;
    }
    if( targetBox.y < arena->y || targetBox.y > arena->height+arena->y-targetBox.height){
        return false;
    }

    return true;
    

}


int gameIntro(){

    // CARREGAR MUSICA 
    TraceLog(LOG_DEBUG, " -- CARREGANDO MUSICA ");
    Music introMusic = LoadMusicStream("sounds/intro_music.mp3");
    SetMusicVolume(introMusic, volume);
    PlayMusicStream(introMusic);

    // CARREGAR IMAGENS DE FUNDO
    TraceLog(LOG_DEBUG, " -- CARREGANDO IMAGENS");
    Texture2D backgroundTex = LoadTexture("resources/intro_bg.png");

    // SETAR TEXTOS
    TraceLog(LOG_DEBUG, " -- CARREGANDO FONTES ");   
    char* textIntro = "[ENTER] para jogar [ESC] para sair.";            
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textIntro, FONT_SIZE, FONT_SPACE);
    Rectangle textRec = { (W_WIDTH - textMeasure.x)/2, 400, textMeasure.x, textMeasure.y };
  
    // INTRO LOOP
    TraceLog(LOG_DEBUG, " -- CARREGAMENTO CONCLUIDO -- ");   
    while (gameStage == INTRO){
        // INPUT HANDLE
        if(IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()){
            gameStage = EXIT;
        }

        if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
            gameStage = GAME;
        }

        if(IsKeyReleased(KEY_KP_ADD)){
            volume+=0.05f;
            SetMusicVolume(introMusic, volume);
        }

        if(IsKeyReleased(KEY_KP_SUBTRACT)){
            volume-=0.05f;
            SetMusicVolume(introMusic, volume);
        }

        //UPDATE
        UpdateMusicStream(introMusic);

        // DRAW
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(backgroundTex, 0.0f, 0.0f, WHITE);
            DrawRectangleRec(textRec, BLACK);
            DrawTextEx(gameFont, textIntro, (Vector2){textRec.x, textRec.y}, FONT_SIZE, FONT_SPACE,  WHITE);
        EndDrawing();
    }

    // DESCARREGAR MUSICA 
    StopMusicStream(introMusic);
    UnloadMusicStream(introMusic);
    

    // DESCARREGAR IMAGENS
    UnloadTexture(backgroundTex);

     return 0;
}


int gameLoop(){

    // SETAR VARIAVEIS DO LOOP
    TraceLog(LOG_DEBUG, "- carregando GLOBAL VARIABLES");
    srand(time(NULL));
    Rectangle colisionRec;
    bool colision;
    volume = 1;
    unsigned short enemyVel = 4;
    
    
    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "-- carregando ENEMY LIST");
    enemyListHead = initLinkedList(
        mapEnemys,
        "resources/cat.png", 
        (unsigned short )sizeof(mapEnemys)/sizeof(Vector2)
    );

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        gameStage = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "--- carregando ITENS LIST");
    itemListHead = initLinkedList(
        mapItens,
        "resources/item.png", 
        (unsigned short)sizeof(mapItens)/sizeof(Vector2)
    );

    if(itemListHead == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        gameStage = EXIT;
    }

    // CARREGAR PERSONAGEM JOGADOR
    TraceLog(LOG_DEBUG, "--- carregando PLAYER");
    player = malloc(sizeof(Object));

    if(player == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar PLAYER");
        gameStage = EXIT;
    }

    setObject(player, (Vector2){400.0f, 400.0f}, "resources/mouse.png" );
    player->life = 200;

    // CARREGAR O MAPA
    TraceLog(LOG_DEBUG, "--- carregando MAP");
    arena.x = 0+MAP_BORDER;
    arena.y = 0+MAP_BORDER;
    arena.width = W_WIDTH-(MAP_BORDER*2); 
    arena.height = W_HEIGHT-(MAP_BORDER*2);
    
    
    // CARRGAR EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "--- carregando EFEITOS SONOROS");
    Sound eatCheese = LoadSound("sounds/eat_cheese.mp3");
    Sound getHited = LoadSound("sounds/get_hit.mp3");

    
    // GAME LOOP
    while (gameStage == GAME) {
        
        // INPUT HANDLE
        if(WindowShouldClose() == true)
        {
            gameStage = EXIT;
        }
        
        player->action = STOP;
        player->moviment = Vector2Zero();
        
        if(IsKeyDown(KEY_UP)){
            player->action = MOVE;
            player->moviment.y -= 10;
        }
        if(IsKeyDown(KEY_DOWN)){
            player->action = MOVE;
            player->moviment.y += 10;
        }
        if(IsKeyDown(KEY_LEFT)){
            player->action = MOVE;
            player->moviment.x -= 10;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player->action = MOVE;
            player->moviment.x += 10;
        }
        
        if(IsKeyDown(KEY_L)){
            enemyVel ++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel --;           
        }

        // UPDATE STATUS 
        colision = false;
        // AÇÕES DO JOGADOR

        // CHECA SE JOGADOR TA VIVO
        if(player->life < 1){

            gameStage = OVER;
            continue;
        }
        
    
        // MOVER JOGADOR
        if(player->action == MOVE){
            /* 
            TODO:
            - melhorar a funcao. posicao ser alterada apenas depois de verifica se a nova posição esta correta
            - corrigir os nomes dos atributos 
            */
            player->position = (Vector2) {player->box.x, player->box.y}; //oldPos
            player->position = Vector2Add(player->position, player->moviment); //newpos
                        
            // VERIFICA SE JOGADOR DENTRO DA ARENA ATUALIZA POSICAO
            if(isInside(player, &arena)){
                player->box.x = player->position.x; //newPosX
                player->box.y = player->position.y; //newPosX
            } 

        }

        // AÇÕES DOS INIMIGOS   
        
        //colisionRec = (Rectangle){0,0,0,0};
        

        // PERCORRE LISTA DE ITENS
        for(LinkedNode* currentNode = itemListHead; currentNode != NULL ; currentNode = currentNode->next){
            
            currentObj = currentNode->obj;

            if ( currentObj->life < 1 ){
                continue;
            }
            
            // checa colisão com itens
            if(CheckCollisionRecs(player->box, currentObj->box)){
                PlaySound(eatCheese);
                colision = true;
                colisionRec = GetCollisionRec(player->box, currentObj->box);
                currentObj->life = -1;
                score++;


                // criar novo queijo aleatorio
                // todo REFATORAR ESSE CODIGO PARA COLOCAR EM UMA FUNÇÃO
                LinkedNode* newNode;
                newNode = malloc(sizeof(LinkedNode));
                newNode->obj = malloc(sizeof(Object));

                setObject(newNode->obj, (Vector2){rand() %700, rand() % 500}, "resources/item.png");

                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }

        //PERCORRE LISTA DE INIMIGOS
        for(LinkedNode* currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next )
        {

            currentObj = currentNode->obj;

            // Comportamento de perseguição
            currentObj->action = STOP;
            currentObj->moviment = Vector2Zero();
            

            if ((currentObj->box.x - player->position.x) < 0){
                currentObj->action = MOVE;
                currentObj->moviment.x += rand() % enemyVel;
            }else{
                currentObj->action = MOVE;
                currentObj->moviment.x -= (rand() % enemyVel);
            }

            if ((currentObj->box.y - player->position.y) < 0){
                currentObj->action = MOVE;
                currentObj->moviment.y += (rand() % enemyVel);
                
            }else{
                currentObj->action = MOVE;
                currentObj->moviment.y -= (rand() % enemyVel);                
            }

            // ENEMY ACTION
            // move action
            currentObj->position = (Vector2){currentObj->box.x, currentObj->box.y}; //oldPos
            currentObj->position = Vector2Add(currentObj->position, currentObj->moviment);

            if (isInside(currentObj ,&arena))
            {
                currentObj->box.x = currentObj->position.x;
                currentObj->box.y = currentObj->position.y;
            }                   

            if(CheckCollisionRecs(player->box, currentObj->box)){
                PlaySound(getHited);
                colision = true;
                colisionRec= GetCollisionRec(player->box, currentObj->box);
                score --;
                player->life--;
            }
        }
        
        // DRAW
        BeginDrawing();
            
            //limpa tela
            ClearBackground(BLACK);
            
            //desenha mapa
            DrawRectangleRec(arena,GRAY);
            
            //desenha itens
            drawNodeList(itemListHead);

            //desenha inimigos
            drawNodeList(enemyListHead);
                      
            //desenha player
            drawObject(player);

            // Desenha UI
            DrawText(TextFormat("LIFE : %d", player->life), 20, 20, 20, BLACK);
            DrawText(TextFormat("SCORE: %d", score ), 20, 40, 20, BLACK);

            if(colision) DrawCircle(colisionRec.x,colisionRec.y, 10, RED);

        EndDrawing();    



    }

             
    //LIBERAR MEMORIA ENEMY LIST    
    TraceLog(LOG_DEBUG, "--- LIBERAR MEMORIA ENEMY LIST");
    for(LinkedNode *temp = enemyListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next){
        free(prev);
    }

    //LIBERAR MEMORIA ITEM LIST
    TraceLog(LOG_DEBUG, "--- LIBERAR MEMORIA ITEM LIST");
    for(LinkedNode *temp = itemListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next){
        free(prev);
    }
    
    //LIBERAR MEMORIA PLAYER
    TraceLog(LOG_DEBUG, "--- LIBERAR MEMORIA PLAYER");
    free(player);
    
    //LIBERAR MEMORIA MAPA
    TraceLog(LOG_DEBUG, "--- LIBERAR MEMORIA MAPA");

    //LIBERAR MEMORIA EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "--- LIBERAR MEMORIA EFEITOS SONOROS");
    UnloadSound(eatCheese);
    UnloadSound(getHited);

    volume = 0.5;
    return 0;
}


int gameOver(){
    // CARREGAR MUSICA 
    TraceLog(LOG_DEBUG, " -- CARREGANDO MUSICA ");
    Music gameoverMusic = LoadMusicStream("sounds/gameover.mp3");
    SetMusicVolume(gameoverMusic, volume);
    PlayMusicStream(gameoverMusic);

    // CARREGAR IMAGENS 
    TraceLog(LOG_DEBUG, " -- CARREGANDO IMAGENS");
    Texture2D backgroundTex = LoadTexture("resources/gameover.png");
    
    // SETAR TEXTOS
    TraceLog(LOG_DEBUG, " -- CARREGANDO FONTES ");          
    char* textOver = "GAME OVER";
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textOver, FONT_SIZE, FONT_SPACE);
    Rectangle textRec = { (W_WIDTH - textMeasure.x)/2, 400, textMeasure.x, textMeasure.y };

    // INTRO LOOP
    TraceLog(LOG_DEBUG, " -- CARREGAMENTO CONCLUIDO -- ");   
    while(gameStage == OVER){
        //INPUT HANDLE   
        if(IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()){
            gameStage = EXIT;
        }

        if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
            gameStage = INTRO;
        }
        // UPDATE
        UpdateMusicStream(gameoverMusic);

        // DRAW
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(backgroundTex, 0, 0, WHITE);
            DrawRectangleRec(textRec, BLACK);
            DrawTextEx(gameFont, textOver, (Vector2){textRec.x, textRec.y}, FONT_SIZE, FONT_SPACE, WHITE);
        EndDrawing();
    }

    // DESCARREGAR MUSICA 
    StopMusicStream(gameoverMusic);
    UnloadMusicStream(gameoverMusic);
    

    // DESCARREGAR IMAGENS
    UnloadTexture(backgroundTex);
  
    return 0;
}

int main(){
    // Config Screen
    InitWindow(W_WIDTH, W_HEIGHT, "CatAndMouse - GAME"); 
    
    InitAudioDevice();

    SetTraceLogLevel(LOG_DEBUG);
    
    TraceLog(LOG_DEBUG, "Iniciando raylib");

    SetTargetFPS(30);
    
    gameFont = GetFontDefault();

    gameStage = INTRO;
    
    while(gameStage != EXIT){

        if (gameStage == INTRO){
            gameIntro();
        }

        if (gameStage == GAME){
            gameLoop();
        }

        if (gameStage == OVER){
            gameOver();
        }
    }   
    CloseAudioDevice();
    CloseWindow();
    return 0;
}