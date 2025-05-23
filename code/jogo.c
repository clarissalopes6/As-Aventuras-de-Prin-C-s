#include "jogo.h"
#include "orbe.h"
#include <stdio.h>

void verificarColisoes(Princesa *princesa, int larguraTela, int alturaTela) {
    Vilao* v = princesa->viloes;
    while (v != NULL) {
        if (v->ativo && CheckCollisionCircles(princesa->posicao, princesa->largura / 2, v->posicao, princesa->larguraVilaoFraco / 2)) {
            v->ativo = false;
            princesa->vida--;
        }

        Ataque* a = princesa->ataques;
        while (a != NULL) {
            if (v->ativo && a->ativo && CheckCollisionCircles(a->posicao, princesa->larguraAtaque / 2, v->posicao, princesa->larguraVilaoFraco / 2)) {
                a->ativo = false;
                v->vida -= princesa->bonusDano;
                if (v->vida <= 0) {
                    v->ativo = false;
                    princesa->pontos++; 
                    criarOrbeXP(princesa, v->posicao);
                    criarOrbeVida(princesa, larguraTela, alturaTela);
                }
            }
            a = a->proximo;
        }
        v = v->proximo;
    }
}    

void desenharInterface(const Princesa *princesa) {
    DrawText(TextFormat("Pontos: %d", princesa->pontos), 10, 10, 20, GREEN);
    DrawText(TextFormat("Vidas: %d", princesa->vida), 10, 40, 20, RED);
    DrawText(TextFormat("Tempo: %02d:%02d", (int)princesa->tempoJogo / 60, (int)princesa->tempoJogo % 60), 10, 70, 20, YELLOW);
    DrawText(TextFormat("Nivel: %d", princesa->nivel), 10, 100, 20, BLUE);
    DrawText(TextFormat("XP: %d/%d", princesa->xpAtual, princesa->xpParaProximoNivel), 10, 130, 20, YELLOW);
}