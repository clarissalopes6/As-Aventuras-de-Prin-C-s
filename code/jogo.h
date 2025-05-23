#ifndef JOGO_H
#define JOGO_H

#include "structs.h"
#include "raylib.h"

void verificarColisoes(Princesa *princesa, int larguraTela, int alturaTela);
void desenharInterface(const Princesa *princesa);

#endif