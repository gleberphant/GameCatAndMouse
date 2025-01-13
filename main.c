/* TODO
- COLISAO ITEM = MAIS PONTO
- COLISAO ENEMY = MORTE
- ELABORAR FUNÇÃO PARA ADICIONAR E REMOVER ITENS DA LISTA
- carregar mapa de arquivo
*/


#include "include/raylib.h"
#include "include/raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define W_WIDTH 800
#define W_HEIGHT 600
#define MAP_BORDER 10

// DECLARAÇÃO DE STRUCT E ENUM
typedef struct Object Object;
typedef struct LinkedNode LinkedNode;
typedef struct Actions Actions;
typedef struct SetupNode SetupNode;

typedef enum ActionType ActionType;
typedef enum GameStatusType GameStatusType;
typedef enum NodeType NodeType;



// TIPOS DE AÇÕES
enum ActionType{
    MOVE,
    STOP
};

// ESTADOS QUE O JOGO PODE SE ENCONTRAR
enum GameStatusType { 
    INTRO, 
    MENU, 
    GAME, 
    OVER,
    EXIT 
};

enum NodeType{
    PLAYER,
    ENEMY,
    ITEM
};

struct SetupNode{
    Vector2 position;
    NodeType type;
};

//OBJETO PRINCIPAL. SERVE PARA JOGADOR, INIMIGOS E ITENS
struct Object{
    Vector2 position;
    Vector2 moviment;
    Vector2 size;
    Rectangle box;
    NodeType type;
    Texture2D sprite;
    int life;
};

// LINKED LIST DE OBJETOS
struct LinkedNode{
    Object* obj;
    LinkedNode* next;
};

// DEFINE UMA AÇÃO FEITA POR UM OBJETO 
struct Actions{
    Vector2 moviment;
    ActionType type;
};

// estrutura do mapa 
struct Map{
    Rectangle area;
    Color color;
    LinkedNode *enemyList;
    LinkedNode *itensList;
};

// VARIAVEIS GLOBAIS
LinkedNode *enemyListHead, *itemListHead;
Object *player, *currentItem;
Rectangle arena;
Actions playerAction;
GameStatusType gameStage;
int score = 0;
bool death = false;
char text[100];

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
    target->position = initPos;
    target->moviment = Vector2Zero();
    target->sprite = LoadTexture(sprite);
    target->size.x = target->sprite.width;
    target->size.y = target->sprite.height;
    target->life = 10;
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
 
    DrawTextureV(target->sprite, target->position, WHITE);
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

