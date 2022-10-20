#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <tipos.h>

/**
 * Função responsável pelo escalonamento oriundo de processos carregados via arquivo de entrada.
*/

void escalonaProcessosPrevios();

/**
 * Função responsável pelo escalonamento de processos que surgem aleatoriamente, com probabilidade de 20%.
*/

void escalonaProcessosRandomicos();

/**
 * Realiza os procedimentos específicos para a fila passada, como liberação do processo, time-out, dispatch, bloqueio etc.
*/

void controlaFilaProcesso(NoProcesso **fila);

/**
 * Realiza o atendimento dos processos para o dispositivo especificado por fila.
*/

void controlaFilaDispositivo(NoIO **fila);

/**
 * Utilizada para os processos já tabelados conforme entrada do arquivo do usuário.
*/

void checaTempoEntradaProcesso(NoProcesso **entrada);

void imprimeInformacoesFilas();

void imprimeInformacoesFilasProcessos(NoProcesso *fila, const char* tipoFila);

void imprimeInformacoesFilasDispositivos(NoIO *fila, const char* tipoFila);

#endif
