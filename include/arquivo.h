#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <tipos.h>
#include <fila.h>
#include <constantes.h>
#include <processo.h>

/**
 * Função responsável pelo carregamento do arquivo do usuário contendo os processos e suas informações.
*/

void carregaEntrada(char *fileName, NoProcesso **entrada);

#endif