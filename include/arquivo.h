#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <tipos.h>
#include <fila.h>
#include <constantes.h>
#include <processo.h>

#include <stdlib.h>
#include <unistd.h>

void carregaEntrada(char *fileName,NoProcesso **entrada);

#endif