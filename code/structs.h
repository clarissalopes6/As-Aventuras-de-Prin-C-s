#ifndef STRUCTS_H
#define STRUCTS_H

#include "raylib.h"
#include "defines.h"

typedef struct {
    char nome[NOME_TAM];
    int score;
} HighScore;

typedef struct Ataque {
    Vector2 posicao;
    Vector2 direcao;
    bool ativo;
    struct Ataque *proximo;
} Ataque;

typedef struct Vilao {
    Vector2 posicao;
    bool ativo;
    struct Vilao *proximo;
    int vida;
} Vilao;

typedef struct OrbeXP {
    Vector2 posicao;
    bool ativo;
    struct OrbeXP *proximo;
    int valorXP;
} OrbeXP;

typedef struct OrbeVida {
    Vector2 posicao;
    bool ativo;
    struct OrbeVida *proximo;
    int Linha; 
    int Coluna; 
} OrbeVida;

typedef struct Princesa {
    Vector2 posicao;
    Texture2D textura;
    Texture2D texturaAtaque;
    Texture2D texturaVilaoFraco;
    Texture2D texturaVilaoForte;
    Texture2D fundo;
    int largura;
    int altura;
    int larguraAtaque;
    int alturaAtaque;
    int larguraVilaoFraco;
    int alturaVilaoFraco;
    int larguraVilaoForte;
    int alturaVilaoForte;
    int velocidade;
    int vida;
    int pontos;
    int acertos;
    Ataque *ataques;
    Vilao *viloes;
    double tempoJogo;       
    int nivel;             
    int inimigosPorNivel;    
    float tempoUltimoAtaque;
    int viloesCriadosNoNivel;
    float bonusVelocidadeAtaque;
    int bonusDano;
    int vidasExtras;
    OrbeXP *orbesXP;
    int xpAtual;
    int xpParaProximoNivel;
    Texture2D texturaOrbeXP; 
    OrbeVida *orbesVida;        
    Texture2D texturaOrbeVida;
    EstadoCelula matrizDeSpawn[MAX_LINHAS][MAX_COLUNAS];
    int numLinhas;
    int numColunas;
} Princesa;

#endif