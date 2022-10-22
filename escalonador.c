#include <tipos.h>
#include <constantes.h>
#include <processo.h>
#include <escalonador.h>
#include <limpeza.h>
#include <arquivo.h>

#include <stdlib.h>
#include <unistd.h>

static void realizaPreparacaoIO(NoProcesso **fila, Processo *atual); // Função utilizada para modularização da preparação para realizar E/S

int t = 0;
int pid_atual = 1;
int processosRodando = 0;
int utilizaEntrada = 0; // Programa irá usar entrada via arquivo

/* Utilizadas para cálculo do instante de retorno de operação de IO */

int tamanhoFilaFita = 0, tamanhoFilaDisco = 0, tamanhoFilaImpressora = 0;

NoProcesso *entrada = NULL; // Lista de processos criados via arquivo
NoProcesso *baixaPrioridade = NULL, *altaPrioridade = NULL;
NoIO *filaFita = NULL, *filaImpressora = NULL, *filaDisco = NULL;

char *caminhoArquivoEntrada = NULL;

void escalonaProcessosPrevios()
{
    //Caso o modo de criação dos processos seja manual via arquivo 
    //de entrada, le os casos do arquivo selecionado

    carregaEntrada(caminhoArquivoEntrada, &entrada);

    for ( ; ; t++) 
    {
        printf("\nInstante %d\n", t);
        printf("=================================\n");

        verificaChegadaProcesso();

        realizaControleFilas();                        

        imprimeInformacoesFilas();

        #ifdef PARADA_ITERACAO
        
            getchar();
        
        #else

            sleep(1);  

        #endif     
    }
}

void escalonaProcessosRandomicos()
{
    #ifdef EXECUCAO_EM_10_MIN  // Solicitado no relatório

    for ( ; t != 600 ; t++)

    #else

    for ( ; ; t++) 

    #endif
    {
        printf("\nInstante %d\n", t);
        printf("=================================\n");

        verificaChegadaProcesso();

        realizaControleFilas();                   

        imprimeInformacoesFilas();

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

static void realizaPreparacaoIO(NoProcesso **fila, Processo *atual)
{
    int tipo = atual->chamadasIO[atual->IOsRealizados]->tipo, offset = 0;

    NoIO *novoNo = (NoIO *) malloc(sizeof(NoIO));
    
    novoNo->io = atual->chamadasIO[atual->IOsRealizados];                    
        
    if (tipo == IO_FITA)
    {
        if (tamanhoFilaFita)
            offset = (tamanhoFilaFita - 1) * TEMPO_IO_FITA + filaFita->io->restante;

        adicionaDispositivoFila(&filaFita, &novoNo, &tamanhoFilaFita);
        
    }

    else if (tipo == IO_DISCO)
    {
        if (tamanhoFilaDisco)
            offset = (tamanhoFilaDisco - 1) * TEMPO_IO_DISCO + filaDisco->io->restante;

        adicionaDispositivoFila(&filaDisco, &novoNo, &tamanhoFilaDisco);        
    }
    
    else
    {
        if (tamanhoFilaImpressora)
            offset = (tamanhoFilaImpressora - 1) * TEMPO_IO_IMPRESSORA + filaImpressora->io->restante;

        adicionaDispositivoFila(&filaImpressora, &novoNo, &tamanhoFilaImpressora);            
    }

    printf(RED "Processo %d irá realizar operação de %s. Retornará no instante %d \n\n" COLOR_RESET,
                atual->pid,
                TIPO_IO(tipo),
                t + atual->chamadasIO[atual->IOsRealizados]->duracao + offset);               


    retiraProcessoFila(fila, NULL, 0);
    
    atual->quantumMomentaneo = MAX_QUANTUM;
}

void controlaFilaProcesso(NoProcesso **fila)
{
    Processo *atual = (*fila)->processo; // Pega o nó da cabeça

    if (atual->tempoChegada != t)
    {
        if (atual->quantidadeIO && atual->chamadasIO[atual->IOsRealizados]->tempoEntrada == atual->tempoExecutado)
        {
            realizaPreparacaoIO(fila, atual);
            return;
        }

        atual->tempoServico--;
        atual->tempoExecutado++;
        atual->quantumMomentaneo--;
        
        if (atual->tempoServico == 0)
        {
            printf(BMAG "Processo %d encerrou.\n\n" COLOR_RESET, atual->pid);

            processosRodando--;

            retiraProcessoFila(fila, NULL, 0);

            liberaProcesso(&atual);
            
            return;
        }

        if (atual->quantidadeIO && atual->chamadasIO[atual->IOsRealizados]->tempoEntrada == atual->tempoExecutado)
            realizaPreparacaoIO(fila, atual);

        else if (atual->quantumMomentaneo == 0 && atual->tempoServico != 0)
        {
            printf(RED "Processo %d chegou ao fim da fatia de tempo.\n\n" COLOR_RESET, atual->pid);

            atual->quantumMomentaneo = MAX_QUANTUM;
            retiraProcessoFila(fila, NULL, 0);

            NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));

            novoNo->processo = atual;

            adicionaProcessoFila(&baixaPrioridade, &novoNo);
        }

        else if (atual->quantumMomentaneo == 0 && atual->tempoServico== 0)
        {
            printf(RED "Processo %d chegou ao fim da fatia de tempo e tempo de serviço. Irá encerrar.\n\n" COLOR_RESET, atual->pid);

            retiraProcessoFila(fila,NULL,0);

            processosRodando--;

            liberaProcesso(&atual);
        }
    }    
}

