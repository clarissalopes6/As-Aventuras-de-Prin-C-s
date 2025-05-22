#include "raylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <float.h>
#include "raymath.h"
#include <string.h>

#define MAX_SCORES 5
#define NOME_TAM 20

#define TAMANHO_CELULA 40
#define MAX_COLUNAS (800 / TAMANHO_CELULA)
#define MAX_LINHAS (600 / TAMANHO_CELULA)

typedef struct {
    char nome[NOME_TAM];
    int score;
} HighScore;

typedef enum {
    JOGANDO,
    ESCOLHENDO_UPGRADE
} EstadoJogo;

typedef enum {
    CELULA_LIVRE,
    CELULA_OCUPADA_ORBE_VIDA,
    CELULA_OBSTACULO
} EstadoCelula;

typedef struct Ataque {
    Vector2 posicao;
    Vector2 direcao;
    bool ativo;
    struct Ataque* proximo;
} Ataque;

typedef struct Vilao {
    Vector2 posicao;
    bool ativo;
    struct Vilao* proximo;
    int vida;
} Vilao;

typedef struct OrbeXP {
    Vector2 posicao;
    bool ativo;
    struct OrbeXP* proximo;
    int valorXP;
} OrbeXP;

typedef struct OrbeVida {
    Vector2 posicao;
    bool ativo;
    struct OrbeVida* proximo;
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
    Ataque* ataques;
    Vilao* viloes;
    double tempoJogo;       
    int nivel;             
    int inimigosPorNivel;    
    float tempoUltimoAtaque;
    int viloesCriadosNoNivel;
    float bonusVelocidadeAtaque;
    int bonusDano;
    int vidasExtras;
    OrbeXP* orbesXP;
    int xpAtual;
    int xpParaProximoNivel;
    Texture2D texturaOrbeXP; 
    OrbeVida* orbesVida;        
    Texture2D texturaOrbeVida;
    EstadoCelula matrizDeSpawn[MAX_LINHAS][MAX_COLUNAS];
    int numLinhas;
    int numColunas;
} Princesa;

void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, const char* imgVilaoFraco, const char* imgVilaoForte, const char* imgFundo, const char* imgOrbeXP, const char* imgOrbeVida, int x, int y, int larguraDoJogo, int alturaDoJogo);
void AtualizarPrincesa(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharPrincesa(const Princesa* princesa);
void AtualizarAtaques(Princesa* princesa);
void DesenharAtaques(const Princesa* princesa);
void AtualizarViloes(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharViloes(const Princesa* princesa);
void VerificarColisoes(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharInterface(const Princesa* princesa);
void LiberarMemoria(Princesa* princesa);
void CriarOrbeXP(Princesa* princesa, Vector2 posicao);
bool AtualizarOrbesXP(Princesa* princesa);
void DesenharOrbesXP(const Princesa* princesa);
void CarregarHighScores(HighScore scores[]);
int VerificarHighScore(HighScore scores[], int score);
void InserirHighScore(HighScore scores[], int posicao, const char* nome, int score);
void MostrarHighScores(HighScore scores[]);
void SalvarHighScores(HighScore scores[]);
void CriarOrbeVida(Princesa* princesa, int larguraTela, int alturaTela);
void AtualizarOrbesVida(Princesa* princesa, int larguraTela, int alturaTela);
void DesenharOrbesVida(const Princesa* princesa);

int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "As Aventuras de Prin C's");
    SetTargetFPS(60);

    Princesa princesa;
    IniciarPrincesa(&princesa, "assets/princesa.png", "assets/ataque.png", "assets/goblin_fraco.png", "assets/goblin_forte.png", "assets/fundo.png", "assets/xp_basica.png", "assets/orb_vida.png", larguraTela / 2, alturaTela / 2, larguraTela, alturaTela);

    double tempoInicio = GetTime();

    EstadoJogo estado = JOGANDO;

    HighScore highScores[MAX_SCORES];
    CarregarHighScores(highScores);

    while (!WindowShouldClose()) {
        if (princesa.vida <= 0) break;
        
        BeginDrawing();

        if (estado == JOGANDO) {
            AtualizarPrincesa(&princesa, larguraTela, alturaTela);
            AtualizarAtaques(&princesa);
            AtualizarViloes(&princesa, larguraTela, alturaTela);
            VerificarColisoes(&princesa, larguraTela, alturaTela);
            AtualizarOrbesVida(&princesa, larguraTela, alturaTela);
    
            if (AtualizarOrbesXP(&princesa)) {
                estado = ESCOLHENDO_UPGRADE;
            }

            princesa.tempoJogo = GetTime() - tempoInicio;

            DrawTexture(princesa.fundo, 0, 0, WHITE);
            DesenharPrincesa(&princesa);
            DesenharAtaques(&princesa);
            DesenharViloes(&princesa);
            DesenharOrbesXP(&princesa);
            DesenharOrbesVida(&princesa);
            DesenharInterface(&princesa);
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

        int posicao = VerificarHighScore(highScores, princesa.pontos);

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
                InserirHighScore(highScores, posicao, nome, princesa.pontos);
                SalvarHighScores(highScores);
            }
        }
        
        MostrarHighScores(highScores);
    }

    LiberarMemoria(&princesa);
    CloseWindow();
    return 0;
}

