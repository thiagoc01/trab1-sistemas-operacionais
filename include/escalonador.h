#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <tipos.h>

void escalonaProcessos();

void controlaFilaProcesso(NoProcesso **fila);

void controlaFilaDispositivo(NoIO **fila);

void checaTempoEntradaProcesso(NoProcesso **entrada);

void imprimeInformacoesFilas();

void imprimeInformacoesFilasProcessos(NoProcesso *fila, const char* tipoFila);

void imprimeInformacoesFilasDispositivos(NoIO *fila, const char* tipoFila);

#endif