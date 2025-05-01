#include "raylib.h"

int main(void) {
    
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int targetFPS = 60;

    InitWindow(screenWidth, screenHeight, "Jogo PIF");
    SetTargetFPS(targetFPS);

    Texture2D background = LoadTexture("assets/fundo.png");
    Texture2D jogador = LoadTexture("assets/princess.png"); 
    
    Vector2 jogadorPos = {
        (float)(screenWidth / 2 - jogador.width / 2),
        (float)(screenHeight / 2 - jogador.height / 2)
    };
    float jogadorSpeed = 5.0f;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) jogadorPos.y -= jogadorSpeed;
        if (IsKeyDown(KEY_S)) jogadorPos.y += jogadorSpeed;
        if (IsKeyDown(KEY_A)) jogadorPos.x -= jogadorSpeed;
        if (IsKeyDown(KEY_D)) jogadorPos.x += jogadorSpeed;

        if (jogadorPos.x < 0) jogadorPos.x = 0;
        if (jogadorPos.y < 0) jogadorPos.y = 0;
        if (jogadorPos.x > screenWidth - jogador.width) jogadorPos.x = screenWidth - jogador.width;
        if (jogadorPos.y > screenHeight - jogador.height) jogadorPos.y = screenHeight - jogador.height;

        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawTexture(background, 0, 0, WHITE);

            DrawTextureV(jogador, jogadorPos, WHITE);

            DrawFPS(10, 10);
        EndDrawing();
    }
    UnloadTexture(background);
    UnloadTexture(jogador);
    CloseWindow();

    return 0;
}