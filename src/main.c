
/* TODO
 *   - REFATORAR. MODULARIZAR CENAS
 *   - implementar MAP TILE para cenário.
 *   - salvar score em arquivo
 *   - IMPLEMENTAR STRUC DO MAPA
 *   - carregar mapa de arquivo
 *   - implementar uma load screen
 *   - criar uma estrutura para CENAS
 *   - aperfeiçoar para rodar em browser.
 */

#include "main.h"
#include "raymath.h"


#include "animation.h"
#include "actor.h"
#include "lists.h"
#include "itens.h"
#include "maps.h"
#include "scene_over.h"
#include "scene_intro.h"
#include "scene_game.h"

#include <math.h>


GameStatus gameScene = INTRO;
Font gameFont;
bool debugMode = false;
float volume = 0.01f, angulo;
int level, score;
Actor *currentActor = NULL;
Item *currentItem = NULL;
Texture2D *itemSpriteSheet;


// cena loop principal
int gameLoop() {
    // SETAR VARIÁVEIS DO LOOP
    TraceLog(LOG_DEBUG, "== definindo GLOBAL VARIABLES");
    Rectangle arena;
    volume = 1;
    int enemyVel = 4;

    // Carregar o mapa
    TraceLog(LOG_DEBUG, "== carregando MAPAS");
    Vector2 mapEnemies[] = {
        (Vector2){20.0f, 140.0f},
        (Vector2){650.0f, 120.0f}
    };

    InitListItens initItens[] = {
        (InitListItens){.type = CHEESE, .initPos = (Vector2){100.0f, 120.0f} },
        (InitListItens){.type = CHEESE, .initPos = (Vector2){600.0f, 500.0f} },
        (InitListItens){.type = TRAP,   .initPos = (Vector2){300.0f, 300.0f} }
    };

    arena.x = MAP_BORDER;
    arena.y = MAP_BORDER;
    arena.width = SCREEN_WIDTH - (MAP_BORDER * 2);
    arena.height = SCREEN_HEIGHT - (MAP_BORDER * 2);


    // CARREGAR LISTA DE INIMIGOS
    TraceLog(LOG_DEBUG, "== carregando ENEMY LIST");
    ActorNode *enemyListHead = getActorList(
        mapEnemies,
        "resources/catA",
        sizeof(mapEnemies) / sizeof(Vector2)
    );

    if (enemyListHead == NULL) {
        TraceLog(LOG_ERROR, " ::: ERROR ao carregar ENEMY LIST");
        gameScene = EXIT;
    }

    // CARREGAR LISTA DE ITENS
    TraceLog(LOG_DEBUG, "== carregando ITENS LIST");
    itemSpriteSheet = malloc(sizeof(Texture2D) * 4);

    itemSpriteSheet[CHEESE] = LoadTexture("resources/item_cheese.png");
    itemSpriteSheet[STRAWBERRY] = LoadTexture("resources/item_strawberry.png");
    itemSpriteSheet[TRAP] = LoadTexture("resources/item_trap.png");
    itemSpriteSheet[END_ITEM] = (Texture2D){0};


    ItemNode *itemListHead = getItemList(
        initItens,
        itemSpriteSheet,
        sizeof(initItens) / sizeof(InitListItens)
    );

    if (itemListHead == NULL) {
        TraceLog(LOG_ERROR, "::: ERROR ao carregar ITENS LIST");
        gameScene = EXIT;
    }

    // Carregar personagem do jogador
    TraceLog(LOG_DEBUG, "== carregando PLAYER");


    ActorNode *playerListHead = getActorList(
        (Vector2[]){PLAYER_INIT_POS},
        "resources/mouseA",
        1
    );

    Actor *player = playerListHead->obj;

    // Inicializar pontuação e nível
    score = 0;
    level = 1;


    // CARREGAR EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== carregando EFEITOS SONOROS");
    const Sound eatCheese = LoadSound("resources/sounds/eat_cheese.mp3");
    const Sound eatStrawberry = LoadSound("resources/sounds/eat_strawberry.mp3");

    const Sound getHit = LoadSound("resources/sounds/get_hit.mp3");

    Music bgMusic;

    if (!loadMusic(&bgMusic, "resources/sounds/game_music.mp3")) gameScene = EXIT;


    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, volume);
    SetExitKey(KEY_NULL);
    // GAME LOOP
    while (gameScene == GAME) {
        // VERIFICA NÍVEL
        if (score > level * 20) {
            enemyVel += 2;
            level++;
        }


        // INPUT HANDLE
        if (IsKeyPressed(KEY_ESCAPE)) {
            gameScene = INTRO;
            break;
        }

        if (WindowShouldClose() || player == NULL) {
            gameScene = EXIT;
            break;
        }

        // DEBUG OPTIONS
        if (IsKeyReleased(KEY_F1)) {
            debugMode = !debugMode;
        }

        if (IsKeyDown(KEY_L)) {
            enemyVel++;
        }
        if (IsKeyDown(KEY_K) && enemyVel > 1) {
            enemyVel--;
        }

        if (IsKeyReleased(KEY_KP_ADD)) {
            volume += 0.05f;
            SetMasterVolume(volume);
        }

        if (IsKeyReleased(KEY_KP_SUBTRACT)) {
            volume -= 0.05f;
            SetMasterVolume(volume);
        }

        // CONTROL PLAYER

        //player->velocity = (Vector2){ 0.0f, 0.0f };
        //player->direction = 0.0f;
        if (player->action == MOVE || player->action == STOP) {
            player->action = STOP;

            if (IsKeyDown(KEY_UP)) {
                player->action = MOVE;
                player->direction = 0.0f;
                if (IsKeyDown(KEY_RIGHT))player->direction += 45.0f;
                if (IsKeyDown(KEY_LEFT))player->direction -= 45.0f;
            } else if (IsKeyDown(KEY_RIGHT)) {
                player->action = MOVE;
                player->direction = 90.0f;
                if (IsKeyDown(KEY_DOWN))player->direction += 45.0f;
            } else if (IsKeyDown(KEY_DOWN)) {
                player->action = MOVE;
                player->direction = 180.0f;
                if (IsKeyDown(KEY_LEFT))player->direction += 45.0f;
            } else if (IsKeyDown(KEY_LEFT)) {
                player->action = MOVE;
                player->direction = 270.0f;
                if (IsKeyDown(KEY_UP))player->direction += 45.0f;
            }

            // controle por mouse
            if (
                IsMouseButtonDown(MOUSE_BUTTON_LEFT) &&
                Vector2Distance(player->position, GetMousePosition()) > player->collisionBox.height
                ) {

                player->action = MOVE;
                player->direction = Vector2LineAngle(
                                        player->position,
                                        GetMousePosition()
                                    ) * RAD2DEG * -1.0f;
                player->direction += 90.0f;
            }
        }

        if (IsKeyReleased(KEY_SPACE)) {
            if (player->action == SPECIAL) player->action = STOP;
            else player->action = SPECIAL;
        }
        // UPDATE
        // Condições de vitória ou derrota
        if (player->life < 1) {
            gameScene = OVER;
            continue;
        }

        switch (player->action) {
            case STOP:
                actionStop(player);
                break;

            case MOVE:
                actionMove(player, &arena);
                break;

            case SPECIAL:
                actionSpecial(player, player);
                break;

            case END:
                TraceLog(LOG_DEBUG, "fim de linha");
                break;

            default:
                TraceLog(LOG_DEBUG, "item desconhecido");
                break;
        }
        //------------------------------------------------------------
        // PERCORRE LISTA DE ITENS
        for (ItemNode *currentNode = itemListHead, *prev = NULL; currentNode != NULL;
             prev = currentNode, currentNode = currentNode->next) {
            currentItem = currentNode->obj;
            currentItem->life--;

            // checa colisão com item
            if (CheckCollisionRecs(player->collisionBox, currentItem->collisionBox)) {
                switch (currentItem->type) {
                    case CHEESE:
                        PlaySound(eatCheese);

                        currentItem->life = -1;
                        score += 5;
                        break;

                    case STRAWBERRY:
                        PlaySound(eatStrawberry);

                        currentItem->life = -1;
                        player->life += 10;
                        break;

                    case TRAP:

                        currentItem->collisionBox.x = (float) GetRandomValue(64, SCREEN_WIDTH  - 74) - currentItem->collisionBox.width / 2;
                        currentItem->collisionBox.y = (float) GetRandomValue(64, SCREEN_HEIGHT - 74) - currentItem->collisionBox.height / 2;
                        player->life -= 10;
                        player->action = SPECIAL;
                        break;
                    default:
                        TraceLog(LOG_DEBUG, "item desconhecido");
                        break;
                }
            }

            // todo REFATORAR ESSE CÓDIGO PARA COLOCAR EM UMA FUNÇÃO
            // criar item novo e remover anterior
            if (currentItem->life < 1) {
                // REMOVER O NÓ
                if (prev == NULL) {
                    prev = currentNode->next;
                    itemListHead = prev;
                } else prev->next = currentNode->next;
                TraceLog(LOG_DEBUG, "== liberando ITEM %p", currentNode);
                free(currentNode);

                currentNode = prev;
                if (currentNode == NULL) break;

                // CRIA UM NOVO ITEM ALEATÓRIO
                ItemNode *newNode = malloc(sizeof(ItemNode));;
                newNode->obj = malloc(sizeof(Item));

                ItemType newItemType = (ItemType) GetRandomValue(CHEESE, TRAP);
                newNode->obj = getItem(
                    (Vector2){
                        (float) GetRandomValue(64, SCREEN_WIDTH - 64), (float) (float) GetRandomValue(64, SCREEN_HEIGHT - 64)
                    },
                    &itemSpriteSheet[newItemType],
                    newItemType
                );

                newNode->next = currentNode->next;
                currentNode->next = newNode;
            }
        }

        //------------------------------------------------------------
        //PERCORRE LISTA DE INIMIGOS
        for (ActorNode *currentNode = enemyListHead; currentNode != NULL; currentNode = currentNode->next) {
            // verifica se está vivo
            if (currentNode->obj->life < 1) continue;

            // zera variáveis
            currentActor = currentNode->obj;
            currentActor->action = STOP; // ação padrão
            currentActor->velocity = Vector2Zero();

            // se estiver distante do jogador então permanece parado
            if (Vector2Distance(currentActor->position, player->position) > 300) continue;

            // se estiver próximo do jogador então comportamento de perseguição
            if ((currentActor->collisionBox.x < player->position.x) && currentActor->action == STOP) {
                currentActor->action = MOVE;
                currentActor->velocity.x += (float) GetRandomValue(0, enemyVel);
            }

            if ((currentActor->collisionBox.x > player->position.x + 64) && currentActor->action == STOP) {
                currentActor->action = MOVE;
                currentActor->velocity.x -= (float) GetRandomValue(0, enemyVel);
            }

            if ((currentActor->collisionBox.y < player->position.y) && currentActor->action == STOP) {
                currentActor->action = MOVE;
                currentActor->velocity.y += (float) GetRandomValue(0, enemyVel);
            }
            if ((currentActor->collisionBox.y > player->position.y + 64) && currentActor->action == STOP) {
                currentActor->action = MOVE;
                currentActor->velocity.y -= (float) GetRandomValue(0, enemyVel);
            }

            // move action

            // obter o angulo do movimento
            currentActor->direction = (float) (atan2((double) currentActor->velocity.y,
                                                     (double) currentActor->velocity.x));
            currentActor->direction = (currentActor->direction * RAD2DEG) + 90.0f;

            // atualiza posição
            currentActor->position = Vector2Add(currentActor->position, currentActor->velocity);

            if (isInside(currentActor, &arena)) {
                currentActor->collisionBox.x = currentActor->position.x;
                currentActor->collisionBox.y = currentActor->position.y;
            } else {
                currentActor->position = (Vector2){currentActor->collisionBox.x, currentActor->collisionBox.y};
            }

            if (CheckCollisionRecs(player->collisionBox, currentActor->collisionBox)) {
                //PlaySound(getHit);
                currentActor->collision = true;
                currentActor->pointOfCollision = GetCollisionRec(player->collisionBox, currentActor->collisionBox);
                //player->action = SPECIAL;

                if (!debugMode) {
                    player->life--;
                }
            }
        }

        // DRAW
        BeginDrawing();

        //limpa tela
        ClearBackground(BLACK);

        //desenha mapa

        DrawRectangleRec(arena,DARKGRAY);

        if (debugMode) {
            drawMap();
        }

        //desenha itens
        drawItemList(itemListHead);

        //desenha inimigos
        drawActorList(enemyListHead);

        //desenha player
        drawActorList(playerListHead);


        DrawText(TextFormat("VIDA : %d", player->life), 20, 20, 20, BLACK);
        DrawText(TextFormat("PONTOS: %d", score), 20, 40, 20, BLACK);
        DrawText(TextFormat("NÍVEL: %d", level), 500, 20, 20, BLACK);

        // debug infor
        if (debugMode) {
            DrawText( TextFormat("Player->direction: %f", player->direction), 500, 40, 18, BLACK);
            DrawText( TextFormat("Player->Action: %d", player->action), 500, 60, 18, BLACK);
            DrawText( TextFormat("SpriteA->repeat: %d", player->spriteA2[player->action]->repeat), 500, 80, 18, BLACK);
            DrawText( TextFormat("Player Pos: x %d y %d", (int) player->position.x, (int) player->position.y ), 500, 100, 18, BLACK);
            DrawText( TextFormat("GriMap Pos: i %d j %d", (int) floorf(player->position.x/TILE_SIZE), (int) floorf(player->position.y/TILE_SIZE)), 500, 120, 18,BLACK);
            DrawText( TextFormat("Mouse: %d %d", GetMouseX(), GetMouseY()), 500, 140, 18, BLACK);

        }
        EndDrawing();


        UpdateMusicStream(bgMusic);
    }


    //LIBERAR MEMORIA PLAYER
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA PLAYER LIST");
    unloadActorList(playerListHead);


    //LIBERAR MEMORIA ENEMY LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ENEMY LIST");
    unloadActorList(enemyListHead);


    //LIBERAR MEMORIA ITEM LIST
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA ITEM LIST");
    for (ItemNode *temp = itemListHead, *prev = NULL; temp != NULL; prev = temp, temp = temp->next) {
        free(prev);
    }
    free(itemSpriteSheet);

    //LIBERAR MEMORIA MAPA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MAPA");

    //LIBERAR MEMORIA EFEITOS SONOROS
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA EFEITOS SONOROS");
    UnloadSound(eatCheese);
    UnloadSound(getHit);
    UnloadSound(eatStrawberry);

    //LIBERAR MEMORIA MUSICA
    TraceLog(LOG_DEBUG, "== LIBERAR MEMORIA MUSICA");
    StopMusicStream(bgMusic);
    UnloadMusicStream(bgMusic);

    volume = 0.5f;
    return 0;
}

