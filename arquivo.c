#include <processo.h>
#include <arquivo.h>
#include <limpeza.h>

extern int pid_atual;

void carregaEntrada(char *fileName, NoProcesso **entrada)
{
    int procs;

    FILE* file = fopen(fileName,"r");

    if (!file)
    {
        printf("\nErro ao abrir o arquivo de entrada.\n\n");
        liberaRecursos(2);
        exit(2);
    }

    fscanf(file, "%d", &procs);

    for (int i = 0 ; i < procs ; ++i)
    {
        int tempoChegada, tempoServico, numIOs;

        fscanf(file,"%d %d %d", &tempoChegada,  &tempoServico, &numIOs);

        Processo *proc = criaProcesso(entrada, pid_atual++, tempoChegada,tempoServico, numIOs, 0);

        if(numIOs == 0)
            proc->chamadasIO = NULL;
        else
            proc->chamadasIO = (IO **) malloc(sizeof(IO *) * numIOs);

        for (int j = 0; j < numIOs; ++j)
        { 
            int tipoIO, tempoEntrada;
            fscanf(file, "%d %d", &tipoIO, &tempoEntrada);
            adicionaIO(&proc,j,tipoIO, tempoEntrada);
        }
    }

    NoProcesso *lista = *entrada;

    if (lista)
    {
        printf(GRN "Processo %d\n\n", lista->processo->pid);
        imprimeInformacoesProcesso(lista->processo);
    }

    lista = lista->proximo;

    while (lista != *entrada)
    {
        printf(GRN "Processo %d.\n\n", lista->processo->pid);
        imprimeInformacoesProcesso(lista->processo);
        lista = lista->proximo;
    }

    printf("Entrada lida com sucesso!\n\n");

    fclose(file);
}
