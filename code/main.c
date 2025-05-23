#include "raylib.h"
#include "defines.h"
#include "structs.h"
#include "princesa.h"
#include "ataque.h"
#include "vilao.h"
#include "orbe.h"
#include "highscore.h"
#include "jogo.h"
#include <time.h>  
#include <stdio.h> 
#include <string.h>

int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "JOGO C");
    SetTargetFPS(60);

    Princesa princesa;
    iniciarPrincesa(&princesa, "assets/princesa.png", "assets/ataque.png", "assets/goblin_fraco.png", "assets/goblin_forte.png", "assets/fundo.png", "assets/xp_basica.png", "assets/orb_vida.png", larguraTela / 2, alturaTela / 2, larguraTela, alturaTela);

    double tempoInicio = GetTime();

    EstadoJogo estado = JOGANDO;

    HighScore highScores[MAX_SCORES];
    carregarHighScores(highScores);

    while (!WindowShouldClose()) {
        if (princesa.vida <= 0) break;
        
        BeginDrawing();

        if (estado == JOGANDO) {
            atualizarPrincesa(&princesa, larguraTela, alturaTela);
            atualizarAtaques(&princesa);
            atualizarViloes(&princesa, larguraTela, alturaTela);
            verificarColisoes(&princesa, larguraTela, alturaTela);
            atualizarOrbesVida(&princesa, larguraTela, alturaTela);
    
            if (atualizarOrbesXP(&princesa)) {
                estado = ESCOLHENDO_UPGRADE;
            }

            princesa.tempoJogo = GetTime() - tempoInicio;

            DrawTexture(princesa.fundo, 0, 0, WHITE);
            desenharPrincesa(&princesa);
            desenharAtaques(&princesa);
            desenharViloes(&princesa);
            desenharOrbesXP(&princesa);
            desenharOrbesVida(&princesa);
            desenharInterface(&princesa);
        } 
        else if (estado == ESCOLHENDO_UPGRADE) {
            ClearBackground(BLACK);
            DrawText("Escolha seu upgrade:", 200, 150, 30, WHITE);
            DrawText("1 - Mais velocidade de ataque", 200, 200, 20, GREEN);
            DrawText("2 - Maior dano (mata vilão com 1 tiro)", 200, 240, 20, RED);
            DrawText("3 - +1 Vida", 200, 280, 20, BLUE);
        
            if (IsKeyPressed(KEY_ONE)) {
                princesa.bonusVelocidadeAtaque *= 0.8f;
                estado = JOGANDO;
            } else if (IsKeyPressed(KEY_TWO)) {
                princesa.bonusDano++;
                estado = JOGANDO;
            } else if (IsKeyPressed(KEY_THREE)) {
                princesa.vida++;
                estado = JOGANDO;
            }
        }
        
        EndDrawing();
    }
    if (princesa.vida <= 0) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("GAME OVER", larguraTela/2 - MeasureText("GAME OVER", 40)/2, alturaTela/2 - 20, 40, RED);
        DrawText(TextFormat("Pontuacao final: %d", princesa.pontos), 
                larguraTela/2 - MeasureText(TextFormat("Pontuacao final: %d", princesa.pontos), 20)/2, 
                alturaTela/2 + 30, 20, WHITE);
        EndDrawing();
        WaitTime(3);

        int posicao = verificarHighScore(highScores, princesa.pontos);

        if (posicao != -1) {
            char nome[NOME_TAM] = {0};
            bool nomeCompleto = false;
            
            while (!nomeCompleto && !WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(BLACK);
                
                DrawText("NOVO HIGH SCORE!", 250, 100, 30, GREEN);
                DrawText(TextFormat("Sua pontuacao: %d", princesa.pontos), 250, 150, 20, WHITE);
                DrawText("Digite seu nome:", 250, 200, 20, WHITE);
                DrawText(nome, 250, 230, 20, YELLOW);
                DrawText("Pressione ENTER para confirmar", 250, 360, 20, GRAY);

                int key = GetCharPressed();
                while (key > 0 && strlen(nome) < NOME_TAM - 1) {
                    nome[strlen(nome)] = (char)key;
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    if (strlen(nome) > 0) {
                        nome[strlen(nome)-1] = '\0';
                    }
                }
                
                if (IsKeyPressed(KEY_ENTER)) {
                    nomeCompleto = true;
                }
                
                EndDrawing();
            }
            if (nomeCompleto) {
                inserirHighScore(highScores, posicao, nome, princesa.pontos);
                salvarHighScores(highScores);
            }
        }
        
        mostrarHighScores(highScores);
    }

    liberarPrincesa(&princesa);

    CloseWindow();

    return 0;
}