#ifndef FILA_H
#define FILA_H

#include <tipos.h>

#include <stdlib.h>
#include <stdio.h>

void adicionaProcessoFila(NoProcesso **cabeca, NoProcesso **processo);
void adicionaDispositivoFila(NoIO **cabeca, NoIO **dispositivo, int *tamanhoFila);
void retiraProcessoFila(NoProcesso **cabeca, Processo **retorno, int entrada);
void retiraDispositivoFila(NoIO **cabeca, int *tamanhoFila);

#endif