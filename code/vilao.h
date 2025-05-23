#ifndef VILAO_H
#define VILAO_H

#include "structs.h"
#include "raylib.h"

void atualizarViloes(Princesa *princesa, int larguraTela, int alturaTela);
void desenharViloes(const Princesa *princesa);
void liberarViloes(Vilao **listaViloes);

#endif