#include "orbe.h"
#include <stdlib.h>

void criarOrbeXP(Princesa *princesa, Vector2 posicao) {
    OrbeXP* novo = (OrbeXP*)malloc(sizeof(OrbeXP));
    novo->posicao = posicao;
    novo->ativo = true;
    novo->valorXP = 1;
    novo->proximo = princesa->orbesXP;
    princesa->orbesXP = novo;
}

bool atualizarOrbesXP(Princesa *princesa) {
    bool levelUp = false;
    OrbeXP* atual = princesa->orbesXP;
    while (atual != NULL) {
        if (atual->ativo) {
            if (CheckCollisionCircles(princesa->posicao, 20, atual->posicao, 15)) {
                atual->ativo = false;
                princesa->xpAtual += atual->valorXP;
                
                if (princesa->xpAtual >= princesa->xpParaProximoNivel) {
                    princesa->xpAtual -= princesa->xpParaProximoNivel;
                    princesa->nivel++;
                    princesa->inimigosPorNivel += 2;
                    princesa->viloesCriadosNoNivel = 0;
                    princesa->xpParaProximoNivel = princesa->nivel * 10;
                    levelUp = true;
                }
            }
        }
        atual = atual->proximo;
    }
    return levelUp;
}

void desenharOrbesXP(const Princesa*princesa) {
    OrbeXP* atual = princesa->orbesXP;
    while (atual != NULL) {
        if (atual->ativo) {
            DrawTexture(princesa->texturaOrbeXP, 
                        atual->posicao.x - princesa->texturaOrbeXP.width/2, 
                        atual->posicao.y - princesa->texturaOrbeXP.height/2, 
                        WHITE);
        }
        atual = atual->proximo;
    }
}

void liberarOrbesXP(OrbeXP **listaOrbesXP) {
    OrbeXP* atual = *listaOrbesXP;
    OrbeXP* proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *listaOrbesXP = NULL;
}

void criarOrbeVida(Princesa *princesa, int larguraTela, int alturaTela) {
    if (GetRandomValue(0, 99) == 0) {
        Vector2 posicoesLivres[MAX_LINHAS * MAX_COLUNAS]; 

        int countLivres = 0;

        for (int i = 0; i < princesa->numLinhas; i++) {
            for (int j = 0; j < princesa->numColunas; j++) {
                if (princesa->matrizDeSpawn[i][j] == CELULA_LIVRE) {
                    posicoesLivres[countLivres] = (Vector2){ (float)j, (float)i };
                    countLivres++;
                }
            }
        }

        if (countLivres == 0) {
            return;
        }

        int indiceAleatorio = GetRandomValue(0, countLivres - 1);
        int colunaEscolhida = (int)posicoesLivres[indiceAleatorio].x;
        int linhaEscolhida = (int)posicoesLivres[indiceAleatorio].y;

        OrbeVida* novo = (OrbeVida*)malloc(sizeof(OrbeVida));

        novo->posicao.x = (float)(colunaEscolhida * TAMANHO_CELULA) + (TAMANHO_CELULA / 2.0f);
        novo->posicao.y = (float)(linhaEscolhida * TAMANHO_CELULA) + (TAMANHO_CELULA / 2.0f);
        novo->ativo = true;
        novo->Linha = linhaEscolhida; 
        novo->Coluna = colunaEscolhida;

        princesa->matrizDeSpawn[linhaEscolhida][colunaEscolhida] = CELULA_OCUPADA_ORBE_VIDA;

        novo->proximo = princesa->orbesVida;
        princesa->orbesVida = novo;
    }
}

void atualizarOrbesVida(Princesa *princesa, int larguraTela, int alturaTela) {
    OrbeVida* atual = princesa->orbesVida;

    while (atual != NULL) {
        if (atual->ativo) {
            float raioPrincesaBase;
            if (princesa->largura < princesa->altura) {
                raioPrincesaBase = (float)princesa->largura;
            } else {
                raioPrincesaBase = (float)princesa->altura;
            }
            float raioPrincesa = (raioPrincesaBase / 2.0f) * 0.8f;
            float raioOrbe = princesa->texturaOrbeVida.width / 2.0f;

            if (CheckCollisionCircles(princesa->posicao, raioPrincesa, atual->posicao, raioOrbe)) {
                atual->ativo = false;
                princesa->vida++;
                if (atual->Linha >= 0 && atual->Linha < princesa->numLinhas &&
                    atual->Coluna >= 0 && atual->Coluna < princesa->numColunas) {
                    princesa->matrizDeSpawn[atual->Linha][atual->Coluna] = CELULA_LIVRE;
                }
            }
        }
        atual = atual->proximo;
    }
}

void desenharOrbesVida(const Princesa *princesa) {
    OrbeVida* atual = princesa->orbesVida;
    while (atual != NULL) {
        if (atual->ativo) {
            DrawTexture(princesa->texturaOrbeVida,
                        atual->posicao.x - princesa->texturaOrbeVida.width / 2,
                        atual->posicao.y - princesa->texturaOrbeVida.height / 2,
                        WHITE);
        }
        atual = atual->proximo;
    }
}

void liberarOrbesVida(OrbeVida **listaOrbesVida) {
    OrbeVida* atual = *listaOrbesVida;
    OrbeVida* proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *listaOrbesVida = NULL;
}