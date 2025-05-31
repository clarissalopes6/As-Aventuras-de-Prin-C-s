#include "princesa.h"
#include "ataque.h"
#include "vilao.h"
#include "orbe.h"
#include "raymath.h"
#include <stdlib.h>
#include <float.h>

void iniciarPrincesa(Princesa *princesa, const char* imgPrincesa, const char* imgAtaque, 
    const char* imgVilaoFraco, const char* imgVilaoForte, const char* imgFundo, const char* imgOrbeXP, const char* imgOrbeVida, int x, int y, int larguraDoJogo, int alturaDoJogo, Sound somDano) {
    princesa->textura = LoadTexture(imgPrincesa);
    princesa->texturaAtaque = LoadTexture(imgAtaque);
    princesa->texturaVilaoFraco = LoadTexture(imgVilaoFraco);
    princesa->texturaVilaoForte = LoadTexture(imgVilaoForte);
    princesa->fundo = LoadTexture(imgFundo);
    princesa->texturaOrbeXP = LoadTexture(imgOrbeXP);
    princesa->texturaOrbeVida = LoadTexture(imgOrbeVida);
    princesa->largura = princesa->textura.width;
    princesa->altura = princesa->textura.height;
    princesa->larguraAtaque = princesa->texturaAtaque.width;
    princesa->alturaAtaque = princesa->texturaAtaque.height;
    princesa->larguraVilaoFraco = princesa->texturaVilaoFraco.width;
    princesa->alturaVilaoFraco = princesa->texturaVilaoFraco.height;
    princesa->larguraVilaoForte = princesa->texturaVilaoForte.width;
    princesa->alturaVilaoForte = princesa->texturaVilaoForte.height;
    princesa->posicao = (Vector2){x, y};
    princesa->velocidade = 5;
    princesa->vida = 3;
    princesa->pontos = 0;
    princesa->acertos = 0;
    princesa->ataques = NULL;
    princesa->viloes = NULL;
    princesa->tempoJogo = 0.0;
    princesa->nivel = 1;
    princesa->inimigosPorNivel = 3;
    princesa->tempoUltimoAtaque = 0.0f;
    princesa->viloesCriadosNoNivel = 0;
    princesa->bonusVelocidadeAtaque = 1.0f;
    princesa->bonusDano = 1;
    princesa->vidasExtras = 0;
    princesa->orbesXP = NULL;
    princesa->orbesVida = NULL;
    princesa->xpAtual = 0;
    princesa->xpParaProximoNivel = 10;
    princesa->somDano = somDano;
    
    princesa->numColunas = larguraDoJogo / TAMANHO_CELULA;
    princesa->numLinhas = alturaDoJogo / TAMANHO_CELULA;

    for (int i = 0; i < princesa->numLinhas; i++) {
        for (int j = 0; j < princesa->numColunas; j++) {
            princesa->matrizDeSpawn[i][j] = CELULA_LIVRE;
        }
    }
}

void atualizarPrincesa(Princesa *princesa, int larguraTela, int alturaTela) {
    if (IsKeyDown(KEY_UP)) princesa->posicao.y -= princesa->velocidade;
    if (IsKeyDown(KEY_DOWN)) princesa->posicao.y += princesa->velocidade;
    if (IsKeyDown(KEY_LEFT)) princesa->posicao.x -= princesa->velocidade;
    if (IsKeyDown(KEY_RIGHT)) princesa->posicao.x += princesa->velocidade;

    if (IsKeyDown(KEY_W)) princesa->posicao.y -= princesa->velocidade;
    if (IsKeyDown(KEY_S)) princesa->posicao.y += princesa->velocidade;
    if (IsKeyDown(KEY_A)) princesa->posicao.x -= princesa->velocidade;
    if (IsKeyDown(KEY_D)) princesa->posicao.x += princesa->velocidade;

    if (princesa->posicao.x < princesa->largura / 2) princesa->posicao.x = princesa->largura / 2;
    if (princesa->posicao.x > larguraTela - princesa->largura / 2) princesa->posicao.x = larguraTela - princesa->largura / 2;
    if (princesa->posicao.y < princesa->altura / 2) princesa->posicao.y = princesa->altura / 2;
    if (princesa->posicao.y > alturaTela - princesa->altura / 2) princesa->posicao.y = alturaTela - princesa->altura / 2;

    float tempoAtual = (float)GetTime();
    if (tempoAtual - princesa->tempoUltimoAtaque > 0.5f * princesa->bonusVelocidadeAtaque) {
        princesa->tempoUltimoAtaque = tempoAtual;
        
        Vilao* vilaoMaisProximo = NULL;
        float menorDistancia = FLT_MAX;
        
        Vilao* v = princesa->viloes;
        while (v != NULL) {
            if (v->ativo) {
                float distancia = Vector2Distance(princesa->posicao, v->posicao);
                if (distancia < menorDistancia) {
                    menorDistancia = distancia;
                    vilaoMaisProximo = v;
                }
            }
            v = v->proximo;
        }

        if (vilaoMaisProximo != NULL) {
            Ataque* novo = (Ataque*)malloc(sizeof(Ataque));
            novo->posicao = princesa->posicao;
            novo->direcao = Vector2Normalize(Vector2Subtract(vilaoMaisProximo->posicao, princesa->posicao));
            novo->ativo = true;
            novo->proximo = princesa->ataques;
            princesa->ataques = novo;
        }
    }
}

void desenharPrincesa(const Princesa *princesa) {
    float escala = 1.3f;
    Vector2 posicao = {
        princesa->posicao.x - (princesa->largura * escala) / 2,
        princesa->posicao.y - (princesa->altura * escala) / 2
    };
    
    DrawTextureEx(princesa->textura, posicao, 0.0f, escala, WHITE);
}

void liberarPrincesa(Princesa *princesa) {
    UnloadTexture(princesa->textura);
    UnloadTexture(princesa->texturaAtaque);
    UnloadTexture(princesa->texturaVilaoFraco);
    UnloadTexture(princesa->texturaVilaoForte);
    UnloadTexture(princesa->fundo);
    UnloadTexture(princesa->texturaOrbeXP);
    UnloadTexture(princesa->texturaOrbeVida);

    liberarAtaques(&(princesa->ataques));
    liberarViloes(&(princesa->viloes));
    liberarOrbesXP(&(princesa->orbesXP));
    liberarOrbesVida(&(princesa->orbesVida));
}