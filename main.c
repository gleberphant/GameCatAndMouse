/* TODO
- ELABORAR FUNÇÃO PARA ADICIONAR E REMOVER ITENS DA LISTA
- carregar mapa de arquivo
- separar item de ator
- impementar diferentes comportamentos para o gato
- implementer ratoeira
*/


#include "main.h"

#define W_WIDTH 800
#define W_HEIGHT 600
#define MAP_BORDER 10
#define FONT_SIZE 32
#define FONT_SPACE 2.0f

#define PLAYER_INIT_POS (Vector2){380.0f, 300.0f}
// VARIAVEIS GLOBAIS

GameStatus gameScene;
Font gameFont; 

float volume = 0.01f;
int score = 0, level = 1;
char textBuffer[100];

/* FUNÇÕES */



// verifica se um objeto esta dentro de outro
bool isInside(Object* target, Rectangle *arena){
/* TODO passar um objeto RECTANGLE ao inves de um objeto inteiro*/
        
    if( target->position.x < arena->x || target->position.x + target->box.width > (arena->width + arena->x)){
        return false;
    }
    if( target->position.y < arena->y || target->position.y + target->box.height > (arena->height + arena->y)){
        return false;
    }
    return true;
}


// cena de introdução
int gameIntro(){

    // CARREGAR MUSICA 
    TraceLog(LOG_DEBUG, " -- CARREGANDO MUSICA ");
    Music introMusic = LoadMusicStream("sounds/intro_music.mp3");
    SetMusicVolume(introMusic, 1.0f);
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
    while (gameScene == INTRO){
        // INPUT HANDLE
        if(IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()){
            gameScene = EXIT;
        }

        if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
            gameScene = GAME;
        }

        if(IsKeyReleased(KEY_KP_ADD)){
            volume+=0.05f;
            SetMasterVolume(volume);
        }

        if(IsKeyReleased(KEY_KP_SUBTRACT)){
            volume-=0.05f;
            SetMasterVolume(volume);
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


// cena loop principal
int gameLoop(){

    // SETAR VARIAVEIS DO LOOP
    TraceLog(LOG_DEBUG, "- carregando GLOBAL VARIABLES");
    Object *currentObj;
    Rectangle arena;
    
    Rectangle colisionRec;
    bool colision;
    volume = 1;
    
    unsigned short enemyVel = 4;
        
    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "-- carregando ENEMY LIST");
    LinkedNode* enemyListHead = initLinkedList(
        mapEnemys,
        "resources/catA", 
        (unsigned short )sizeof(mapEnemys)/sizeof(Vector2)
    );

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        gameScene = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "--- carregando ITENS LIST");
    LinkedNode* itemListHead = initLinkedList(
        mapItens,
        "resources/item", 
        (unsigned short)sizeof(mapItens)/sizeof(Vector2)
    );

    if(itemListHead == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        gameScene = EXIT;
    }

    // CARREGAR PERSONAGEM JOGADOR
    TraceLog(LOG_DEBUG, "--- carregando PLAYER");
    Object* player = malloc(sizeof(Object));

    if(player == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar PLAYER");
        gameScene = EXIT;
    }

    setObject(
        player, 
        PLAYER_INIT_POS, 
        "resources/mouseA" 
    ); 

    score = 0;

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
    while (gameScene == GAME) {
        
        // INPUT HANDLE
        if(WindowShouldClose() == true)
        {
            gameScene = EXIT;
        }
        
        player->action = STOP;
        player->move = Vector2Zero();
        
        if(IsKeyDown(KEY_UP)){
            player->action = MOVE;
            player->direction = UP;
            player->move.y -= 10;
        }
        if(IsKeyDown(KEY_DOWN)){
            player->action = MOVE;
            player->direction = DOWN;
            player->move.y += 10;
        }
        if(IsKeyDown(KEY_LEFT)){
            player->action = MOVE;
            player->direction = LEFT;
            player->move.x -= 10;
        }
        if(IsKeyDown(KEY_RIGHT)){
            player->action = MOVE;
            player->direction = RIGHT;
            player->move.x += 10;
        }
        
        if(IsKeyDown(KEY_SPACE)){
            player->action = SPECIAL;
            
        }

        if(IsKeyDown(KEY_L)){
            enemyVel ++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel --;           
        }


        if(IsKeyReleased(KEY_KP_ADD)){
            volume+=0.05f;
            SetMasterVolume(volume);
        }

        if(IsKeyReleased(KEY_KP_SUBTRACT)){
            volume-=0.05f;
            SetMasterVolume(volume);
        }


        // UPDATE STATUS 
        colision = false;
        
        
        if(score > level * 20){   
            enemyVel+=2;
            level++;
        }

        // AÇÕES DO JOGADOR

        // CHECA SE JOGADOR TA VIVO
        if(player->life < 1){

            gameScene = OVER;
            continue;
        }
        
    
        // MOVER JOGADOR
        if(player->action == MOVE){
            /* 
            TODO:
            - melhorar a funcao. posicao ser alterada apenas depois de verifica se a nova posição esta correta
            - corrigir os nomes dos atributos 
            */
    
            player->position = Vector2Add(player->position, player->move); //nova posicao
                        
            // VERIFICA SE JOGADOR DENTRO DA ARENA ATUALIZA POSICAO
            if(isInside(player, &arena)){
                player->box.x = player->position.x; //newPosX
                player->box.y = player->position.y; //newPosY
            } 

            player->position.x = player->box.x;
            player->position.y = player->box.y;

        }

        // AÇÕES DOS INIMIGOS   
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
                score+=5;

                // criar novo queijo aleatorio
                // todo REFATORAR ESSE CODIGO PARA COLOCAR EM UMA FUNÇÃO
                LinkedNode* newNode;
                newNode = malloc(sizeof(LinkedNode));
                newNode->obj = malloc(sizeof(Object));

                setObject(newNode->obj, (Vector2){rand() %700, rand() % 500}, "resources/item");

                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }
        //float distX , distY;
        //PERCORRE LISTA DE INIMIGOS
        for(LinkedNode* currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next )
        {

            currentObj = currentNode->obj;

            // Comportamento de perseguição
            currentObj->action = STOP;
            currentObj->move = Vector2Zero();
            
            if(Vector2Distance(currentObj->position, player->position) > 300){ continue;}

          

            if ((currentObj->box.x < player->position.x) && currentObj->action == STOP){
                    currentObj->action = MOVE;
                    currentObj->direction = RIGHT;
                    currentObj->move.x += (rand() % enemyVel);
            }
            
            if ((currentObj->box.x > player->position.x+64) && currentObj->action == STOP){
                    currentObj->action = MOVE;
                    currentObj->direction =  LEFT;
                    currentObj->move.x -= (rand() % enemyVel);
            }

            if ((currentObj->box.y < player->position.y)  && currentObj->action == STOP){
                currentObj->action = MOVE;
                currentObj->direction = DOWN;
                currentObj->move.y += (rand() % enemyVel);
                
            }
            if ((currentObj->box.y > player->position.y+64) && currentObj->action == STOP){
                currentObj->action = MOVE;
                currentObj->direction = UP;
                currentObj->move.y -= (rand() % enemyVel);                
            }

            // ENEMY ACTION
            // move action
            currentObj->position = (Vector2){currentObj->box.x, currentObj->box.y}; //oldPos
            currentObj->position = Vector2Add(currentObj->position, currentObj->move);

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
            //if(player->action == MOVE) updateAnimationFrame (player->spriteA);
            drawObject(player);
            
            

            // Desenha UI
            DrawText(TextFormat("LIFE : %d", player->life), 20, 20, 20, BLACK);
            DrawText(TextFormat("SCORE: %d", score ), 20, 40, 20, BLACK);

            DrawText(TextFormat("LEVEL: %d", level ), 500, 20, 20, BLACK);
            DrawText(TextFormat("Enemy Vel: %d", enemyVel ), 500, 40, 20, BLACK);

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
    while(gameScene == OVER){
        //INPUT HANDLE   
        if(IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()){
            gameScene = EXIT;
        }

        if(IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)){
            gameScene = INTRO;
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

    SetMasterVolume(volume);

    srand(time(NULL));
    
    gameFont = GetFontDefault();

    gameScene = INTRO;
    
    while(gameScene != EXIT){

        if (gameScene == INTRO){
            gameIntro();
        }

        if (gameScene == GAME){
            gameLoop();
        }

        if (gameScene == OVER){
            gameOver();
        }
    }   
    CloseAudioDevice();
    CloseWindow();
    return 0;
}