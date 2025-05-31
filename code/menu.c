#include "raylib.h"
#include "menu.h"

static int screenWidth = 800;
static int screenHeight = 600;

static Texture2D background;
static Rectangle playButton;
static Rectangle scoresButton;

static int finishScreen = 0;

void InitMenuScreen(void) {
    finishScreen = 0;

    background = LoadTexture("assets/fundo menu.png");

    playButton = (Rectangle){ screenWidth/2.0f - 120, screenHeight/2.0f - 40, 240, 50 };
    scoresButton = (Rectangle){ screenWidth/2.0f - 120, screenHeight/2.0f + 30, 240, 50 };
}

void UpdateMenuScreen(void) {
    Vector2 mousePoint = GetMousePosition();

    if (CheckCollisionPointRec(mousePoint, playButton)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            finishScreen = 1;
        }
    }

    if (CheckCollisionPointRec(mousePoint, scoresButton)) {
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            finishScreen = 2;
        }
    }
}

void DrawMenuScreen(void) {
    BeginDrawing();

        ClearBackground(RAYWHITE);

        Rectangle sourceRec = { .x = 0, .y = 0, .width = (float)background.width, .height = (float)background.height };
        Rectangle destRec = { .x = 0, .y = 0, .width = (float)screenWidth, .height = (float)screenHeight };

        DrawTexturePro(background, sourceRec, destRec, (Vector2){ 0, 0 }, 0.0f, WHITE);

        DrawRectangleRec(playButton, Fade(SKYBLUE, 0.7f));
        DrawRectangleRec(scoresButton, Fade(SKYBLUE, 0.7f));

        if (CheckCollisionPointRec(GetMousePosition(), playButton)) DrawRectangleLinesEx(playButton, 3, MAROON);
        if (CheckCollisionPointRec(GetMousePosition(), scoresButton)) DrawRectangleLinesEx(scoresButton, 3, MAROON);

        DrawText("JOGAR", playButton.x + 75, playButton.y + 15, 30, DARKBLUE);
        DrawText("SCORES", scoresButton.x + 65, scoresButton.y + 15, 30, DARKBLUE);

    EndDrawing();
}

void UnloadMenuScreen(void) {
    UnloadTexture(background);
}

int FinishMenuScreen(void) {
    return finishScreen;
}