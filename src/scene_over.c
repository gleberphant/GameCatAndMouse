#include "scene_over.h"
#include "main.h"

Music gameoverMusic;
Texture2D backgroundTex;
//Font gameFont;
char *textOver = "GAME OVER";
Rectangle textRec;

void initSceneOver() {

    TraceLog(LOG_DEBUG, " == CARREGANDO IMAGENS ==");
    backgroundTex = LoadTexture("resources/gameover.png");

    TraceLog(LOG_DEBUG, " == CARREGANDO TEXTOS == ");
    gameFont = GetFontDefault();
    Vector2 textMeasure = MeasureTextEx(gameFont, textOver, FONT_SIZE, FONT_SPACE);
    textRec = (Rectangle){(SCREEN_WIDTH - textMeasure.x) / 2, 400, textMeasure.x, textMeasure.y};

    TraceLog(LOG_DEBUG, " == CARREGANDO MUSICA == ");
    gameoverMusic = LoadMusicStream("resources/sounds/gameover.mp3");
    SetMusicVolume(gameoverMusic, volume);
    PlayMusicStream(gameoverMusic);
}

void closeSceneOver() {
    // DESCARREGAR MUSICA
    StopMusicStream(gameoverMusic);
    UnloadMusicStream(gameoverMusic);

    // DESCARREGAR IMAGENS
    UnloadTexture(backgroundTex);
}


void inputSceneOver() {
    //INPUT HANDLE
    if (IsKeyReleased(KEY_ESCAPE) || WindowShouldClose()) {
        gameScene = EXIT;
    }

    if (IsKeyReleased(KEY_ENTER) || IsKeyReleased(KEY_KP_ENTER)) {
        gameScene = INTRO;
    }
}

void drawSceneOver() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(backgroundTex, 0, 0, WHITE);
    DrawRectangleRec(textRec, BLACK);
    DrawTextEx(gameFont, textOver, (Vector2){textRec.x, textRec.y}, FONT_SIZE, FONT_SPACE, WHITE);
    EndDrawing();
}

// cena de game over
void loopSceneOver() {

    initSceneOver();

    while (gameScene == OVER) {
        inputSceneOver();

        UpdateMusicStream(gameoverMusic);

        drawSceneOver();
    }

    closeSceneOver();
}
