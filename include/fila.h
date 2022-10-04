#ifndef FILA_H
#define FILA_H

#include <tipos.h>

#include <stdlib.h>
#include <stdio.h>

void adicionaProcessoFila(NoProcesso **cabeca, NoProcesso **processo);
void adicionaDispositivoFila(NoIO **cabeca, NoIO **dispositivo);
void retiraProcessoFila(NoProcesso **cabeca);
void retiraDispositivoFila(NoIO **cabeca);

#endif