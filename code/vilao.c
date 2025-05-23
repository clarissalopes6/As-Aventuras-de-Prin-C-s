#include "vilao.h"
#include "raymath.h"
#include <stdlib.h>

void atualizarViloes(Princesa* princesa, int larguraTela, int alturaTela) {
    if (GetRandomValue(0, 100) < (1.5 + princesa->nivel)) {
        Vilao* novo = (Vilao*)malloc(sizeof(Vilao));
        int lado = GetRandomValue(0, 3); 

        if (lado == 0) {
            novo->posicao = (Vector2){ GetRandomValue(0, larguraTela), 0 };
        } else if (lado == 1) {
            novo->posicao = (Vector2){ GetRandomValue(0, larguraTela), alturaTela };
        } else if (lado == 2) { 
            novo->posicao = (Vector2){ 0, GetRandomValue(0, alturaTela) };
        } else { 
            novo->posicao = (Vector2){ larguraTela, GetRandomValue(0, alturaTela) };
        }

        if (GetRandomValue(0, 100) < 10) {
            novo->vida = 3;
        } else {
            novo->vida = 1;
        }
        
        novo->ativo = true;
        novo->proximo = princesa->viloes;
        princesa->viloes = novo;
        princesa->viloesCriadosNoNivel++;
    }

    Vilao* atual = princesa->viloes;
    while (atual != NULL) {
        if (atual->ativo) {
            Vector2 direcao = Vector2Normalize(Vector2Subtract(princesa->posicao, atual->posicao));
            atual->posicao = Vector2Add(atual->posicao, Vector2Scale(direcao, 1.5));
            if (atual->posicao.x < 0) atual->posicao.x = 0;
            if (atual->posicao.x > larguraTela) atual->posicao.x = larguraTela;
            if (atual->posicao.y < 0) atual->posicao.y = 0;
            if (atual->posicao.y > alturaTela) atual->posicao.y = alturaTela;
        }
        atual = atual->proximo;
    }
}

void desenharViloes(const Princesa* princesa) {
    Vilao* atual = princesa->viloes;
    while (atual != NULL) {
        if (atual->ativo) {
            if (atual->vida > 1) {
                DrawTexture(princesa->texturaVilaoForte, 
                          atual->posicao.x - princesa->larguraVilaoForte / 2, 
                          atual->posicao.y - princesa->alturaVilaoForte / 2, WHITE);
            } else {
                DrawTexture(princesa->texturaVilaoFraco, 
                          atual->posicao.x - princesa->larguraVilaoFraco / 2, 
                          atual->posicao.y - princesa->alturaVilaoFraco / 2, WHITE);
            }
        }
        atual = atual->proximo;
    }
}

void liberarViloes(Vilao **listaViloes) {
    Vilao *atual = *listaViloes;
    Vilao *proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *listaViloes = NULL;
}