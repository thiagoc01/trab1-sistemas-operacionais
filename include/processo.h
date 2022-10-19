#ifndef PROCESSO_H
#define PROCESSO_H

#include <tipos.h>
#include <fila.h>
#include <constantes.h>

Processo *criaProcesso(NoProcesso **cabeca, int pid, int chegada, int tempoServico, int numIOs,int autoGen);

void liberaProcesso(Processo **processo);

/**
 * Utilizada para gerar operações aleatórias de E/S para um processo.
*/

void geraIOProcesso(Processo **novo);

/**
 * Utilizada para criar as operações de E/S especificadas no arquivo de entrada.
*/

void adicionaIO(Processo **proc, int posicao, int tipo, int tempoEntrada);

void imprimeInformacoesProcesso(Processo *processo);

#endif 