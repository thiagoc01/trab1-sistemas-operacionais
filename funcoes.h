int t = 0;
int pid_atual = 1;

struct Processo {
  int pid;
  int tempoServico;
  Processo* proximo;
  Processo* anterior;
  IO* chamadasIO;
}

struct IO {
  int  tipo;
  int tempoEntrada;
  IO* proximo;
  IO* anterior;
}

Processo *filaBaixa = null, *filaAlta = null, *filaEspera = null, *entrada = null;

Processo *filaDisco = null, *filaImpress = null, *filaFita = null;


Processo* criaProcesso(int tempoServico);
{
  Proceso* proc = (Processo *) malloc (sizeof(Processo*));
  proc->pid = pid_atual++;
  proc-> tempoServico = tempoServico;

  return proc;
}

void adicionaIO(Processo* proc, int tipo, int tempoEntrada)
{
  IO* io = (IO *) malloc (sizeof(IO*));
  io->tempoEntrada = tempoEntrada;
  io->tipo = tipo;


  if(proc->chamadasIO == null)
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
  if(proc->chamadasIO == null)
  {
    printf("Erro, pop em fila vazia de IO");
    exit(-1);
  }
  else if (proc->chamadasIO->proximo == proc->chamadasIO)
  {
    IO* pop = proc->chamadasIO;
    proc->chamadasIO = null;
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
  if(primeiro == null)
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
  if(primeiro == null)
  {
    printf("Erro, pop em fila vazia");
    exit(-1);
  }
  else if (primeiro->proximo == primeiro)
  {
    Processo* pop = primeiro;
    primeiro = null;
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
  while(fila != null && 
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
  for (int i = 0; i < procs ++i)
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
}