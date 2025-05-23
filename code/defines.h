#ifndef DEFINES_H
#define DEFINES_H

#define MAX_SCORES 5
#define NOME_TAM 20

#define TAMANHO_CELULA 40
#define MAX_COLUNAS (800 / TAMANHO_CELULA)
#define MAX_LINHAS (600 / TAMANHO_CELULA)

typedef enum {
    JOGANDO,
    ESCOLHENDO_UPGRADE
} EstadoJogo;

typedef enum {
    CELULA_LIVRE,
    CELULA_OCUPADA_ORBE_VIDA,
    CELULA_OBSTACULO
} EstadoCelula;

#endif