void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, 
    const char* imgVilaoFraco, const char* imgVilaoForte, const char* imgFundo, const char* imgOrbeXP, const char* imgOrbeVida, int x, int y, int larguraDoJogo, int alturaDoJogo) {
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
    princesa->numColunas = larguraDoJogo / TAMANHO_CELULA;
    princesa->numLinhas = alturaDoJogo / TAMANHO_CELULA;

    for (int i = 0; i < princesa->numLinhas; i++) {
        for (int j = 0; j < princesa->numColunas; j++) {
            princesa->matrizDeSpawn[i][j] = CELULA_LIVRE;
        }
    }
}

void AtualizarPrincesa(Princesa* princesa, int larguraTela, int alturaTela) {
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

void DesenharPrincesa(const Princesa* princesa) {
    float escala = 1.3f;
    Vector2 posicao = {
        princesa->posicao.x - (princesa->largura * escala) / 2,
        princesa->posicao.y - (princesa->altura * escala) / 2
    };
    
    DrawTextureEx(princesa->textura, posicao, 0.0f, escala, WHITE);
}

void AtualizarAtaques(Princesa* princesa) {
    Ataque* atual = princesa->ataques;
    while (atual != NULL) {
        if (atual->ativo) {
            atual->posicao = Vector2Add(atual->posicao, Vector2Scale(atual->direcao, 10));

            if (atual->posicao.x < 0 || atual->posicao.x > GetScreenWidth() ||
                atual->posicao.y < 0 || atual->posicao.y > GetScreenHeight()) {
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

void DesenharViloes(const Princesa* princesa) {
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

void VerificarColisoes(Princesa* princesa, int larguraTela, int alturaTela) {
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
                    CriarOrbeXP(princesa, v->posicao);
                    CriarOrbeVida(princesa, larguraTela, alturaTela);
                }
            }
            a = a->proximo;
        }
        v = v->proximo;
    }
}    

void DesenharInterface(const Princesa* princesa) {
    DrawText(TextFormat("Pontos: %d", princesa->pontos), 10, 10, 20, GREEN);
    DrawText(TextFormat("Vidas: %d", princesa->vida), 10, 40, 20, RED);
    DrawText(TextFormat("Tempo: %02d:%02d", (int)princesa->tempoJogo / 60, (int)princesa->tempoJogo % 60), 10, 70, 20, YELLOW);
    DrawText(TextFormat("Nivel: %d", princesa->nivel), 10, 100, 20, BLUE);
    DrawText(TextFormat("XP: %d/%d", princesa->xpAtual, princesa->xpParaProximoNivel), 10, 130, 20, YELLOW);
}

void CriarOrbeXP(Princesa* princesa, Vector2 posicao) {
    OrbeXP* novo = (OrbeXP*)malloc(sizeof(OrbeXP));
    novo->posicao = posicao;
    novo->ativo = true;
    novo->valorXP = 1;
    novo->proximo = princesa->orbesXP;
    princesa->orbesXP = novo;
}

bool AtualizarOrbesXP(Princesa* princesa) {
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

void DesenharOrbesXP(const Princesa* princesa) {
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

void CriarOrbeVida(Princesa* princesa, int larguraTela, int alturaTela) {
    if (GetRandomValue(0, 1) == 0) {
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

void AtualizarOrbesVida(Princesa* princesa, int larguraTela, int alturaTela) {
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

void DesenharOrbesVida(const Princesa* princesa) {
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

void LiberarMemoria(Princesa* princesa) {
    UnloadTexture(princesa->textura);
    UnloadTexture(princesa->texturaAtaque);
    UnloadTexture(princesa->texturaVilaoFraco);
    UnloadTexture(princesa->texturaVilaoForte);
    UnloadTexture(princesa->fundo);
    UnloadTexture(princesa->texturaOrbeXP); 

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

    OrbeXP* o = princesa->orbesXP;
    while (o != NULL) {
        OrbeXP* prox = o->proximo;
        free(o);
        o = prox;
    }

    OrbeVida* ov = princesa->orbesVida; 
    while (ov != NULL) {               
        OrbeVida* prox = ov->proximo;  
        free(ov);      
        ov = prox;          
    }  
}

void CarregarHighScores(HighScore scores[]) {
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

void SalvarHighScores(HighScore scores[]) {
    FILE *file = fopen("highscores.txt", "w");
    if (file) {
        fwrite(scores, sizeof(HighScore), MAX_SCORES, file);
        fclose(file);
    }
}

int VerificarHighScore(HighScore scores[], int score) {
    for (int i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i].score) {
            return i;
        }
    }
    return -1;
}

void InserirHighScore(HighScore scores[], int posicao, const char* nome, int score) {

    for (int i = MAX_SCORES - 1; i > posicao; i--) {
        scores[i] = scores[i - 1];
    }

    strncpy(scores[posicao].nome, nome, NOME_TAM - 1);
    scores[posicao].nome[NOME_TAM - 1] = '\0';
    scores[posicao].score = score;
}

void MostrarHighScores(HighScore scores[]) {
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