#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <tipos.h>

#define MAX_PROCESSOS 5
#define MAX_QUANTUM 5

#define TEMPO_IO_DISCO 3
#define TEMPO_IO_FITA 5
#define TEMPO_IO_IMPRESSORA 7

#define IO_FITA 1
#define IO_DISCO 2
#define IO_IMPRESSORA 3

#define TIPO_IO(x)  ( (x) == IO_FITA ? "fita" : (x) == IO_DISCO ? "disco" : "impressora")

/* Cores para output */

#define BMAG "\e[0;95m"
#define RED "\e[0;91m"
#define GRN "\e[0;92m"
#define YEL "\e[0;93m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;96m"
#define WHT "\e[0;37m"
#define COLOR_RESET "\e[0m"

#endif
