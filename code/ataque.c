#include "ataque.h"
#include <stdlib.h>

#include "raymath.h"

void atualizarAtaques(Princesa *princesa) {
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

void desenharAtaques(const Princesa *princesa) {
    Ataque* atual = princesa->ataques;
    while (atual != NULL) {
        if (atual->ativo) {
            DrawTexture(princesa->texturaAtaque, atual->posicao.x - princesa->larguraAtaque / 2, atual->posicao.y - princesa->alturaAtaque / 2, WHITE);
        }
        atual = atual->proximo;
    }
}

void liberarAtaques(Ataque **listaAtaques) {
    Ataque* atual = *listaAtaques;
    Ataque* proximo = NULL;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    *listaAtaques = NULL;
}