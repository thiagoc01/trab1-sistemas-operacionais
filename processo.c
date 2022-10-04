#include <tipos.h>
#include <fila.h>
#include <constantes.h>
#include <processo.h>

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

void liberaProcesso(Processo **processo)
{
    if ((*processo)->chamadasIO)
        free((*processo)->chamadasIO);

    free(*processo);
}

void geraIOProcesso(Processo **novo)
{
    int numeroIOs = rand() % 4; // O máximo de operação de E/S que podee ser feitas é de 3.

    if (numeroIOs >= (*novo)->tempoServico) // Caso em que temos mais operações que tempo de serviço
        numeroIOs = rand() % ((*novo)->tempoServico); // Limitamos para o tempo - 1

    (*novo)->quantidadeIO = numeroIOs;

    if (numeroIOs != 0)
    {
        (*novo)->chamadasIO = (IO **) malloc(sizeof(IO *) * numeroIOs); // Aloca o vetor de operações

        int temposIO[numeroIOs]; // Guarda os tempos de IO para atribuição

        /*
            Devemos tomar cuidado ao atribuir os valores.
            Assume-se que o vetor está ordenado.
            Por isso, os valores devem ser crescentes.
            Entretanto, se o primeiro valor já é o tempo de serviço - 1,
            isso causa problema.
            Logo, subtraímos o tempo da quantidade de IOs e somamos a iteração para ter o pior caso como teto,
            que é uma sequência de números consecutivos até o tempo de serviço.
        */

        temposIO[0] = rand() % ((*novo)->tempoServico - numeroIOs + 1); // Somamos um para o caso em que a subtração pode dar 0.

        for (int i = 1 ; i < numeroIOs ; i++)
        {
            temposIO[i] = rand() % ((*novo)->tempoServico - numeroIOs + i);

            while (temposIO[i - 1] >= temposIO[i]) // Caso em que o anterior é maior ou igual
                temposIO[i] = rand() % ((*novo)->tempoServico - numeroIOs + i + 1);
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