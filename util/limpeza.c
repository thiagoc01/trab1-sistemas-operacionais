#include <limpeza.h>

extern NoProcesso *baixaPrioridade, *altaPrioridade;
extern NoIO *filaFita, *filaImpressora, *filaDisco;

void liberaProcessos(NoProcesso **no)
{
    NoProcesso *aux, *prox;

    if (*no)
    {
        Processo *processoRaiz = (*no)->processo;

        if ((*no)->processo->chamadasIO) // O processo realizou operações de IO
        {
            /* Como a quantidade de realizados é incrementada e a quantidade total decrementada, a soma resultado no total solicitado. */

            for (int i = processoRaiz->IOsRealizados ; i < processoRaiz->quantidadeIO + processoRaiz->IOsRealizados ; i++)
            {
                if (processoRaiz->chamadasIO[i])
                    free(processoRaiz->chamadasIO[i]);
            }

            free(processoRaiz->chamadasIO);
        }

        if (processoRaiz)
            free(processoRaiz);

        prox = (*no)->proximo;            
    
        while (prox != *no) // Itera na lista inteira
        {
            aux = prox;

            if (prox->processo->chamadasIO)
            {
                for (int i = prox->processo->IOsRealizados ; i < prox->processo->quantidadeIO + prox->processo->IOsRealizados ; i++)
                {
                    if (prox->processo->chamadasIO[i])
                        free(prox->processo->chamadasIO[i]);
                }

                free(prox->processo->chamadasIO);
            }

            if (prox->processo)
                free(prox->processo);

            prox = prox->proximo;

            free(aux);
        }
        
        free(*no);
    }

    
}

void liberaDispositivos(NoIO **no)
{
    NoIO *aux, *prox;

    if (*no)
    {
        if ((*no)->io)
        {
            if ((*no)->io->solicitante) // Caso em que o processo estava realizando IO na parada do programa. Logo, ele não estava na lista de processos.
            {
                Processo *processoSolicitante = (*no)->io->solicitante;
                
                if (processoSolicitante->chamadasIO)
                {
                    /* Mesmo raciocínio na liberação de processos, porém iremos liberar a cabeça na linha à frente. Logo, ocorrerá double free. */

                    for (int i = processoSolicitante->IOsRealizados + 1; i < processoSolicitante->quantidadeIO + processoSolicitante->IOsRealizados ; i++)
                    {
                        if (processoSolicitante->chamadasIO[i])
                            free(processoSolicitante->chamadasIO[i]);
                    }

                    free(processoSolicitante->chamadasIO);
                }

                free(processoSolicitante);
            }

            free((*no)->io);
        }            

        prox = (*no)->proximo;
    
        while (prox != *no)
        {
            aux = prox;

            if (prox->io)
            {
                if (prox->io->solicitante)
                {
                    if (prox->io->solicitante->chamadasIO)
                    {
                        for (int i = prox->io->solicitante->IOsRealizados + 1 ; i < prox->io->solicitante->quantidadeIO + prox->io->solicitante->IOsRealizados ; i++)
                        {
                            if ((prox->io->solicitante->chamadasIO[i]))
                                free(prox->io->solicitante->chamadasIO[i]);
                        }

                        free(prox->io->solicitante->chamadasIO);
                    }

                    free(prox->io->solicitante);
                }
                free(prox->io);
            }

            prox = prox->proximo;
            
            free(aux);
        }

        free(*no);
    }    
}

void liberaRecursos(int signum)
{
    printf("\nEncerrando o escalonador. Liberando recursos...\n");

    liberaProcessos(&baixaPrioridade);
    liberaProcessos(&altaPrioridade);

    liberaDispositivos(&filaFita);
    liberaDispositivos(&filaDisco);
    liberaDispositivos(&filaImpressora);    

    exit(0);
}

// Redireciona os sinais para liberação de recursos

#ifdef _WIN32 // Windows não suporta a implementação de sigaction

void alteraComportamentoSigTermInt() 
{
    signal(SIGTERM, liberaRecursos);
    signal(SIGINT, liberaRecursos);    
}

#else

void alteraComportamentoSigTermInt()
{
    struct sigaction sa;

    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    sa.sa_handler = liberaRecursos;

    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);    
}

#endif

