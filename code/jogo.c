#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "raymath.h"

typedef struct Ataque {
    Vector2 posicao;
    bool ativo;
    struct Ataque* proximo;
} Ataque;

typedef struct Vilao {
    Vector2 posicao;
    bool ativo;
    struct Vilao* proximo;
} Vilao;

typedef struct Princesa {
    Vector2 posicao;
    Texture2D textura;
    Texture2D texturaAtaque;
    Texture2D texturaVilao;
    Texture2D fundo;
    int largura;
    int altura;
    int larguraAtaque;
    int alturaAtaque;
    int larguraVilao;
    int alturaVilao;
    int velocidade;
    int vida;
    int pontos;
    int acertos;
    Ataque* ataques;
    Vilao* viloes;
} Princesa;

void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, const char* imgVilao, const char* imgFundo, int x, int y);
void AtualizarPrincesa(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharPrincesa(const Princesa* princesa);
void AtualizarAtaques(Princesa* princesa);
void DesenharAtaques(const Princesa* princesa);
void AtualizarViloes(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharViloes(const Princesa* princesa);
void VerificarColisoes(Princesa* princesa);
void DesenharInterface(const Princesa* princesa, double tempoRestante);
void LiberarMemoria(Princesa* princesa);

int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "As Aventuras de Prin C's");
    SetTargetFPS(60);

    Princesa princesa;
    IniciarPrincesa(&princesa, "assets/princesa.png", "assets/ataque.png", "assets/viloes.png", "assets/fundo.png", larguraTela / 2, alturaTela / 2);

    double tempoInicio = GetTime();
    bool venceu = false;

    while (!WindowShouldClose() && princesa.vida > 0 && !venceu) {
        AtualizarPrincesa(&princesa, larguraTela, alturaTela);
        AtualizarAtaques(&princesa);
        AtualizarViloes(&princesa, larguraTela, alturaTela);
        VerificarColisoes(&princesa);

        double tempoAtual = GetTime() - tempoInicio;
        double tempoRestante = 60.0 - tempoAtual;
        if (tempoRestante <= 0) {
            tempoRestante = 0;
            venceu = true;
        }

        BeginDrawing();
        DrawTexture(princesa.fundo, 0, 0, WHITE);
        DesenharPrincesa(&princesa);
        DesenharAtaques(&princesa);
        DesenharViloes(&princesa);
        DesenharInterface(&princesa, tempoRestante);
        EndDrawing();
    }

    BeginDrawing();
    ClearBackground(DARKGRAY);
    if (venceu) {
        DrawText("Vitória da Princesa!", larguraTela / 2 - 150, alturaTela / 2 - 20, 40, GREEN);
    } else {
        DrawText("Game Over!", larguraTela / 2 - 100, alturaTela / 2 - 20, 40, RED);
    }
    DrawText(TextFormat("Pontuação final: %d", princesa.pontos), larguraTela / 2 - 120, alturaTela / 2 + 30, 20, YELLOW);
    EndDrawing();
    WaitTime(3.0f);

    LiberarMemoria(&princesa);
    CloseWindow();
    return 0;
}

void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, const char* imgVilao, const char* imgFundo, int x, int y) {
    princesa->textura = LoadTexture(imgPrincesa);
    princesa->texturaAtaque = LoadTexture(imgAtaque);
    princesa->texturaVilao = LoadTexture(imgVilao);
    princesa->fundo = LoadTexture(imgFundo);
    princesa->largura = princesa->textura.width;
    princesa->altura = princesa->textura.height;
    princesa->larguraAtaque = princesa->texturaAtaque.width;
    princesa->alturaAtaque = princesa->texturaAtaque.height;
    princesa->larguraVilao = princesa->texturaVilao.width;
    princesa->alturaVilao = princesa->texturaVilao.height;
    princesa->posicao = (Vector2){x, y};
    princesa->velocidade = 5;
    princesa->vida = 3;
    princesa->pontos = 0;
    princesa->acertos = 0;
    princesa->ataques = NULL;
    princesa->viloes = NULL;
}

void AtualizarPrincesa(Princesa* princesa, int larguraTela, int alturaTela) {
    if (IsKeyDown(KEY_W)) princesa->posicao.y -= princesa->velocidade;
    if (IsKeyDown(KEY_S)) princesa->posicao.y += princesa->velocidade;
    if (IsKeyDown(KEY_A)) princesa->posicao.x -= princesa->velocidade;
    if (IsKeyDown(KEY_D)) princesa->posicao.x += princesa->velocidade;

    if (princesa->posicao.x < princesa->largura / 2) princesa->posicao.x = princesa->largura / 2;
    if (princesa->posicao.x > larguraTela - princesa->largura / 2) princesa->posicao.x = larguraTela - princesa->largura / 2;
    if (princesa->posicao.y < princesa->altura / 2) princesa->posicao.y = princesa->altura / 2;
    if (princesa->posicao.y > alturaTela - princesa->altura / 2) princesa->posicao.y = alturaTela - princesa->altura / 2;

    if (IsKeyPressed(KEY_SPACE)) {
        Ataque* novo = (Ataque*)malloc(sizeof(Ataque));
        novo->posicao = princesa->posicao;
        novo->ativo = true;
        novo->proximo = princesa->ataques;
        princesa->ataques = novo;
    }
}

