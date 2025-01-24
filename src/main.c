#include <stdio.h>

/* TODO
 *   - REFATORAR. MODULARIZAR CENA GAME LOP
 *   - implementar MAP TILE para cenário.
 *   - salvar score em arquivo
 *   - IMPLEMENTAR STRUC DO MAPA
 *   - carregar mapa de arquivo
 *   - implementar uma load screen
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

SceneData* currentScene;
ScenesType currentSceneType = INTRO;
Font gameFont;
bool debugMode = false;
float volumeMaster = 1.01f, angulo;
int level, score;
Actor *currentActor = NULL;
Item *currentItem = NULL;
Texture2D *itemSpriteSheet;

typedef struct savefile{
    int score;
    int level;
}savefile;

// cena loop principal
int gameLoop() {



    // SETAR VARIÁVEIS DO LOOP
    TraceLog(LOG_DEBUG, "== definindo GLOBAL VARIABLES");
    Rectangle arena;
    volumeMaster = 1;
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
        currentSceneType = EXIT;
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
        currentSceneType = EXIT;
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

    if (!loadMusic(&bgMusic, "resources/sounds/game_music.mp3")) currentSceneType = EXIT;

    loadMap("resources/tileset.png");

    PlayMusicStream(bgMusic);
    SetMusicVolume(bgMusic, volumeMaster);
    SetExitKey(KEY_NULL);
    // GAME LOOP
    while (currentSceneType == GAME) {

        // INPUT HANDLE
        if (IsKeyPressed(KEY_ESCAPE)) {
            currentSceneType = INTRO;
            break;
        }

        if (WindowShouldClose() || player == NULL) {
            currentSceneType = EXIT;
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
            volumeMaster += 0.05f;
            SetMasterVolume(volumeMaster);
        }

        if (IsKeyReleased(KEY_KP_SUBTRACT)) {
            volumeMaster -= 0.05f;
            SetMasterVolume(volumeMaster);
        }

        // CONTROL PLAYER
        // Ação especial
        if (IsKeyReleased(KEY_SPACE)) {
            if (player->action == SPECIAL) player->action = STOP;
            else player->action = SPECIAL;
        }

        // Movimento
        if (player->action == MOVE || player->action == STOP) {

            player->action = STOP;

            // controle por teclado
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
                Vector2Distance(player->position, GetMousePosition()) > player->collisionBox.width
                ) {

                player->action = MOVE;
                player->direction = Vector2LineAngle(
                                        player->position,
                                        GetMousePosition()
                                    ) * RAD2DEG * -1.0f;
                player->direction += 90.0f;
            }
        }

        // UPDATE

        // Condições de vitória ou derrota.
        if (player->life < 1) {
            currentSceneType = OVER;
            continue;
        }

        // Verifica o nível atual
        if (score > level * 20) {
            enemyVel += 2;
            level++;
        }

        // ações do jogador
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
                TraceLog(LOG_DEBUG, "fim da linha");
                break;

            default:
                TraceLog(LOG_DEBUG, "item desconhecido");
                break;
        }

        // ------------------------------------------------------------
        // "ações" dos itens
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

        // ------------------------------------------------------------
        // "ações" dos INIMIGOS
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

        //DrawRectangleRec(arena,DARKGRAY);

        drawMap(player);


        //desenha itens
        drawItemList(itemListHead);

        //desenha inimigos
        drawActorList(enemyListHead);

        //desenha player
        drawActorList(playerListHead);


        DrawText(TextFormat("VIDA : %d", player->life), 20, 20, 20, BLACK);
        DrawText(TextFormat("PONTOS: %d", score), 20, 40, 20, BLACK);
        DrawText(TextFormat("NÍVEL: %d", level), 500, 20, 40, BLACK);

        DrawText(TextFormat("FPS: %d", GetFPS() ), 500, 570, 20, BLACK);

        // debug infor
        if (debugMode) {
            BeginBlendMode(BLEND_ALPHA);
            DrawRectangle(15, 450, 270, 140, ColorAlpha(SKYBLUE, 0.5f));
            DrawText( TextFormat("Player->direction: %f", player->direction), 20, 560, 18, BLACK);
            DrawText( TextFormat("Player->Action: %d", player->action), 20, 540, 18, BLACK);
            DrawText( TextFormat("SpriteA->repeat: %d", player->spriteA2[player->action]->repeat), 20, 520, 18, BLACK);
            DrawText( TextFormat("Player Pos: x %d y %d", (int) player->position.x, (int) player->position.y ), 20, 500, 18, BLACK);
            DrawText( TextFormat("GriMap Pos: i %d j %d", (int) floorf(player->position.x/TILE_SIZE), (int) floorf(player->position.y/TILE_SIZE)), 20, 480, 18,BLACK);
            DrawText( TextFormat("Mouse: %d %d", GetMouseX(), GetMouseY()), 20, 460, 18, BLACK);
            EndBlendMode();
        }
        EndDrawing();


        UpdateMusicStream(bgMusic);
    }




    //SaveFileData("score.dat", &score, sizeof(int));

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

    volumeMaster = 0.5f;


    // carregar pontuação salva
    FILE* file = fopen("score.dat", "rb");
    int top_5[5];

    if (file != NULL ) {
        fread(top_5, sizeof(int), 5, file);
    }

    fclose(file);

    // verifica os recordes
    for (int i = 0; i < 5; i++) {
        // o score é um novo record?
        if (score > top_5[i]) {
            // reordena os recordes anteriores
            for (int j = 4; j > i; j--) {
                top_5[j] = top_5[j-1];
            }
            //insere o novo recorde
            top_5[i] = score;

            break;
        }
    }
     // salva o novo top 5
    file = fopen("score.dat","wb");
    if (file == NULL) {
        TraceLog(LOG_ERROR, "Erro ao abrir arquivo");
        return;
    }

    fwrite(top_5, sizeof(top_5), 1, file);

    fclose(file);

    return 0;
}

//----------------------------------------------------------

/**
 * @brief Initializes a scene of the specified type by setting up its required resources and logic.
 *
 * This function determines the necessary initialization routine for the given scene type. It delegates
 * the initialization to the appropriate function based on the specified scene type, ensuring that the
 * corresponding scene is properly prepared for execution.
 *
 * @param sceneType The type of scene to initialize. Valid options include INTRO and OVER.
 */
