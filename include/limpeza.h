#ifndef LIMPEZA_H
#define LIMPEZA_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <tipos.h>

/* Utilizado para liberar os recursos do programa. É chamado ao matar o processo ou na execução de 10 minutos automaticamente. */

void liberaProcessos(NoProcesso **no);

void liberaDispositivos(NoIO **no);

void liberaRecursos(int signum);

void alteraComportamentoSigTermInt();

#endif