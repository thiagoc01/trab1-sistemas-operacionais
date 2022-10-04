#include "tipos.h"
#include "fila.h"
#include "constantes.h"
#include "processo.h"

Processo *criaProcesso(NoProcesso **cabeca, int pid, int chegada)
{
    Processo *novo = (Processo *) malloc(sizeof(Processo));
    NoProcesso *novoNo = (NoProcesso *) malloc(sizeof(NoProcesso));

    novo->pid = pid;
    novo->prioridade = ALTA_PRIOR;
    novo->quantumMomentaneo = MAX_QUANTUM;
    novo->status = PRONTO;
    novo->tempoServico = 1 + rand() % 20;
    novo->tempoChegada = chegada;
    novo->tempoExecutado = 0;
    novo->IOsRealizados = 0;

    geraIOProcesso(&novo);

    novoNo->processo = novo;
    adicionaProcessoFila(cabeca, &novoNo);

    return novo;
}

void liberaProcesso(Processo *processo)
{
    free(processo->chamadasIO);
    free(processo);
}

void geraIOProcesso(Processo **novo)
{
    int numeroIOs = rand() % 4;

    if (numeroIOs > (*novo)->tempoServico)
        numeroIOs = rand() % ((*novo)->tempoServico);

    (*novo)->quantidadeIO = numeroIOs;

    if (numeroIOs != 0)
    {
        (*novo)->chamadasIO = (IO **) malloc(sizeof(IO *) * numeroIOs);

        int temposIO[numeroIOs];

        temposIO[0] = rand() % ((*novo)->tempoServico - numeroIOs);

        for (int i = 1 ; i < numeroIOs ; i++)
        {
            temposIO[i] = rand() % ((*novo)->tempoServico - numeroIOs + i);

            while (temposIO[i - 1] >= temposIO[i])
                temposIO[i] = rand() % ((*novo)->tempoServico - numeroIOs + i);
        }

        for (int i = 0 ; i < numeroIOs ; i++)
        {
            IO *novoIO = (IO *) malloc(sizeof(IO));

            novoIO->tipo = 1 + rand() % 3;

            switch (novoIO->tipo)
            {
                case IO_DISCO:
                    novoIO->duracao = TEMPO_IO_DISCO;
                    break;
                
                case IO_FITA:
                    novoIO->duracao = TEMPO_IO_FITA;
                    break;

                case IO_IMPRESSORA:
                    novoIO->duracao = TEMPO_IO_IMPRESSORA;
                    break;
            }

            novoIO->tempoEntrada = temposIO[i];

            novoIO->solicitante = *novo;

            novoIO->restante = novoIO->duracao;

            (*novo)->chamadasIO[i] = novoIO;             
        }
    }

    else
        (*novo)->chamadasIO = NULL;
}

void imprimeInformacoesProcesso(Processo *processo)
{
    printf("Processo %d criado.\n", processo->pid);

    printf("Informações do processo: \n");
    printf("=================================\n");

    printf("Tempo de serviço: %d\n", processo->tempoServico);
    printf("Tempo de chegada: %d\n", processo->tempoChegada);
    printf("Informações sobre IO: \n");

    if (!processo->chamadasIO)
        puts("O processo não irá pedir IO.");
    
    else
    {
        for (int i = 0 ; i < processo->quantidadeIO ; i++)
        {
            printf("Tipo: %d    Tempo: %d   Duração: %d\n",
                                processo->chamadasIO[i]->tipo,
                                processo->chamadasIO[i]->tempoEntrada,
                                processo->chamadasIO[i]->duracao);
        }
    }

}