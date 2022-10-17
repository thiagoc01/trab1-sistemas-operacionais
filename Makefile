CC = gcc
INCLUDES = ./include
FLAGS =  -Wall -I $(INCLUDES) $(DEFINES) -g -o
TARGET = escalonador
OBJETOS = fila.o limpeza.o
EXECUCAO_EM_10_MIN =
PARADA_ITERACAO = 

ifdef EXECUCAO_EM_10_MIN
	DEFINES += -D EXECUCAO_EM_10_MIN
endif

ifdef PARADA_ITERACAO
	DEFINES += -D PARADA_ITERACAO
endif

all: escalonador clean

escalonador: fila.o limpeza.o
	$(CC) $(OBJETOS) processo.c arquivo.c escalonador.c main.c $(FLAGS) escalonador

fila.o: 
	$(CC) -c util/fila.c -I $(INCLUDES)

limpeza.o:
	$(CC) -c util/limpeza.c -I $(INCLUDES)

clean:
	rm *.o

