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
};

// LINKED LIST DE OBJETOS
struct LinkedNode{
    Object* node;
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
Object *player, *currentEnemy, *currentItem;
Rectangle arena;
Actions playerAction;
GameStatusType gameStage;
int score;
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
    TraceLog(LOG_DEBUG, "Loading person %s", sprite);
    target->position = initPos;
    target->moviment = Vector2Zero();
    target->sprite = LoadTexture(sprite);
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
        newListNode->node = malloc(sizeof(Object));
        setObject(newListNode->node, initPosition[i], sprite);
        newListNode->next = headListNode;
        headListNode = newListNode;
        
    }
    return headListNode;
}


void drawNode(Object* target){
    // DrawRectangleRec(
    // (Rectangle){target->position.x-2, target->position.y-2, target->size.x+2, target->size.y+2 }, 
    // BLACK
    // );
    DrawTexture(target->sprite, target->position.x, target->position.y, WHITE);
}

void drawNodeList(LinkedNode* targetList){
    LinkedNode *currentEnemy;
    for(currentEnemy = targetList; currentEnemy != NULL ; currentEnemy = currentEnemy->next)
    {
        drawNode(currentEnemy->node);
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

        colisionRec = (Rectangle){0 ,0,0,0};
        playerRec = (Rectangle){ 
                player->position.x, 
                player->position.y,
                player->size.x,
                player->size.y
        };

        for(LinkedNode* itemNode = itemListHead; itemNode != NULL ; itemNode = itemNode->next){
            itemRec = (Rectangle){ 
                itemNode->node->position.x, 
                itemNode->node->position.y,
                itemNode->node->size.x,
                itemNode->node->size.y
            };

            if(CheckCollisionRecs(playerRec, itemRec)){
                
                colisionRec = GetCollisionRec(playerRec, itemRec);
                
            }
        }
        for(LinkedNode* currentEnemy = enemyListHead; currentEnemy != NULL; currentEnemy = currentEnemy->next )
        {
            // Comportamento de perseguição
            enemyRec = (Rectangle){ 
                currentEnemy->node->position.x, 
                currentEnemy->node->position.y,
                currentEnemy->node->size.x,
                currentEnemy->node->size.y
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
            currentEnemy->node->moviment = playerAction.moviment;
            oldPos = currentEnemy->node->position;
            currentEnemy->node->position = Vector2Add(currentEnemy->node->position, currentEnemy->node->moviment);
            currentEnemy->node->moviment = Vector2Zero();


            // CHECK COLISIONS
            if (!isInside(currentEnemy->node ,&arena))
            {
                currentEnemy->node->position = oldPos;
            }                   

            if(CheckCollisionRecs(playerRec, enemyRec)){
                
                colisionRec= GetCollisionRec(playerRec, enemyRec);
                
                DrawRectangleRec(playerRec,RED);    
            }

        }
        
        // DRAW
        BeginDrawing();
            // ClearBackground(WHITE);
            DrawRectangleRec(arena,GRAY);

            drawNodeList(enemyListHead);

            drawNodeList(itemListHead);
            drawNode(player);

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
    }   

    CloseWindow();
    return 0;
}