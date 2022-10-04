#ifndef TIPOS_H
#define TIPOS_H

typedef struct Processo Processo;
typedef struct IO IO;
typedef struct NoProcesso NoProcesso;
typedef struct NoIO NoIO;

struct Processo 
{
    int pid;
    int tempoServico;
    int tempoChegada;
    int status;
    int tempoExecutado;
    int prioridade;
    int quantumMomentaneo;
    int quantidadeIO;
    int IOsRealizados;
    IO **chamadasIO;
};

struct IO 
{
    int tipo;
    int tempoEntrada;
    int duracao;
    int restante;
    Processo *solicitante;    
};

struct NoProcesso
{
    NoProcesso* proximo;
    NoProcesso* anterior;
    Processo* processo;    
};

struct NoIO
{
    NoIO* proximo;
    NoIO* anterior;
    IO *io;
};



#endif