//TODO centralizar intro e Gameove em uma mesma função passando como parâmetro uma flag
// cena de introdução
int gameIntro() {
    // CARREGAR MUSICA
    TraceLog(LOG_DEBUG, " == CARREGANDO MUSICA ");
    Music introMusic = LoadMusicStream("resources/sounds/intro_music.mp3");
    SetMusicVolume(introMusic, 1.0f);
    PlayMusicStream(introMusic);

    // CARREGAR IMAGENS DE FUNDO
    TraceLog(LOG_DEBUG, " == CARREGANDO IMAGENS");
    Texture2D backgroundTex = LoadTexture("resources/intro_bg.png");

    // SETAR TEXTOS
    TraceLog(LOG_DEBUG, " == CARREGANDO FONTES ");
    char *textIntro = "[ENTER] para jogar [ESC] para sair.";
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textIntro, FONT_SIZE, FONT_SPACE);
    Rectangle textRec = {(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};

    SetExitKey(KEY_ESCAPE);
    // INTRO LOOP
    TraceLog(LOG_DEBUG, " == CARREGAMENTO CONCLUÍDO ");
    while (gameScene == INTRO) {
        // INPUT HANDLE
        if (WindowShouldClose()) {
            gameScene = EXIT;
        }

        if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER) || IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            gameScene = GAME;
        }

        if (IsKeyReleased(KEY_KP_ADD)) {
            volume += 0.05f;
            SetMasterVolume(volume);
        }

        if (IsKeyReleased(KEY_KP_SUBTRACT)) {
            volume -= 0.05f;
            SetMasterVolume(volume);
        }

        //UPDATE
        UpdateMusicStream(introMusic);

        // DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(backgroundTex, 0, 0, WHITE);
        DrawRectangleRec(textRec, BLACK);
        DrawTextEx(gameFont, textIntro, (Vector2){textRec.x, textRec.y}, FONT_SIZE, FONT_SPACE, WHITE);
        EndDrawing();
    }

    // DESCARREGAR MUSICA
    StopMusicStream(introMusic);
    UnloadMusicStream(introMusic);

    // DESCARREGAR IMAGENS
    UnloadTexture(backgroundTex);

    return 0;
}




// carregar um objeto musica com tratamento de error
bool loadMusic(Music *music, const char *filepath) {
    *music = LoadMusicStream(filepath);
    if (music->stream.buffer == NULL) {
        TraceLog(LOG_ERROR, ":: ERRO AO CARREGAR A MUSICA");
        return false;
    }
    return true;
}


int main() {
    // Config Screen
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");

    InitAudioDevice();

    SetTraceLogLevel(LOG_DEBUG);

    TraceLog(LOG_DEBUG, "Iniciando raylib");

    SetTargetFPS(30);

    SetMasterVolume(volume);

    gameFont = GetFontDefault();

    gameScene = INTRO;

    // ReSharper disable once CppDFALoopConditionNotUpdated
    while (gameScene != EXIT) {
        if (gameScene == INTRO) {
            gameIntro();
        }

        if (gameScene == GAME) {
            gameLoop();
        }

        if (gameScene == OVER) {
            loopSceneOver();
        }
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

