#include "raylib.h"
#include "defines.h"
#include "structs.h"
#include "princesa.h"
#include "ataque.h"
#include "vilao.h"
#include "orbe.h"
#include "highscore.h"
#include "jogo.h"
#include "menu.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef enum TelaJogo {
    TELA_MENU,
    TELA_JOGANDO,
    TELA_UPGRADE,
    TELA_GAME_OVER,
    TELA_INSERIR_HIGHSCORE,
    TELA_MOSTRAR_HIGHSCORE
} TelaJogo;

void IniciarJogo(Princesa *princesa, const int larguraTela, const int alturaTela, Sound somDanoPrincesa);
void desenharHighScores(HighScore scores[], int larguraTela, int alturaTela);

int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "As Aventuras de Prin-C-s");
    InitAudioDevice();

    Music musicaMenu = LoadMusicStream("assets/msc_menu.ogg");
    Music musicaJogo = LoadMusicStream("assets/msc_jogo.ogg");
    Sound somUpgrade = LoadSound("assets/som_upgrade.wav");
    Sound somDanoPrincesa = LoadSound("assets/dano_princesa.ogg");

    SetSoundVolume(somDanoPrincesa, 0.05f);

    SetTargetFPS(60);

    PlayMusicStream(musicaMenu);
    SetMusicVolume(musicaMenu, 0.05f);
    SetMusicVolume(musicaJogo, 0.05f);

    TelaJogo telaAtual = TELA_MENU;
    Princesa princesa;
    HighScore highScores[MAX_SCORES];
    carregarHighScores(highScores);

    const int buttonWidth = 450;
    const int buttonHeight = 40;
    const int buttonX = (larguraTela - buttonWidth) / 2;
    Rectangle upgradeButton1 = { buttonX, 200, buttonWidth, buttonHeight };
    Rectangle upgradeButton2 = { buttonX, 260, buttonWidth, buttonHeight };
    Rectangle upgradeButton3 = { buttonX, 320, buttonWidth, buttonHeight };

    double tempoInicio;
    int posicaoHighScore = -1;
    char nomeJogador[NOME_TAM] = {0};
    int framesCounter = 0;

    InitMenuScreen();

    while (!WindowShouldClose()) {
        UpdateMusicStream(musicaMenu);
        UpdateMusicStream(musicaJogo);

        switch (telaAtual) {
            case TELA_MENU:
                UpdateMenuScreen();
                if (FinishMenuScreen() == 1) {
                    StopMusicStream(musicaMenu);
                    PlayMusicStream(musicaJogo);
                    UnloadMenuScreen();
                    IniciarJogo(&princesa, larguraTela, alturaTela, somDanoPrincesa);
                    tempoInicio = GetTime();
                    telaAtual = TELA_JOGANDO;
                } else if (FinishMenuScreen() == 2) {
                    UnloadMenuScreen();
                    telaAtual = TELA_MOSTRAR_HIGHSCORE;
                }
                break;
            case TELA_JOGANDO:
                atualizarPrincesa(&princesa, larguraTela, alturaTela);
                atualizarAtaques(&princesa);
                atualizarViloes(&princesa, larguraTela, alturaTela);
                verificarColisoes(&princesa, larguraTela, alturaTela);
                atualizarOrbesVida(&princesa, larguraTela, alturaTela);
                princesa.tempoJogo = GetTime() - tempoInicio;
                if (atualizarOrbesXP(&princesa)) {
                    telaAtual = TELA_UPGRADE;
                }
                if (princesa.vida <= 0) {
                    framesCounter = 0;
                    telaAtual = TELA_GAME_OVER;
                }
                break;
            case TELA_UPGRADE:
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mousePoint = GetMousePosition();
                    bool upgradeEscolhido = false;
                    if (CheckCollisionPointRec(mousePoint, upgradeButton1)) {
                        princesa.bonusVelocidadeAtaque *= 0.8f;
                        upgradeEscolhido = true;
                    } else if (CheckCollisionPointRec(mousePoint, upgradeButton2)) {
                        princesa.bonusDano++;
                        upgradeEscolhido = true;
                    } else if (CheckCollisionPointRec(mousePoint, upgradeButton3)) {
                        princesa.vida++;
                        upgradeEscolhido = true;
                    }
                    if (upgradeEscolhido) {
                        PlaySound(somUpgrade);
                        telaAtual = TELA_JOGANDO;
                    }
                }
                break;
            case TELA_GAME_OVER:
                framesCounter++;
                if (framesCounter > 180) {
                    posicaoHighScore = verificarHighScore(highScores, princesa.pontos);
                    if (posicaoHighScore != -1) {
                        telaAtual = TELA_INSERIR_HIGHSCORE;
                    } else {
                        telaAtual = TELA_MOSTRAR_HIGHSCORE;
                    }
                }
                break;
            case TELA_INSERIR_HIGHSCORE:
                {
                    int key = GetCharPressed();
                    while (key > 0 && strlen(nomeJogador) < NOME_TAM - 1) {
                        nomeJogador[strlen(nomeJogador)] = (char)key;
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && strlen(nomeJogador) > 0) {
                        nomeJogador[strlen(nomeJogador) - 1] = '\0';
                    }
                    if (IsKeyPressed(KEY_ENTER)) {
                        inserirHighScore(highScores, posicaoHighScore, nomeJogador, princesa.pontos);
                        salvarHighScores(highScores);
                        memset(nomeJogador, 0, NOME_TAM);
                        telaAtual = TELA_MOSTRAR_HIGHSCORE;
                    }
                }
                break;
            case TELA_MOSTRAR_HIGHSCORE:
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                    StopMusicStream(musicaJogo);
                    PlayMusicStream(musicaMenu);
                    InitMenuScreen();
                    telaAtual = TELA_MENU;
                }
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (telaAtual) {
            case TELA_MENU:
                DrawMenuScreen();
                break;
            case TELA_JOGANDO:
                DrawTexture(princesa.fundo, 0, 0, WHITE);
                desenharPrincesa(&princesa);
                desenharAtaques(&princesa);
                desenharViloes(&princesa);
                desenharOrbesXP(&princesa);
                desenharOrbesVida(&princesa);
                desenharInterface(&princesa);
                break;
            case TELA_UPGRADE:
                DrawTexture(princesa.fundo, 0, 0, WHITE);
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.7f));
                DrawText("Escolha seu upgrade!", larguraTela / 2 - MeasureText("Escolha seu upgrade!", 30) / 2, 130, 30, GOLD);

                Vector2 mousePoint = GetMousePosition();

                DrawRectangleRec(upgradeButton1, Fade(SKYBLUE, 0.6f));
                if (CheckCollisionPointRec(mousePoint, upgradeButton1)) DrawRectangleLinesEx(upgradeButton1, 3, MAROON);
                DrawText("Mais velocidade de ataque", upgradeButton1.x + 20, upgradeButton1.y + 10, 20, WHITE);

                DrawRectangleRec(upgradeButton2, Fade(SKYBLUE, 0.6f));
                if (CheckCollisionPointRec(mousePoint, upgradeButton2)) DrawRectangleLinesEx(upgradeButton2, 3, MAROON);
                DrawText("Maior dano (mata vilão com 1 tiro)", upgradeButton2.x + 20, upgradeButton2.y + 10, 20, WHITE);

                DrawRectangleRec(upgradeButton3, Fade(SKYBLUE, 0.6f));
                if (CheckCollisionPointRec(mousePoint, upgradeButton3)) DrawRectangleLinesEx(upgradeButton3, 3, MAROON);
                DrawText("+1 Vida", upgradeButton3.x + 20, upgradeButton3.y + 10, 20, WHITE);
                break;
            case TELA_GAME_OVER:
                DrawTexture(princesa.fundo, 0, 0, WHITE);
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.8f));
                DrawText("GAME OVER", larguraTela / 2 - MeasureText("GAME OVER", 40) / 2, alturaTela / 2 - 20, 40, RED);
                break;
            case TELA_INSERIR_HIGHSCORE:
                DrawText("NOVO HIGH SCORE!", 250, 100, 30, GREEN);
                DrawText(TextFormat("Sua pontuacao: %d", princesa.pontos), 250, 150, 20, WHITE);
                DrawText("Digite seu nome:", 250, 200, 20, WHITE);
                DrawRectangle(245, 225, 310, 30, Fade(LIGHTGRAY, 0.5f));
                DrawText(nomeJogador, 250, 230, 20, YELLOW);
                DrawText("Pressione ENTER para confirmar", 250, 360, 20, GRAY);
                break;
            case TELA_MOSTRAR_HIGHSCORE:
                desenharHighScores(highScores, larguraTela, alturaTela);
                break;
        }
        EndDrawing();
    }

    UnloadSound(somUpgrade);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaJogo);
    UnloadSound(somDanoPrincesa); 
    CloseAudioDevice();

    liberarPrincesa(&princesa);
    CloseWindow();

    return 0;
}

