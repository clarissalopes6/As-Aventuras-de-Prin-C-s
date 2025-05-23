#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "structs.h"
#include "raylib.h"

void carregarHighScores(HighScore scores[]);
int verificarHighScore(HighScore scores[], int score);
void inserirHighScore(HighScore scores[], int posicao, const char *nome, int score);
void mostrarHighScores(HighScore scores[]);
void salvarHighScores(HighScore scores[]);

#endif