#ifndef ATAQUE_H
#define ATAQUE_H

#include "structs.h"
#include "raylib.h"

void atualizarAtaques(Princesa *princesa);
void desenharAtaques(const Princesa *princesa);
void liberarAtaques(Ataque **listaAtaques);

#endif