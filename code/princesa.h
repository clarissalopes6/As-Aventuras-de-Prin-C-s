#ifndef PRINCESA_H
#define PRINCESA_H

#include "raylib.h"
#include "structs.h"

void iniciarPrincesa(Princesa *princesa, const char* imgPrincesa, const char* imgAtaque, const char* imgVilaoFraco, const char* imgVilaoForte, const char* imgFundo, const char* imgOrbeXP, const char* imgOrbeVida, int x, int y, int larguraDoJogo, int alturaDoJogo);

void atualizarPrincesa(Princesa *princesa, int larguraTela, int alturaTela);

void desenharPrincesa(const Princesa *princesa);

void liberarPrincesa(Princesa *princesa);

#endif