void DesenharPrincesa(const Princesa* princesa) {
    DrawTexture(princesa->textura, princesa->posicao.x - princesa->largura / 2, princesa->posicao.y - princesa->altura / 2, WHITE);
}

void AtualizarAtaques(Princesa* princesa) {
    Ataque* atual = princesa->ataques;
    while (atual != NULL) {
        if (atual->ativo) {
            atual->posicao.y -= 10;
            if (atual->posicao.y < 0) {
                atual->ativo = false;
            }
        }
        atual = atual->proximo;
    }
}

void DesenharAtaques(const Princesa* princesa) {
    Ataque* atual = princesa->ataques;
    while (atual != NULL) {
        if (atual->ativo) {
            DrawTexture(princesa->texturaAtaque, atual->posicao.x - princesa->larguraAtaque / 2, atual->posicao.y - princesa->alturaAtaque / 2, WHITE);
        }
        atual = atual->proximo;
    }
}

void AtualizarViloes(Princesa* princesa, int larguraTela, int alturaTela) {
    if (GetRandomValue(0, 60) == 1) {
        Vilao* novo = (Vilao*)malloc(sizeof(Vilao));
        novo->posicao = (Vector2){ GetRandomValue(0, larguraTela), 0 };
        novo->ativo = true;
        novo->proximo = princesa->viloes;
        princesa->viloes = novo;
    }

    Vilao* atual = princesa->viloes;
    while (atual != NULL) {
        if (atual->ativo) {
            Vector2 direcao = Vector2Normalize(Vector2Subtract(princesa->posicao, atual->posicao));
            atual->posicao = Vector2Add(atual->posicao, Vector2Scale(direcao, 2));
            if (atual->posicao.x < 0) atual->posicao.x = 0;
            if (atual->posicao.x > larguraTela) atual->posicao.x = larguraTela;
            if (atual->posicao.y < 0) atual->posicao.y = 0;
            if (atual->posicao.y > alturaTela) atual->posicao.y = alturaTela;
        }
        atual = atual->proximo;
    }
}

void DesenharViloes(const Princesa* princesa) {
    Vilao* atual = princesa->viloes;
    while (atual != NULL) {
        if (atual->ativo) {
            DrawTexture(princesa->texturaVilao, atual->posicao.x - princesa->larguraVilao / 2, atual->posicao.y - princesa->alturaVilao / 2, WHITE);
        }
        atual = atual->proximo;
    }
}

void VerificarColisoes(Princesa* princesa) {
    Vilao* v = princesa->viloes;
    while (v != NULL) {
        if (v->ativo && CheckCollisionCircles(princesa->posicao, 20, v->posicao, 20)) {
            v->ativo = false;
            princesa->acertos++;
            if (princesa->acertos >= 3) {
                princesa->vida = 0;
            }
        }

        Ataque* a = princesa->ataques;
        while (a != NULL) {
            if (a->ativo && v->ativo && CheckCollisionCircles(a->posicao, 10, v->posicao, 20)) {
                a->ativo = false;
                v->ativo = false;
                princesa->pontos++;
            }
            a = a->proximo;
        }

        v = v->proximo;
    }
}

void DesenharInterface(const Princesa* princesa, double tempoRestante) {
    DrawText(TextFormat("Pontos: %d", princesa->pontos), 10, 10, 20, GREEN);
    DrawText(TextFormat("Acertos dos vilões: %d / 3", princesa->acertos), 10, 40, 20, RED);
    DrawText(TextFormat("Tempo restante: %02d:%02d", (int)tempoRestante / 60, (int)tempoRestante % 60), 10, 70, 20, YELLOW);
}

void LiberarMemoria(Princesa* princesa) {
    UnloadTexture(princesa->textura);
    UnloadTexture(princesa->texturaAtaque);
    UnloadTexture(princesa->texturaVilao);
    UnloadTexture(princesa->fundo);

    Ataque* a = princesa->ataques;
    while (a != NULL) {
        Ataque* prox = a->proximo;
        free(a);
        a = prox;
    }

    Vilao* v = princesa->viloes;
    while (v != NULL) {
        Vilao* prox = v->proximo;
        free(v);
        v = prox;
    }
}