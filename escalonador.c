#include <tipos.h>
#include <constantes.h>
#include <processo.h>
#include <escalonador.h>
#include <limpeza.h>

#include <stdlib.h>
#include <unistd.h>

int t = 0;
int pid_atual = 1;
int processosRodando = 0;

NoProcesso *baixaPrioridade = NULL, *altaPrioridade = NULL;
NoIO *filaFita = NULL, *filaImpressora = NULL, *filaDisco = NULL;

void escalonaProcessos()
{
    #ifdef EXECUCAO_EM_10_MIN // Solicitado no relatório

    for ( ; t != 600 ; t++)

    #else

    for ( ; ; t++) 

    #endif
    {
        printf("\nInstante %d\n", t);
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

        #ifdef PARADA_ITERACAO
        
            getchar();
        
        #else

            sleep(1);  

        #endif     
    }

    #ifdef EXECUCAO_EM_10_MIN

        liberaRecursos(0);

    #endif
}

void controlaFilaProcesso(NoProcesso **fila)
{
    Processo *atual = (*fila)->processo; // Pega o nó da cabeça

    if (atual->tempoServico == 0)
    {
        printf(BMAG "Processo %d encerrou.\n\n" COLOR_RESET, atual->pid);

        processosRodando--;

        retiraProcessoFila(fila);

        liberaProcesso(&atual);
        
        return;
    }

    if (atual->quantidadeIO && atual->chamadasIO[atual->IOsRealizados]->tempoEntrada == atual->tempoExecutado )
    {
        int tipo = atual->chamadasIO[atual->IOsRealizados]->tipo;

        printf(RED "Processo %d irá realizar operação de %s. Retornará no instante %d \n\n" COLOR_RESET,
                    atual->pid,
                    TIPO_IO(tipo),
                    atual->chamadasIO[atual->IOsRealizados]->duracao + t);

        NoIO *novoNo = (NoIO *) malloc(sizeof(NoIO));
        
        novoNo->io = atual->chamadasIO[atual->IOsRealizados];                    
            
        if (tipo == IO_FITA)
            adicionaDispositivoFila(&filaFita, &novoNo);

        else if (tipo == IO_DISCO)
            adicionaDispositivoFila(&filaDisco, &novoNo);
        
        else
            adicionaDispositivoFila(&filaImpressora, &novoNo);                


        retiraProcessoFila(fila);
        
        atual->quantumMomentaneo = MAX_QUANTUM;
    }

    else if (atual->quantumMomentaneo == 0 && atual->tempoServico - 1 != 0)
    {
        printf(RED "Processo %d chegou ao fim da fatia de tempo.\n\n" COLOR_RESET, atual->pid);

        atual->quantumMomentaneo = MAX_QUANTUM;
        retiraProcessoFila(fila);

        NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));

        novoNo->processo = atual;

        adicionaProcessoFila(&baixaPrioridade, &novoNo);
    }

    else if (atual->quantumMomentaneo == 0 && atual->tempoServico - 1 == 0)
    {
        printf(RED "Processo %d chegou ao fim da fatia de tempo e tempo de serviço. Irá encerrar.\n\n" COLOR_RESET, atual->pid);

        retiraProcessoFila(fila);

        processosRodando--;

        liberaProcesso(&atual);

        return;
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
        printf(RED "Resta(m) %d u.t. para o processo %d encerrar a operação de %s.\n\n" COLOR_RESET,
                                io->restante,
                                io->solicitante->pid,
                                TIPO_IO(io->tipo));
        
    }

    else
    {
        printf(RED "Processo %d encerrou a operação de %s.\n\n" COLOR_RESET, io->solicitante->pid, TIPO_IO(io->tipo));        

        NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));
        novoNo->processo = io->solicitante;

        io->solicitante->IOsRealizados++;
        io->solicitante->quantidadeIO--;

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
        printf(YEL "Não há processos na fila de ");
        printf("%s", tipoFila);
        puts(".\n" COLOR_RESET);
        return;
    }

    printf(CYN "Fila de ");
    printf("%s", tipoFila);
    puts(": ");
    printf("=================================\n\n");

    printf("Processo %d com tempo de serviço %d, %d solicitação(ões) de IO e quantum restante de %d u.t..\n\n",
                        fila->processo->pid,
                        fila->processo->tempoServico,
                        fila->processo->quantidadeIO,
                        fila->processo->quantumMomentaneo);

    NoProcesso *proximo = fila->proximo;

    while (proximo != fila)
    {
        printf("Processo %d com tempo de serviço %d e %d solicitação(ões) de IO.\n\n",
                        proximo->processo->pid,
                        proximo->processo->tempoServico,
                        proximo->processo->quantidadeIO);
        proximo = proximo->proximo;
    }

    printf("=================================\n\n" COLOR_RESET);
}

void imprimeInformacoesFilasDispositivos(NoIO *fila, const char* tipoFila)
{
    if (!fila)
    {
        printf(YEL "Não há processos na fila de ");
        printf("%s", tipoFila);
        puts(".\n" COLOR_RESET);
        return;
    }

    printf(CYN "Fila de ");
    printf("%s", tipoFila);
    puts(": ");
    printf("=================================\n\n");

    printf("Processo %d\n\n", fila->io->solicitante->pid);

    NoIO *proximo = fila->proximo;

    while (proximo != fila)
    {
        printf("Processo %d\n\n", proximo->io->solicitante->pid);

        proximo = proximo->proximo;
    }

    printf("=================================\n\n" COLOR_RESET);
}