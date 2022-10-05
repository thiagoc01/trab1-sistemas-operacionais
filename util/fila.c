#include <fila.h>
#include <constantes.h>

/*
    Implementação de fila circular para o gerenciamento de processos e entrada/saída.
*/

void adicionaProcessoFila(NoProcesso **cabeca, NoProcesso **processo)
{
    if (*cabeca == NULL)
    {
        (*processo)->anterior = *processo;
        (*processo)->proximo = *processo;
        *cabeca = *processo;
        
    }

    else
    {
        (*cabeca)->anterior->proximo = *processo;
        (*processo)->anterior = (*cabeca)->anterior;
        (*processo)->proximo = *cabeca;
        (*cabeca)->anterior = *processo;
    }
}

void adicionaDispositivoFila(NoIO **cabeca, NoIO **dispositivo, int *tamanhoFila)
{
    if (*cabeca == NULL)
    {
        (*dispositivo)->anterior = *dispositivo;
        (*dispositivo)->proximo = *dispositivo;
        *cabeca = *dispositivo;
        *tamanhoFila = 1;        
    }

    else
    {
        (*cabeca)->anterior->proximo = *dispositivo;
        (*dispositivo)->anterior = (*cabeca)->anterior;
        (*dispositivo)->proximo = *cabeca;
        (*cabeca)->anterior = *dispositivo;
        *tamanhoFila++;
    }
}

void retiraProcessoFila(NoProcesso **cabeca)
{
    if (*cabeca)
    {
        NoProcesso *pop = *cabeca;

        if ((*cabeca)->anterior == *cabeca && (*cabeca)->proximo == *cabeca)
        {
            free(*cabeca);
            *cabeca = NULL;
        }
            

        else
        {
            (*cabeca)->anterior->proximo = (*cabeca)->proximo;

            *cabeca = pop->proximo;
            (*cabeca)->anterior = pop->anterior;
            free(pop);
        }       
    }
}

void retiraDispositivoFila(NoIO **cabeca, int *tamanhoFila)
{
    if (*cabeca)
    {
        NoIO *pop = *cabeca;
        
        if ((*cabeca)->anterior == *cabeca && (*cabeca)->proximo == *cabeca)
        {
            free(*cabeca);
            *cabeca = NULL;
            *tamanhoFila = 0;
        }

        else
        {
            (*cabeca)->anterior->proximo = (*cabeca)->proximo;

            *cabeca = pop->proximo;
            (*cabeca)->anterior = pop->anterior;
            free(pop);
            *tamanhoFila--;
        }

        
    }
}
