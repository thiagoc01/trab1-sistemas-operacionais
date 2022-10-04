#include "fila.h"
#include "constantes.h"

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

void adicionaDispositivoFila(NoIO **cabeca, NoIO **dispositivo)
{
    if (*cabeca == NULL)
    {
        (*dispositivo)->anterior = *dispositivo;
        (*dispositivo)->proximo = *dispositivo;
        *cabeca = *dispositivo;
        
    }

    else
    {
        (*cabeca)->anterior->proximo = *dispositivo;
        (*dispositivo)->anterior = (*cabeca)->anterior;
        (*dispositivo)->proximo = *cabeca;
        (*cabeca)->anterior = *dispositivo;
    }
}

void retiraProcessoFila(NoProcesso **cabeca)
{
    if (*cabeca)
    {
        NoProcesso *pop = *cabeca;

        if ((*cabeca)->anterior == *cabeca && (*cabeca)->proximo == *cabeca)
            *cabeca = NULL;

        else
        {
            (*cabeca)->anterior->proximo = (*cabeca)->proximo;

            *cabeca = pop->proximo;
            (*cabeca)->anterior = pop->anterior;
        }
        
        

        free(pop);
    }
}

void retiraDispositivoFila(NoIO **cabeca)
{
    if (*cabeca)
    {
        NoIO *pop = *cabeca;
        
        if ((*cabeca)->anterior == *cabeca && (*cabeca)->proximo == *cabeca)
            *cabeca = NULL;

        else
        {
            (*cabeca)->anterior->proximo = (*cabeca)->proximo;

            *cabeca = pop->proximo;
            (*cabeca)->anterior = pop->anterior;
        }

        free(pop);
    }
}

/*void adicionaIO(Processo* proc, int tipo, int tempoEntrada)
{
  IO* io = (IO *) malloc (sizeof(IO*));
  io->tempoEntrada = tempoEntrada;
  io->tipo = tipo;

  if(proc->chamadasIO == NULL)
  {
    proc->chamadasIO = io;
    proc->chamadasIO->proximo = proc->chamadasIO;
    proc->chamadasIO->anterior = proc->chamadasIO;
  }
  else
  {
    io->proximo = proc->chamadasIO;
    io->anterior = proc->chamadasIO->anterior;
    proc->chamadasIO->anterior = io;
  }
}

IO* popIO(Processo *proc)
{
  if(proc->chamadasIO == NULL)
  {
    printf("Erro, pop em fila vazia de IO");
    exit(-1);
  }
  else if (proc->chamadasIO->proximo == proc->chamadasIO)
  {
    IO* pop = proc->chamadasIO;
    proc->chamadasIO = NULL;
    return pop;
  }
  else {
    IO* pop = proc->chamadasIO;
    proc->chamadasIO->proximo->anterior = proc->chamadasIO->anterior;
    proc->chamadasIO = proc->chamadasIO->proximo;
    return pop;
  }
}

void appendProcesso(Processo *append)
{
  if(primeiro == NULL)
  {
    primeiro = append;
    primeiro->proximo = primeiro;
    primeiro->anterior = primeiro;
  }
  else
  {
    append->proximo = primeiro;
    append->anterior = primeiro->anterior;
    primeiro->anterior = append;
  }
}

Processo* popProcesso(Processo *primeiro)
{
  if(primeiro == NULL)
  {
    printf("Erro, pop em fila vazia");
    exit(-1);
  }
  else if (primeiro->proximo == primeiro)
  {
    Processo* pop = primeiro;
    primeiro = NULL;
    return pop;
  }
  else {
    Processo* pop = primeiro;
    primeiro->proximo->anterior = primeiro->anterior;
    primeiro = primeiro->proximo;
    return pop;
  }
}

void atualizaFila(Processo* fila)
{
  Processo* proc = fila;
  while(fila != NULL && 
    fila->chamadasIO->tempoEntrada + fila->chamadasIO->tempoServico == t)
  {
    popProcesso(fila);
  }
}

void carregaEntrada()
{
  int procs;
  FILE* file = fopen("entrada.txt","r");
  fscanf(file, "%d", procs);
  for (int i = 0; i < procs ; ++i)
  {
    int tempoServico, numIOs;
    fscanf(file,"%d %d", &tempoServico, &numIOs);
    proc = criaProcesso(tempoServico);

    for (int j = 0; j < numIOs; ++j)
    { 
      int tipoIO, tempoEntrada;
      scanf(file, "%d %d", &tipoIO, &tempoEntrada);
      adicionaIO(proc,tipoIO, tempoEntrada);
    }


  }
  fclose(file);
}*/
