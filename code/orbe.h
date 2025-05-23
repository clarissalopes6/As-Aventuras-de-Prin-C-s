#ifndef ORBE_H
#define ORBE_H

#include "structs.h"
#include "raylib.h"

void criarOrbeXP(Princesa* princesa, Vector2 posicao);
bool atualizarOrbesXP(Princesa* princesa);
void desenharOrbesXP(const Princesa* princesa);
void liberarOrbesXP(OrbeXP** listaOrbesXP);

void criarOrbeVida(Princesa *princesa, int larguraTela, int alturaTela);
void atualizarOrbesVida(Princesa *princesa, int larguraTela, int alturaTela);
void desenharOrbesVida(const Princesa *princesa);
void liberarOrbesVida(OrbeVida **listaOrbesVida);

#endif