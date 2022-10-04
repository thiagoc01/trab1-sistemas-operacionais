#include <stdlib.h>
#include <time.h>
#include "escalonador.h"

int main()
{
    srand(time(NULL));

    escalonaProcessos();

    return 0;
}