#include "scene.h"
#include "scene_intro.h"
#include "scene_over.h"
#include "scene_game.h"
#include "main.h"

/**
 * @brief Inicializa a cena com base no tipo fornecido.
 * 
 * Esta função inicializa a cena apropriada com base no tipo de cena fornecido.
 * 
 * @param sceneType O tipo de cena a ser inicializada.
 * @return SceneData* Retorna um ponteiro para os dados da cena inicializada.
 */
SceneData* initScene(ScenesType sceneType){

    SceneData* temp;

    switch (sceneType ) {
        case INTRO:
            temp = initSceneIntro();
            break;
        case OVER:
            temp = initSceneOver();
            break;
        default:
            temp = NULL;
            break;
    }
    return temp;
}


/**
 * @brief Processa eventos de entrada para a cena atual.
 * 
 * Esta função lida com eventos de entrada, como fechamento da janela e teclas de depuração,
 * e chama a função de entrada específica da cena.
 * 
 * @param scene A cena atual para processar eventos de entrada.
 */
void inputScene(SceneData *scene) {

    if (WindowShouldClose()) {
        currentSceneType = EXIT;
        return;
    }

    // Opções de depuração
    if (IsKeyReleased(KEY_F1)) {
        debugMode = !debugMode;
    }

    if (IsKeyReleased(KEY_KP_ADD)) {
        volumeMaster += 0.05f;
        SetMasterVolume(volumeMaster);
    }

    if (IsKeyReleased(KEY_KP_SUBTRACT)) {
        volumeMaster -= 0.05f;
        SetMasterVolume(volumeMaster);
    }

    scene->input();
}

/**
 * @brief Atualiza a lógica da cena atual.
 * 
 * Esta função atualiza a lógica da cena atual, incluindo a atualização do fluxo de música
 * e a chamada da função de atualização específica da cena.
 * 
 * @param scene A cena atual para atualizar.
 */
void updateScene(SceneData *scene) {
    UpdateMusicStream(scene->music);
    scene->update();
}

/**
 * @brief Desenha a cena atual na tela.
 * 
 * Esta função chama a função de desenho específica da cena para renderizar a cena na tela.
 * 
 * @param scene A cena atual para desenhar.
 */
void drawScene(SceneData *scene) {
    scene->draw();
}

/**
 * @brief Executa o loop principal da cena atual.
 * 
 * Esta função executa o loop principal da cena, incluindo a entrada, atualização e desenho da cena.
 * 
 * @param scene A cena atual para executar.
 */
void loopScene(SceneData *scene) {

    // Preparação da cena
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);
    TraceLog(LOG_DEBUG, TextFormat(" == Play music " ));
    
    // Loop da cena
    while (currentSceneType == scene->type) {

        inputScene(scene);

        updateScene(scene);

        drawScene(scene);

    }
}

/**
 * @brief Executa a cena com base no tipo fornecido.
 * 
 * Esta função inicializa, executa e finaliza a cena com base no tipo de cena fornecido.
 * 
 * @param sceneType O tipo de cena a ser executada.
 */
void runScene(ScenesType sceneType) {

    // Preparando a cena
    TraceLog(LOG_DEBUG, " == INICIANDO SCENE ");
    SceneData* scene = initScene(sceneType);
    currentScene = scene;
        
    // Prepara a música para ser tocada
    PlayMusicStream(scene->music);
    SetMusicVolume(scene->music, volumeMaster);

    // Executa o loop principal da cena
    TraceLog(LOG_DEBUG, " == SCENE LOOP ");
    loopScene(scene);

    // Finaliza a cena
    TraceLog(LOG_DEBUG, " == FINALIZANDO SCENE  ");
    StopMusicStream(scene->music);

    unloadScene(scene);

}



/**
 * @brief Descarrega os recursos de uma cena e libera a memória associada.
 * 
 * Esta função libera a memória alocada para a estrutura SceneData e descarrega
 * os recursos associados, como a música e a textura de fundo.
 * 
 * @param scene Ponteiro para a estrutura SceneData a ser descarregada. Se for NULL,
 * a função não faz nada.
 */
void unloadScene(SceneData* scene) {
    if (scene != NULL) {
        TraceLog(LOG_DEBUG, " == Liberando Scene %p ", scene);
        UnloadMusicStream(scene->music);
        UnloadTexture(scene->background);
        free(scene);
    }
}


/**
 * @brief Carrega os dados da cena a partir dos caminhos fornecidos.
 * 
 * Esta função carrega a textura de fundo e a música da cena a partir dos caminhos fornecidos,
 * e inicializa a estrutura SceneData.
 * 
 * @param backgroundPath O caminho para a textura de fundo.
 * @param musicPath O caminho para o arquivo de música.
 * @param type O tipo de cena.
 * @param font A fonte a ser utilizada.
 * @return SceneData* Retorna um ponteiro para os dados da cena carregada.
 */
SceneData* loadSceneData(const char* backgroundPath, const char* musicPath, ScenesType type, Font* font) {

    TraceLog(LOG_DEBUG, TextFormat(" == Load Scene Data %d bytes ", sizeof(SceneData) ));
    SceneData* scene = malloc( sizeof(SceneData) );

    if (scene == NULL) {
        TraceLog(LOG_ERROR, "  !!  error ao carregar SCENE !!");
        return NULL;
    }
    
    // Carregar música
    scene->music = LoadMusicStream(musicPath);
    
    if (scene->music.stream.buffer == NULL) {
        TraceLog(LOG_ERROR, "  !!  error ao carregar MUSIC !!");
        //free(scene);
        //return NULL;
    }

    // Carregar textura de fundo
    scene->background = LoadTexture(backgroundPath);

    if (scene->background.id == 0) {
        TraceLog(LOG_ERROR, "  !!  error ao carregar BACKGROUND !!");
        UnloadMusicStream(scene->music);
        //free(scene);
        //return NULL;
    }

    scene->font = font;
    scene->type = type;
    scene->nextScene = OVER;

    SetMusicVolume(scene->music, 0.4f);

    return scene;
}


/**
 * @brief Retorna um retângulo com as dimensões do texto.
 * 
 * Esta função calcula as dimensões do texto renderizado com a fonte e tamanho fornecidos,
 * e retorna um retângulo que centraliza o texto na tela.
 * 
 * @param text O texto a ser medido.
 * @param font A fonte a ser utilizada.
 * @param fontSize O tamanho da fonte.
 * @param space O espaço entre as letras.
 * @return Rectangle Retorna um retângulo com as dimensões do texto.
 */
Rectangle getTextRect(const char *text, Font font, const float fontSize, const float space) {
    const Vector2 textMeasure = MeasureTextEx(font, text, fontSize, space);
    return (Rectangle){(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};
}