void initializeScene(ScenesType sceneType) {
    if (sceneType == INTRO) initSceneIntro();
    else if (sceneType == OVER) initSceneOver();
}

/**
 * @brief Manages the current scene's lifecycle, including initialization, execution, and cleanup.
 *
 * This function handles the full lifecycle of the active scene based on the current scene type.
 * It initializes the scene, prepares its music stream for playback, and adjusts the music volume
 * according to the master volume. The function then executes the main loop of the scene, which
 * processes its logic and rendering. Once the scene's loop concludes, it performs cleanup by
 * unloading scene resources and deallocating any associated memory to prevent leaks.
 */
void manageScene() {
    // Inicializa a cena conforme o tipo atual
    initializeScene(currentSceneType);

    // Prepara a música para ser tocada
    PlayMusicStream(currentScene->music);
    SetMusicVolume(currentScene->music, volumeMaster);

    // Executa o loop principal da cena
    runSceneLoop();

    // Finaliza a cena
    unloadScene(currentScene);

    // Libera memória associada à cena
    if (currentScene != NULL) {
        free(currentScene);
        currentScene = NULL;
    }
}

/**
 * @brief Executes the loop for the active scene, handling its logic and rendering.
 *
 * This function continuously processes the logic and rendering of the current scene
 * until the type of the scene changes, indicating a transition. It handles user input,
 * updates the scene's music stream to ensure smooth audio playback, and renders the
 * scene's graphical elements. The loop only exits once the scene type is updated,
 * signaling a transition to a new scene.
 */
void runSceneLoop() {
    while (currentSceneType == currentScene->sceneType) {
        sceneInputHandler(currentScene);
        UpdateMusicStream(currentScene->music);
        drawScene(currentScene);
    }
}

/**
 * @brief Executes the primary loop controlling the transitions between scenes in the application.
 *
 * This function manages the flow of the application's main logic by continuously checking
 * the current scene type and performing actions based on the scene. It transitions between
 * game scenes (GAME, OVER, etc.) and handles scene-specific logic until the application exits.
 */
void playMainLoop() {
    while (currentSceneType != EXIT) {
        if (currentSceneType == GAME) {
            gameLoop();
            currentSceneType = OVER;
        } else {
            manageScene();
        }
    }
}

/**
 * @brief Configures the display settings for the application.
 *
 * This function initializes the application window with the specified dimensions and title,
 * and sets the target frames per second (FPS) for rendering. It prepares the screen for rendering
 * and overall graphical display.
 */
void configureScreen() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CatAndMouse by Handerson Gleber (Gr4v4t1nh4)");
    SetTargetFPS(30);
}


/**
 * @brief Configures the audio system and sets the master volume level.
 *
 * This function initializes the audio device to enable audio playback and adjusts
 * the master volume to the specified level, preparing the audio system for use.
 *
 * @param masterVolume The desired master volume level, a float value typically between 0.0 (silent) and 1.0 (maximum volume).
 */
void configureAudio(float masterVolume) {
    InitAudioDevice();
    SetMasterVolume(masterVolume);
}


/**
 * @brief Initializes the game resources and sets the initial game state.
 *
 * This function configures the game screen and audio settings, using the global
 * variable volumeMaster to set the initial audio volume. It also sets the debugging
 * log level, initializes the game font to the default font provided by the framework,
 * and defines the initial scene type as INTRO. This prepares the game for execution.
 */
void initGame() {
    configureScreen();
    configureAudio(volumeMaster);  // Using global variable volumeMaster

    SetTraceLogLevel(LOG_DEBUG);   // Set debugging level for logs
    gameFont = GetFontDefault();   // Use the default font
    currentSceneType = INTRO;      // Start with the INTRO scene
}


/**
 * @brief Unloads game resources and closes the application.
 *
 * This function ensures that all game resources are released properly by
 * closing the audio device and shutting down the game window. It prevents
 * resource leaks and ensures a clean exit from the application.
 */
void unloadGame() {
    CloseAudioDevice();
    CloseWindow();
}


/**
 * @brief Entry point of the application.
 *
 * Initializes the game, executes the main game loop, and releases all resources.
 * This function is responsible for managing the lifecycle of the game, ensuring
 * proper initialization, execution, and cleanup of the game processes.
 *
 * @return An integer value, typically 0, indicating the successful execution of the program.
 */
int main() {
    initGame();          // Inicializa o estado e os recursos do jogo
    playMainLoop();      // Executa o loop principal do jogo
    unloadGame();        // Libera os recursos utilizados no jogo
    return 0;            // Retorna 0 para indicar execução bem-sucedida
}

