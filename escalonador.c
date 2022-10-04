#include "tipos.h"
#include "constantes.h"
#include "processo.h"
#include "escalonador.h"

#include <stdlib.h>
#include <unistd.h>

int t = 0;
int pid_atual = 1;
int processosRodando = 0;

NoProcesso *baixaPrioridade = NULL, *altaPrioridade = NULL;
NoIO *filaFita = NULL, *filaImpressora = NULL, *filaDisco = NULL;

void escalonaProcessos()
{
    for ( ; ; t++)
    {
        printf("Instante %d\n", t);
        printf("=================================\n");

        imprimeInformacoesFilas();

        if (processosRodando < MAX_PROCESSOS && !(rand() % 5))
        {
            Processo *novo = criaProcesso(&altaPrioridade, pid_atual, t);
            processosRodando++;
            pid_atual++;

            imprimeInformacoesProcesso(novo);
        }

        if (filaFita)
            controlaFilaDispositivo(&filaFita);
        
        if (filaDisco)
            controlaFilaDispositivo(&filaDisco);
        
        if (filaImpressora)
            controlaFilaDispositivo(&filaImpressora);  

        if (altaPrioridade)
            controlaFilaProcesso(&altaPrioridade);
        
        else if(baixaPrioridade)
            controlaFilaProcesso(&baixaPrioridade);        
              
        
        sleep(1);

        getchar();
    }
}

void controlaFilaProcesso(NoProcesso **fila)
{
    Processo *atual = (*fila)->processo;

    if (atual->tempoServico == 0)
    {
        printf("Processo %d encerrou.\n", atual->pid);
        processosRodando--;
        retiraProcessoFila(fila);
        liberaProcesso(atual);
    }

    else if (atual->quantidadeIO && atual->chamadasIO[atual->IOsRealizados]->tempoEntrada == atual->tempoExecutado )
    {
        int tipo = atual->chamadasIO[atual->IOsRealizados]->tipo;

        printf("Processo %d irá realizar operação de %s. Retornará no instante %d \n",
                    atual->pid,
                    TIPO_IO(tipo),
                    atual->chamadasIO[atual->IOsRealizados]->duracao + t);

        NoIO *novoNo = (NoIO *) malloc(sizeof(NoIO));
        novoNo->io = atual->chamadasIO[atual->IOsRealizados++];                    
            
        if (tipo == IO_FITA)
            adicionaDispositivoFila(&filaFita, &novoNo);

        else if (tipo == IO_DISCO)
            adicionaDispositivoFila(&filaDisco, &novoNo);
        
        else
            adicionaDispositivoFila(&filaImpressora, &novoNo);                


        retiraProcessoFila(fila);
        atual->quantidadeIO--;
        atual->quantumMomentaneo = MAX_QUANTUM;
    }

    else if (atual->quantumMomentaneo == 0)
    {
        printf("Processo %d chegou ao fim da fatia de tempo.\n", atual->pid);

        atual->quantumMomentaneo = MAX_QUANTUM;
        retiraProcessoFila(fila);

        NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));

        novoNo->processo = atual;

        adicionaProcessoFila(&baixaPrioridade, &novoNo);
    }

    atual->tempoServico--;
    atual->tempoExecutado++;
    atual->quantumMomentaneo--;

}

void controlaFilaDispositivo(NoIO **fila)
{
    IO *io = (*fila)->io;

    io->restante--;
    
    if (io->restante)
    {
        printf("Resta %d u.t. para o processo %d encerrar a operação de %s.\n",
                                io->restante,
                                io->solicitante->pid,
                                TIPO_IO(io->tipo));
        
    }

    else
    {
        printf("Processo %d encerrou a operação de %s.\n", io->solicitante->pid, TIPO_IO(io->tipo));        

        NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));
        novoNo->processo = io->solicitante;

        if (io->tipo == IO_DISCO)
            adicionaProcessoFila(&baixaPrioridade, &novoNo);
        
        else
            adicionaProcessoFila(&altaPrioridade, &novoNo);

        retiraDispositivoFila(fila);

        free(io);
    }   

}

void imprimeInformacoesFilas()
{
    imprimeInformacoesFilasProcessos(baixaPrioridade, "baixa prioridade");
    imprimeInformacoesFilasProcessos(altaPrioridade, "alta prioridade");
    imprimeInformacoesFilasDispositivos(filaFita, "fita");
    imprimeInformacoesFilasDispositivos(filaDisco, "disco");
    imprimeInformacoesFilasDispositivos(filaImpressora, "impressora");
}

void imprimeInformacoesFilasProcessos(NoProcesso *fila, const char *tipoFila)
{
    if (!fila)
    {
        printf("Não há processos na fila de ");
        printf("%s", tipoFila);
        puts(".");
        return;
    }

    printf("Fila de ");
    printf("%s", tipoFila);
    puts(": ");
    printf("=================================\n");

    printf("Processo %d com tempo de serviço %d e %d solicitações de IO.\n",
                        fila->processo->pid,
                        fila->processo->tempoServico,
                        fila->processo->quantidadeIO);

    NoProcesso *proximo = fila->proximo;

    while (proximo != fila)
    {
        printf("Processo %d com tempo de serviço %d e %d solicitações de IO.\n",
                        proximo->processo->pid,
                        proximo->processo->tempoServico,
                        proximo->processo->quantidadeIO);
        proximo = proximo->proximo;
    }

    printf("=================================\n");
}

void imprimeInformacoesFilasDispositivos(NoIO *fila, const char* tipoFila)
{
    if (!fila)
    {
        printf("Não há processos na fila de ");
        printf("%s", tipoFila);
        puts(".");
        return;
    }

    printf("Fila de ");
    printf("%s", tipoFila);
    puts(": ");
    printf("=================================\n");

    printf("Processo %d\n", fila->io->solicitante->pid);

    NoIO *proximo = fila->proximo;

    while (proximo != fila)
    {
        printf("Processo %d\n", fila->io->solicitante->pid);

        proximo = proximo->proximo;
    }

    printf("=================================\n");
}