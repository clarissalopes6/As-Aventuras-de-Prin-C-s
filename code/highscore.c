#include "highscore.h"
#include <stdio.h>
#include <string.h>

void carregarHighScores(HighScore scores[]) {
    FILE *file = fopen("highscores.txt", "r");
    if (file) {
        fread(scores, sizeof(HighScore), MAX_SCORES, file);
        fclose(file);
    } else {
        for (int i = 0; i < MAX_SCORES; i++) {
            strcpy(scores[i].nome, "---");
            scores[i].score = 0;
        }
    }
}

void salvarHighScores(HighScore scores[]) {
    FILE *file = fopen("highscores.txt", "w");
    if (file) {
        fwrite(scores, sizeof(HighScore), MAX_SCORES, file);
        fclose(file);
    }
}

int verificarHighScore(HighScore scores[], int score) {
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i].score) {
            return i;
        }
    }
    return -1;
}

void inserirHighScore(HighScore scores[], int posicao, const char *nome, int score) {

    for (int i = MAX_SCORES - 1; i > posicao; i--) {
        scores[i] = scores[i - 1];
    }

    strncpy(scores[posicao].nome, nome, NOME_TAM - 1);
    scores[posicao].nome[NOME_TAM - 1] = '\0';
    scores[posicao].score = score;
}

void mostrarHighScores(HighScore scores[]) {
    bool sair = false; 

    while(!sair && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("HIGH SCORES", 250, 50, 40, YELLOW);
        
        for (int i = 0; i < MAX_SCORES; i++) {
            char texto[100];
            snprintf(texto, sizeof(texto), "%d. %s: %d", i+1, scores[i].nome, scores[i].score);
            DrawText(texto, 250, 120 + i * 40, 30, WHITE);
        }
        
        DrawText("Pressione ESC para sair", 200, 350, 20, GRAY);
        EndDrawing();
        
        if (IsKeyPressed(KEY_ESCAPE)) {
            sair = true;
        }
    }
}