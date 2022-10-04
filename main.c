#include <stdlib.h>
#include <time.h>
#include <escalonador.h>
#include <limpeza.h>

int main()
{
    alteraComportamentoSigTermInt();

    srand(time(NULL));

    escalonaProcessos();

    return 0;
}