CC = gcc
INCLUDES = ./include
WARNING = -Wall
FLAGS =  $(WARNING) -I $(INCLUDES) $(DEFINES) -o
SOURCES = processo.c arquivo.c escalonador.c main.c
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

escalonador: fila.o limpeza.o $(SOURCES)
	$(CC) $(OBJETOS) $(SOURCES) $(FLAGS) $(TARGET)

fila.o: util/fila.c
	$(CC) -c util/fila.c -I $(INCLUDES) $(WARNING)

limpeza.o: util/limpeza.c
	$(CC) -c util/limpeza.c -I $(INCLUDES) $(WARNING)

clean:
	rm *.o