bool isInside(Object* target, Rectangle *arena){
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

            // carregar imagem e texto de abertura
            Font fontDefault = GetFontDefault(); 
            Texture2D bgTexture = LoadTexture("resources/intro_bg.png");
            int textFontSize = 32;
            char* textIntro = "[ENTER] para jogar [ESC] para sair.";            

            Vector2 textSize = MeasureTextEx(fontDefault, textIntro, textFontSize, 2);
            Vector2 textPos = (Vector2){(W_WIDTH - textSize.x)/2, 400 };              
            
            Rectangle textBackground = { textPos.x-20, textPos.y-20, textSize.x+60, textSize.y+40 };

            // aguardar o jogador apertar ENTER ou ESC
            while (gameStage == INTRO){
         
                if(IsKeyDown(KEY_ESCAPE) || WindowShouldClose()){
                    gameStage = EXIT;
                }

                if(IsKeyDown(KEY_ENTER)){
                    gameStage = GAME;
                }

                BeginDrawing();
                    DrawTextureEx(bgTexture, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
                    DrawRectangleRec(textBackground, BLACK);
                    DrawText(textIntro, textPos.x, textPos.y, textFontSize, WHITE);
                EndDrawing();
            }

    return 0;
}


int gameLoop(){

// Load Enemy list
    TraceLog(LOG_DEBUG, "- Loading ENEMYS");
    
    enemyListHead = initLinkedList(
        mapEnemys,
        "resources/cat.png", 
        (unsigned short )sizeof(mapEnemys)/sizeof(Vector2)
    );

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ENEMY LIST");
        gameStage = EXIT;
        return 1;
    }

    TraceLog(LOG_DEBUG, "- Loading ITENS");
    itemListHead = initLinkedList(
        mapItens,
        "resources/item.png", 
        (unsigned short)sizeof(mapItens)/sizeof(Vector2)
    );

    if(itemListHead == NULL){
        TraceLog(LOG_ERROR, "Failed to load ITENS LIST");
        gameStage = EXIT;
        return 1;
    }

    // LOAD PLAYER
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    player = malloc(sizeof(Object));

    if(player == NULL){
        TraceLog(LOG_ERROR, "Failed to load PLAYER");
        gameStage = EXIT;
        return 1;
    }

    setObject(player, (Vector2){400.0f, 400.0f}, "resources/mouse.png" );
    
    // LOAD MAP
    TraceLog(LOG_DEBUG, "- Loading PLAYER");
    arena.x = 0+MAP_BORDER;
    arena.y = 0+MAP_BORDER;
    arena.width = W_WIDTH-(MAP_BORDER*2); 
    arena.height = W_HEIGHT-(MAP_BORDER*2);
    
    // SET VARIABLES
    TraceLog(LOG_DEBUG, "- set VARIABLES");
    Vector2 oldPos;
    srand(time(NULL));
    unsigned short enemyVel = 4;
    
    // GAME LOOP
    while (gameStage == GAME) {
        // INPUT HANDLE
        if(WindowShouldClose() == true)
        {
            gameStage = EXIT;
        }
        
        playerAction.type = STOP;
        playerAction.moviment = Vector2Zero();
        
        if(IsKeyDown(KEY_UP)){
            playerAction.type = MOVE;
            playerAction.moviment.y -= 10;
        }
        if(IsKeyDown(KEY_DOWN)){
            playerAction.type = MOVE;
            playerAction.moviment.y += 10;
        }
        if(IsKeyDown(KEY_LEFT)){
            playerAction.type = MOVE;
            playerAction.moviment.x -= 10;
        }
        if(IsKeyDown(KEY_RIGHT)){
            playerAction.type = MOVE;
            playerAction.moviment.x += 10;
        }
        
        if(IsKeyDown(KEY_L)){
            enemyVel ++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel --;           
        }

        // UPDATE STATUS
        
        // PLAYER
        // PLAYER ACTION
        // checka se player ta vivo
        if(player->life < 1){

            gameStage = OVER;
            continue;
        }
        
        // move action


        if(playerAction.type == MOVE){
            player->moviment = playerAction.moviment;
            oldPos = player->position;
            player->position = Vector2Add(player->position, player->moviment);
            player->moviment = Vector2Zero();
            
            // CHECK COLISION
            if(!isInside(player, &arena)){
                player->position = oldPos;
            }        
        }

        // UPDATE ENEMYS
        Rectangle enemyRec, playerRec, itemRec, colisionRec;

        colisionRec = (Rectangle){0,0,0,0};
        
        playerRec = (Rectangle){ 
                player->position.x, 
                player->position.y,
                player->size.x,
                player->size.y
        };

        // checa colisão com itens
        for(LinkedNode* currentNode = itemListHead; currentNode != NULL ; currentNode = currentNode->next){
            
            if ( currentNode->obj->life < 1 ){
                continue;
            }
            
            itemRec = (Rectangle){ 
                currentNode->obj->position.x, 
                currentNode->obj->position.y,
                currentNode->obj->size.x,
                currentNode->obj->size.y
            };

            if(CheckCollisionRecs(playerRec, itemRec)){
                // ocorreu colisão

                colisionRec = GetCollisionRec(playerRec, itemRec);
                currentNode->obj->life = -1;
                score++;
                TraceLog(LOG_DEBUG, "COMEU QUEIJO score %d", score);

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
        for(LinkedNode* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            // Comportamento de perseguição
            enemyRec = (Rectangle){ 
                currentEnemy->obj->position.x, 
                currentEnemy->obj->position.y,
                currentEnemy->obj->size.x,
                currentEnemy->obj->size.y
            };

            playerAction.moviment = Vector2Zero();
            playerAction.type = STOP;

            if ((enemyRec.x - player->position.x) < 0){
                playerAction.type = MOVE;
                playerAction.moviment.x += rand() % enemyVel;
            }else{
                playerAction.type = MOVE;
                playerAction.moviment.x -= (rand() % enemyVel);
            }

            if ((enemyRec.y - player->position.y) < 0){
                playerAction.type = MOVE;
                playerAction.moviment.y += (rand() % enemyVel);
                
            }else{
                playerAction.type = MOVE;
                playerAction.moviment.y -= (rand() % enemyVel);                
            }

            // ENEMY ACTION
            // move action
            currentEnemy->obj->moviment = playerAction.moviment;
            oldPos = currentEnemy->obj->position;
            currentEnemy->obj->position = Vector2Add(currentEnemy->obj->position, currentEnemy->obj->moviment);
            currentEnemy->obj->moviment = Vector2Zero();


            // CHECK COLISIONS
            if (!isInside(currentEnemy->obj ,&arena))
            {
                currentEnemy->obj->position = oldPos;
            }                   

            if(CheckCollisionRecs(playerRec, enemyRec)){
                colisionRec= GetCollisionRec(playerRec, enemyRec);
                score --;
                player->life--;
    
            }

        }
        
        // DRAW
        BeginDrawing();
            // ClearBackground(WHITE);
            DrawRectangleRec(arena,GRAY);

            drawNodeList(enemyListHead);

            drawNodeList(itemListHead);
            drawObject(player);

            DrawCircle(colisionRec.x,colisionRec.y, 10, RED);

        EndDrawing();    
    }

    
    free(player);
     
    //FREE enemy list
    LinkedNode *temp; 
    int i = 0;

    while(enemyListHead != NULL){
        temp = enemyListHead->next;
        free(enemyListHead);
        enemyListHead = temp;
        i++;
    }

    TraceLog(LOG_DEBUG, "- Liberado %d ENEMY NODE", i);

    //FREE enemy list
    i = 0;
    while(itemListHead != NULL){
        temp = itemListHead->next;
        free(itemListHead);
        itemListHead = temp;
        i++;
    }

    TraceLog(LOG_DEBUG, "- Liberado %d ITEM NODE", i);
    
    return 0;
}


int gameOver(){

    

    // carregar imagem e texto de abertura
    Font fontDefault = GetFontDefault(); 
    Texture2D gameover = LoadTexture("resources/gameover.png");
    int textFontSize = 32;
    char* textIntro = "[ENTER] para jogar [ESC] para sair.";            
    Vector2 textSize = MeasureTextEx(fontDefault, textIntro, textFontSize, 2);
    Vector2 textPos = (Vector2){(W_WIDTH - textSize.x)/2, 400 };              
    
    Rectangle textBackground = { textPos.x-20, textPos.y-20, textSize.x+60, textSize.y+40 };

    while(gameStage == OVER){

        if(IsKeyPressed(KEY_ESCAPE)){
            gameStage = EXIT;

        }
        if(IsKeyReleased(KEY_ENTER)){
            gameStage = INTRO;
        }

        BeginDrawing();
            ClearBackground(BLACK);
           // DrawTextureEx(gameover, (Vector2){0.0f, 0.0f}, 0.0f, 1.0f, WHITE);
            
            DrawTextureV(gameover, (Vector2){0,0}, WHITE);
            DrawRectangleRec(textBackground, BLACK);
            DrawText(textIntro, textPos.x, textPos.y, textFontSize, WHITE);
        EndDrawing();
    }
    return 0;
}

int main(){
    // Config Screen
    InitWindow(W_WIDTH, W_HEIGHT, "CatAndMouse - GAME"); 
    
    SetTraceLogLevel(LOG_DEBUG);
    
    TraceLog(LOG_DEBUG, "Iniciando raylib");

    SetTargetFPS(30);
    

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

    CloseWindow();
    return 0;
}