void IniciarJogo(Princesa *princesa, const int larguraTela, const int alturaTela, Sound somDanoPrincesa) {
    iniciarPrincesa(princesa, "assets/princesa.png", "assets/ataque.png", "assets/goblin_fraco.png", "assets/goblin_forte.png", "assets/fundo.png", "assets/xp_basica.png", "assets/orb_vida.png", larguraTela / 2, alturaTela / 2, larguraTela, alturaTela, somDanoPrincesa);
}

void desenharHighScores(HighScore scores[], int larguraTela, int alturaTela) {
    DrawText("HIGH SCORES", larguraTela / 2 - MeasureText("HIGH SCORES", 40) / 2, 80, 40, GOLD);
    for (int i = 0; i < MAX_SCORES; i++) {
        if (scores[i].score > 0) {
            DrawText(TextFormat("%d. %s", i + 1, scores[i].nome), 250, 150 + i * 30, 20, LIGHTGRAY);
            DrawText(TextFormat("%d", scores[i].score), 500, 150 + i * 30, 20, LIGHTGRAY);
        } else {
            DrawText(TextFormat("%d. ...", i + 1), 250, 150 + i * 30, 20, DARKGRAY);
        }
    }
    DrawText("Pressione ENTER para voltar ao Menu", larguraTela / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, alturaTela - 50, 20, GRAY);
}
