#ifndef CONSTANTES_H
#define CONSTANTES_H

#include "tipos.h"

#define MAX_PROCESSOS 5
#define MAX_QUANTUM 5

#define TEMPO_IO_IMPRESSORA 3
#define TEMPO_IO_DISCO 4
#define TEMPO_IO_FITA 5

#define IO_FITA 1
#define IO_DISCO 2
#define IO_IMPRESSORA 3

#define TIPO_IO(x)  ( (x) == IO_FITA ? "fita" : (x) == IO_DISCO ? "disco" : "impressora")

#define PRONTO 4
#define ESPERANDO 5
#define BLOQUEADO 6

#define ALTA_PRIOR 7
#define BAIXA_PRIOR 8

#endif