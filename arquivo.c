#include <processo.h>
#include <arquivo.h>

extern int pid_atual;

void carregaEntrada(char *fileName,NoProcesso **entrada)
{
  int procs;
  FILE* file = fopen(fileName,"r");
  fscanf(file, "%d", &procs);
  for (int i = 0; i < procs;++i)
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
  printf("Entrada lida com sucesso!\n\n");
  fclose(file);
}
