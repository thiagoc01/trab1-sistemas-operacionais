#ifndef FILA_H
#define FILA_H

#include "tipos.h"

#include <stdlib.h>
#include <stdio.h>

void adicionaProcessoFila(NoProcesso **cabeca, NoProcesso **processo);
void adicionaDispositivoFila(NoIO **cabeca, NoIO **dispositivo);
void retiraProcessoFila(NoProcesso **cabeca);
void retiraDispositivoFila(NoIO **cabeca);

#endif


/*Processo* criaProcesso(int tempoServico, int pid);

void adicionaIO(Processo* proc, int tipo, int tempoEntrada);

IO* popIO(Processo *proc);

void appendProcesso(Processo *append);

Processo* popProcesso(Processo *primeiro);

void atualizaFila(Processo* fila);

void carregaEntrada();*/