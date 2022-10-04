#ifndef PROCESSO_H
#define PROCESSO_H

#include <tipos.h>
#include <fila.h>
#include <constantes.h>

Processo *criaProcesso(NoProcesso **cabeca, int pid, int chegada);

void liberaProcesso(Processo **processo);

void geraIOProcesso(Processo **novo);

void imprimeInformacoesProcesso(Processo *processo);

#endif 