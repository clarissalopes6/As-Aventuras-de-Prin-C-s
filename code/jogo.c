    #include "raylib.h"
    #include <stdlib.h>
    #include <stdio.h>
    #include <time.h>
    #include <float.h>
    #include "raymath.h"

    typedef enum {
        JOGANDO,
        ESCOLHENDO_UPGRADE
    } EstadoJogo;

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
    } Princesa;
    

    void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, const char* imgVilao, const char* imgFundo, const char* imgOrbeXP, int x, int y);
    void AtualizarPrincesa(Princesa* princesa, int larguraTela, int alturaTela);
    void DesenharPrincesa(const Princesa* princesa);
    void AtualizarAtaques(Princesa* princesa);
    void DesenharAtaques(const Princesa* princesa);
    void AtualizarViloes(Princesa* princesa, int larguraTela, int alturaTela);
    void DesenharViloes(const Princesa* princesa);
    void VerificarColisoes(Princesa* princesa);
    void DesenharInterface(const Princesa* princesa);
    void LiberarMemoria(Princesa* princesa);
    void CriarOrbeXP(Princesa* princesa, Vector2 posicao) ;
    bool AtualizarOrbesXP(Princesa* princesa);
    void DesenharOrbesXP(const Princesa* princesa);

    int main(void) {
        const int larguraTela = 800;
        const int alturaTela = 600;

        InitWindow(larguraTela, alturaTela, "As Aventuras de Prin C's");
        SetTargetFPS(60);

        Princesa princesa;
        IniciarPrincesa(&princesa, "assets/princesa.png", "assets/ataque.png", "assets/viloes.png", "assets/fundo.png", "assets/xp_basica.png", larguraTela / 2, alturaTela / 2);

        double tempoInicio = GetTime();
        bool venceu = false;

        EstadoJogo estado = JOGANDO;

        while (!WindowShouldClose() && !venceu) {
            if (princesa.vida <= 0) break;
            
            BeginDrawing();

            
            if (estado == JOGANDO) {
                AtualizarPrincesa(&princesa, larguraTela, alturaTela);
                AtualizarAtaques(&princesa);
                AtualizarViloes(&princesa, larguraTela, alturaTela);
                VerificarColisoes(&princesa);
        
                princesa.tempoJogo = GetTime() - tempoInicio;
        
                if (AtualizarOrbesXP(&princesa)) {
                    estado = ESCOLHENDO_UPGRADE;
                }

                princesa.tempoJogo = GetTime() - tempoInicio;

                DrawTexture(princesa.fundo, 0, 0, WHITE);
                DesenharPrincesa(&princesa);
                DesenharAtaques(&princesa);
                DesenharViloes(&princesa);
                DesenharOrbesXP(&princesa);
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
            
            venceu = (princesa.nivel >= 10); 
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
        }
        else if (venceu) {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("VITORIA!", larguraTela/2 - MeasureText("VITORIA!", 40)/2, alturaTela/2 - 20, 40, GREEN);
            DrawText(TextFormat("Pontuacao final: %d", princesa.pontos), 
                   larguraTela/2 - MeasureText(TextFormat("Pontuacao final: %d", princesa.pontos), 20)/2, 
                   alturaTela/2 + 30, 20, WHITE);
            EndDrawing();
            WaitTime(3);
        }

        LiberarMemoria(&princesa);
        CloseWindow();
        return 0;
    }

    void IniciarPrincesa(Princesa* princesa, const char* imgPrincesa, const char* imgAtaque, 
        const char* imgVilao, const char* imgFundo, const char* imgOrbeXP, int x, int y) {
        princesa->textura = LoadTexture(imgPrincesa);
        princesa->texturaAtaque = LoadTexture(imgAtaque);
        princesa->texturaVilao = LoadTexture(imgVilao);
        princesa->fundo = LoadTexture(imgFundo);
        princesa->texturaOrbeXP = LoadTexture(imgOrbeXP);
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
        princesa->tempoJogo = 0.0;
        princesa->nivel = 1;
        princesa->inimigosPorNivel = 3;
        princesa->tempoUltimoAtaque = 0.0f;
        princesa->viloesCriadosNoNivel = 0;
        princesa->bonusVelocidadeAtaque = 1.0f;
        princesa->bonusDano = 1;
        princesa->vidasExtras = 0;
        princesa->orbesXP = NULL;
        princesa->xpAtual = 0;
        princesa->xpParaProximoNivel = 10;
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
        DrawTexture(princesa->textura, princesa->posicao.x - princesa->largura / 2, princesa->posicao.y - princesa->altura / 2, WHITE);
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
        if (GetRandomValue(0, 100) < (2 + princesa->nivel)) {
            Vilao* novo = (Vilao*)malloc(sizeof(Vilao));
            //novo->posicao = (Vector2){ GetRandomValue(0, larguraTela), GetRandomValue(0, alturaTela/2) };
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
            novo->vida = 1;
            novo->ativo = true;
            novo->proximo = princesa->viloes;
            princesa->viloes = novo;
            princesa->viloesCriadosNoNivel++;
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
            if (v->ativo && CheckCollisionCircles(princesa->posicao, princesa->largura / 2, v->posicao, princesa->larguraVilao / 2)) {
                v->ativo = false;
                princesa->vida--;
            }
    
            Ataque* a = princesa->ataques;
            while (a != NULL) {
                if (v->ativo && a->ativo && CheckCollisionCircles(a->posicao, princesa->larguraAtaque / 2, v->posicao, princesa->larguraVilao / 2)) {
                    a->ativo = false;
                    v->vida -= princesa->bonusDano;
                    if (v->vida <= 0) {
                        v->ativo = false;
                        princesa->pontos++;
                        CriarOrbeXP(princesa, v->posicao);
                    }
                }
                a = a->proximo;
            }
            v = v->proximo;
        }
    }    

    void DesenharInterface(const Princesa* princesa) {
        DrawText(TextFormat("Pontos: %d", princesa->pontos), 10, 10, 20, GREEN);
        DrawText(TextFormat("Vida: %d", princesa->vida), 10, 40, 20, RED);
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

    void LiberarMemoria(Princesa* princesa) {
        UnloadTexture(princesa->textura);
        UnloadTexture(princesa->texturaAtaque);
        UnloadTexture(princesa->texturaVilao);
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
        
    }