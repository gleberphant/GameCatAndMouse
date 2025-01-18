/* TODO
 * - criar uma estrutura para CENAS
 * - modularizar carregamento de recursoc com liberação de recursos
 * - REFATORAR SISTEMA DE COLISÃO E MOVIMENTO
 * - CORRIGIR COLISÃO
 * - ELABORAR FUNÇÃO PARA ADICIONAR E REMOVER ITENS DA LISTA:
 * - IMPLEMENTAR LISTA DUPLA
 * - implementer ITEM ratoeira (PRENDE JOGADOR POR UM TEMPO)
 * - IMPLEMENTAR STRUC DO MAPA
 * - carregar mapa de arquivo
 * - implementar uma load screen
 * - implementar sprites sortidos para os itens
 * - 
 */


#include "main.h"



GameStatus gameScene=INTRO;
Font gameFont;
bool debugMode = false;
float volume = 0.01f;
int level, score;



// carregar um objeto musica com tratamento de error
bool loadMusic(Music* music, const char* filepath){
    *music = LoadMusicStream(filepath);
    if(music->stream.buffer == NULL){
        TraceLog(LOG_ERROR, ":: ERRO AO CARREGAR A MUSICA");
        return false;
    }
    return true;
}



// cena loop principal
int gameLoop(){

    // SETAR VARIÁVEIS DO LOOP
    TraceLog(LOG_DEBUG, "== carregando GLOBAL VARIABLES");
    Actor *currentActor = NULL;
    Item *currentItem = NULL;
    Rectangle arena, collisionRec = (Rectangle){0,0, 0,0};
    bool collision = false;
    volume = 1;
    int enemyVel = 4;

    // Carregar o mapa
    TraceLog(LOG_DEBUG, "== carregando MAP");
    Vector2 mapEnemies[] = {
        (Vector2){20.0f, 140.0f},
        (Vector2){650.0f, 120.0f}
        

    };

    Vector2 mapItens[] = {
        (Vector2){100.0f, 120.0f},
        (Vector2){600.0f, 500.0f}
    };

    arena.x = MAP_BORDER;
    arena.y = MAP_BORDER;
    arena.width = W_WIDTH-(MAP_BORDER*2);
    arena.height = W_HEIGHT-(MAP_BORDER*2);


    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "== carregando ENEMY LIST");
    ActorNode* enemyListHead = initActorList(
        mapEnemies,
        "resources/catA",
        sizeof(mapEnemies)/sizeof(Vector2)
    );

    if(enemyListHead == NULL){
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        gameScene = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "== carregando ITENS LIST");
    ItemNode* itemListHead = initItemList(
        mapItens,
        "resources/02_cheese.png",
        sizeof(mapItens)/sizeof(Vector2)
    );

    if(itemListHead == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        gameScene = EXIT;
    }

    // Carregar personagem do jogador
    TraceLog(LOG_DEBUG, "== carregando PLAYER");
    Actor* player = malloc(sizeof(Actor));

    if(player == NULL){
        TraceLog(LOG_ERROR, "::: ERROR ao carregar PLAYER");
        gameScene = EXIT;
    }

    setObject(
        player,
        PLAYER_INIT_POS,
        "resources/mouseA"
    );

    // Inicializar pontuação e nível
    score = 0;
    level = 1;

 

    // CARREGAR EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== carregando EFEITOS SONOROS");
    const Sound eatCheese = LoadSound("sounds/eat_cheese.mp3");
    const Sound getHit = LoadSound("sounds/get_hit.mp3");
    
    Music bgMusic;
    
    if (!loadMusic(&bgMusic, "sounds/game_music.mp3")) gameScene = EXIT;

    
    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic,volume);

    // GAME LOOP
    while (gameScene == GAME) {

        // INPUT HANDLE
        if(WindowShouldClose() == true || player == NULL) {
            gameScene = EXIT;
            break;
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
            enemyVel++;
        }
        if(IsKeyDown(KEY_K) && enemyVel > 1){
            enemyVel--;
        }


        if(IsKeyReleased(KEY_KP_ADD)){
            volume+=0.05f;
            SetMasterVolume(volume);
        }

        if(IsKeyReleased(KEY_KP_SUBTRACT)){
            volume-=0.05f;
            SetMasterVolume(volume);
        }

        if(IsKeyReleased(KEY_F1)){
            debugMode = !debugMode;
        }

        // UPDATE STATUS
        collision = false;


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
            // TODO - MELHORAR O MOVIMENTO

            player->position = Vector2Add(player->position, player->move); //nova posição

            // VERIFICA SE JOGADOR DENTRO DA ARENA
            if(isInside(player, &arena)){
                player->box.x = player->position.x; //newPosX
                player->box.y = player->position.y; //newPosY
            }

            player->position.x = player->box.x;
            player->position.y = player->box.y;

        }

        // AÇÕES DOS INIMIGOS
        // PERCORRE LISTA DE ITENS
        for(ItemNode* currentNode = itemListHead; currentNode != NULL ; currentNode = currentNode->next){

            if ( currentNode->obj->life < 1 ) continue;
            currentItem = currentNode->obj;


            // checa colisão com itens
            if(CheckCollisionRecs(player->box, currentItem->box)){
                PlaySound(eatCheese);
                collision = true;
                collisionRec = GetCollisionRec(player->box, currentItem->box);
                currentItem->life = -1;
                score+=5;

                // criar novo queijo aleatorio
                // todo REFATORAR ESSE CÓDIGO PARA COLOCAR EM UMA FUNÇÃO
                ItemNode* newNode = malloc(sizeof(ItemNode));;
                newNode->obj = malloc(sizeof(Item));

                newNode->obj = getItem(
                    (Vector2) { (float)GetRandomValue(64, W_WIDTH-64), (float)(float) GetRandomValue(64, W_HEIGHT-64)},
                    "resources/00_cheese.png",
                    CHEESE
                );

                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }
        //float distX , distY;
        //PERCORRE LISTA DE INIMIGOS
        for(ActorNode* currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next )
        {

            if ( currentNode->obj->life < 1 ) continue;

            currentActor = currentNode->obj;



            // Comportamento de perseguição
            currentActor->action = STOP;
            currentActor->move = Vector2Zero();

            if(Vector2Distance(currentActor->position, player->position) > 300){ continue;}



            if ((currentActor->box.x < player->position.x) && currentActor->action == STOP){
                    currentActor->action = MOVE;
                    currentActor->direction = RIGHT;
                    currentActor->move.x += (float) GetRandomValue(0, enemyVel);
            }

            if ((currentActor->box.x > player->position.x+64) && currentActor->action == STOP){
                    currentActor->action = MOVE;
                    currentActor->direction =  LEFT;
                    currentActor->move.x -= (float) GetRandomValue(0, enemyVel);
            }

            if ((currentActor->box.y < player->position.y)  && currentActor->action == STOP){
                currentActor->action = MOVE;
                currentActor->direction = DOWN;
                currentActor->move.y += (float) GetRandomValue(0, enemyVel);

            }
            if ((currentActor->box.y > player->position.y+64) && currentActor->action == STOP){
                currentActor->action = MOVE;
                currentActor->direction = UP;
                currentActor->move.y -= (float) GetRandomValue(0, enemyVel);
            }

            // ENEMY ACTION
            // move action
            currentActor->position = (Vector2){currentActor->box.x, currentActor->box.y}; //oldPos
            currentActor->position = Vector2Add(currentActor->position, currentActor->move);

            if (isInside(currentActor ,&arena))
            {
                currentActor->box.x = currentActor->position.x;
                currentActor->box.y = currentActor->position.y;
            }

            if(CheckCollisionRecs(player->box, currentActor->box)){
                PlaySound(getHit);
                collision = true;
                collisionRec= GetCollisionRec(player->box, currentActor->box);
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
            drawItemList(itemListHead);

            //desenha inimigos
            drawActorList(enemyListHead);

            //desenha player

            // DrawRectangleRec(player->box,LIGHTGRAY);
            drawObject(player);




            // Desenha UI
            DrawText(TextFormat("LIFE : %d", player->life), 20, 20, 20, BLACK);
            DrawText(TextFormat("SCORE: %d", score ), 20, 40, 20, BLACK);

            DrawText(TextFormat("LEVEL: %d", level ), 500, 20, 20, BLACK);
            DrawText(TextFormat("Enemy Vel: %d", enemyVel ), 500, 40, 20, BLACK);

            if(collision) DrawCircle((int)collisionRec.x,(int)collisionRec.y, 10, RED);

        EndDrawing();

        UpdateMusicStream(bgMusic);
    }


    //LIBERAR MEMORIA ENEMY LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ENEMY LIST");
    for(ActorNode *temp = enemyListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next){
        free(prev);
    }

    //LIBERAR MEMORIA ITEM LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ITEM LIST");
    for(ItemNode *temp = itemListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next){
        free(prev);
    }

    //LIBERAR MEMORIA PLAYER
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA PLAYER");
    free(player);

    //LIBERAR MEMORIA MAPA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MAPA");

    //LIBERAR MEMORIA EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA EFEITOS SONOROS");
    UnloadSound(eatCheese);
    UnloadSound(getHit);


    //LIBERAR MEMORIA MUSICA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MUSICA");
    StopMusicStream(bgMusic);
    UnloadMusicStream(bgMusic);

    volume = 0.5f;
    return 0;
}

// cena de introdução
int gameIntro(){

    // CARREGAR MUSICA 
    TraceLog(LOG_DEBUG, " == CARREGANDO MUSICA ");
    Music introMusic = LoadMusicStream("sounds/intro_music.mp3");
    SetMusicVolume(introMusic, 1.0f);
    PlayMusicStream(introMusic);

    // CARREGAR IMAGENS DE FUNDO
    TraceLog(LOG_DEBUG, " == CARREGANDO IMAGENS");
    Texture2D backgroundTex = LoadTexture("resources/intro_bg.png");

    // SETAR TEXTOS
    TraceLog(LOG_DEBUG, " == CARREGANDO FONTES ");
    char* textIntro = "[ENTER] para jogar [ESC] para sair.";            
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textIntro, FONT_SIZE, FONT_SPACE);
    Rectangle textRec = { (W_WIDTH - textMeasure.x)/2, 400, textMeasure.x, textMeasure.y };
  
    // INTRO LOOP
    TraceLog(LOG_DEBUG, " == CARREGAMENTO CONCLUÍDO ");
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
            DrawTexture(backgroundTex, 0, 0, WHITE);
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


// cena de game over
int gameOver(){
    // CARREGAR MUSICA 
    TraceLog(LOG_DEBUG, " == CARREGANDO MUSICA == ");
    const Music gameoverMusic = LoadMusicStream("sounds/gameover.mp3");
    SetMusicVolume(gameoverMusic, volume);
    PlayMusicStream(gameoverMusic);

    // CARREGAR IMAGENS 
    TraceLog(LOG_DEBUG, " == CARREGANDO IMAGENS ==");
    const Texture2D backgroundTex = LoadTexture("resources/gameover.png");
    
    // SETAR TEXTOS
    TraceLog(LOG_DEBUG, " == CARREGANDO FONTES == ");
    char* textOver = "GAME OVER";
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textOver, FONT_SIZE, FONT_SPACE);
    Rectangle textRec = { (W_WIDTH - textMeasure.x)/2, 400, textMeasure.x, textMeasure.y };

    // INTRO LOOP
    TraceLog(LOG_DEBUG, " == CARREGAMENTO CONCLUÍDO == ");
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
    InitWindow(W_WIDTH, W_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");
    
    InitAudioDevice();

    SetTraceLogLevel(LOG_DEBUG);
    
    TraceLog(LOG_DEBUG, "Iniciando raylib");

    SetTargetFPS(30);

    SetMasterVolume(volume);

    gameFont = GetFontDefault();

    gameScene = INTRO;
    
    // ReSharper disable once CppDFALoopConditionNotUpdated
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