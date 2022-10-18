#include <stdlib.h>
#include <time.h>
#include <escalonador.h>
#include <limpeza.h>
#include <string.h>

extern int utilizaEntrada;
extern char *caminhoArquivoEntrada;

void trataArgumentosTerminal(int argc, char **argv)
{
    if (argc == 2)
    {
        utilizaEntrada = 1;

        int qtdCharsArquivoEntrada = strlen(argv[1]) + 1;

        caminhoArquivoEntrada = (char *) malloc(qtdCharsArquivoEntrada);

        strcpy(caminhoArquivoEntrada, argv[1]);
    }        
}

int main(int argc, char **argv)
{
    trataArgumentosTerminal(argc, argv);

    alteraComportamentoSigTermInt();

    srand(time(NULL));

    escalonaProcessos();

    return 0;
}