void checaTempoEntradaProcesso(NoProcesso **entrada)
{
    if (*entrada)
    {
        while (*entrada && (*entrada)->processo->tempoChegada == t)
        {
            Processo *proc = NULL;
            retiraProcessoFila(entrada, &proc, 1);           
            NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));
            novoNo->processo = proc;
            adicionaProcessoFila(&altaPrioridade, &novoNo);

            printf(GRN "Processo %d criado.\n\n", proc->pid );

            imprimeInformacoesProcesso(proc);
            processosRodando++;
        }  
    }

    else if (!processosRodando)
    {
        printf("Não há mais processos a serem realizados.");
        liberaRecursos(0);
    }
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
            {
                verificaChegadaProcesso();
                adicionaProcessoFila(&altaPrioridade, &novoNo);
            }
                

            switch (io->tipo)
            {
                case IO_DISCO:
                    retiraDispositivoFila(fila, &tamanhoFilaDisco);
                    break; 

                case IO_FITA:
                    retiraDispositivoFila(fila, &tamanhoFilaFita);
                    break; 

                case IO_IMPRESSORA:
                    retiraDispositivoFila(fila, &tamanhoFilaImpressora);
                    break; 
            }                

            free(io);
        }
    
}

void realizaControleFilas()
{
    if (filaFita)
        controlaFilaDispositivo(&filaFita);
    
    if (filaDisco)
        controlaFilaDispositivo(&filaDisco);
    
    if (filaImpressora)
        controlaFilaDispositivo(&filaImpressora); 

    if (altaPrioridade)
    {
        /* Caso em que existe um processo na fila de baixa prioridade executando. A fila de alta deve aguardar. */

        if (!(baixaPrioridade && baixaPrioridade->processo->quantumMomentaneo != MAX_QUANTUM))
            controlaFilaProcesso(&altaPrioridade);
        else
            controlaFilaProcesso(&baixaPrioridade);
    } 
    
    else if (baixaPrioridade)
        controlaFilaProcesso(&baixaPrioridade);       
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

    if (fila == baixaPrioridade && altaPrioridade && baixaPrioridade->processo->quantumMomentaneo == MAX_QUANTUM) // Se há processo na fila de alta prioridade, não devemos imprimir o quantum restante.
    {
        printf("Processo %d com tempo de serviço %d, %d solicitação(ões) de IO.\n\n",
                    fila->processo->pid,
                    fila->processo->tempoServico,
                    fila->processo->quantidadeIO);
        
    }

    else
    {
        printf("Processo %d com tempo de serviço %d, %d solicitação(ões) de IO e quantum restante de %d u.t..\n\n",
                        fila->processo->pid,
                        fila->processo->tempoServico,
                        fila->processo->quantidadeIO,
                        fila->processo->quantumMomentaneo);
    }    

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

void verificaChegadaProcesso()
{
    if (utilizaEntrada)
        checaTempoEntradaProcesso(&entrada);

    else
    {
        if (processosRodando < MAX_PROCESSOS && !(rand() % 5))
        {
            Processo *novo = criaProcesso(&altaPrioridade, pid_atual, t, 0, 0, 1);
            processosRodando++;
            pid_atual++;

            printf(GRN "Processo %d criado.\n\n", novo->pid);

            imprimeInformacoesProcesso(novo);
        }  
    }

}